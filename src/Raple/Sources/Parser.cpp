//-------------------------------------
#include "../Headers/Parser.h"
#include "../Headers/Constants.h"
//-------------------------------------


namespace Raple
{
	Parser::Parser(ITokenizer *const tokenizer, const ILogger *logger)
			: ParserBase(tokenizer, logger)
	{
		_position = 0;
	}

	Parser::~Parser()
	{
	}

	TreeNode *Parser::ParseScript(const SourceCode *sourceCode)
	{
		_position = 0;
		_errorsWhileParsing = false;
		_syntaxError = false;
		_sourceCode = sourceCode;

		return parseScript();
	}

	void Parser::getNextToken(Token *token)
	{
		if (token == 0)
			return;

		do
		{
			if (_position >= _sourceCode->GetCodeLength())
			{
				token->Type = ttEndOfFile;
				token->Length = 0;
			}
			else
				token->Type = _tokenizer->GetTokenType(_sourceCode->GetCode() + _position, _sourceCode->GetCodeLength() - _position, &token->Length);

			token->Position = _position;
			_position += token->Length;

			_sourceCode->ConvertPositionToRowColumn(token->Position, &token->Row, 0);
		}
		while (token->Type == ttWhitespace ||
			   token->Type == ttComment);
	}

	void Parser::moveTo(const Token *token)
	{
		if (token == 0)
			return;

		_position = token->Position;
	}

	void Parser::unexpectedTokenError(const Token *token)
	{
		_errorsWhileParsing = true;
		_syntaxError = true;
		rstring line;

		int row, column;
		_sourceCode->ConvertPositionToRowColumn(token->Position, &row, &column);

		line.Format("Unexpected token '%s' at (%d,%d)", GetTokenDefinition(token->Type), row, column);
		_logger->Error(Constants::LogTitleParserError, line);
	}

	void Parser::expectedTokenError(const char *tokenText, int position)
	{
		_errorsWhileParsing = true;
		_syntaxError = true;
		rstring line;

		int row, column;
		_sourceCode->ConvertPositionToRowColumn(position, &row, &column);

		line.Format("Expected token '%s' at (%d,%d)", tokenText, row, column);
		_logger->Error(Constants::LogTitleParserError, line);
	}

	void Parser::match(TokenType type)
	{
		Token t;
		getNextToken(&t);

		if (type != t.Type)
			unexpectedTokenError(&t);
	}

	bool Parser::isConstant(TokenType tokenType)
	{
		return tokenType == ttBinConst ||
				tokenType == ttDecConst ||
				tokenType == ttFloatConst ||
				tokenType == ttHexConst ||
				tokenType == ttString ||
				tokenType == ttTrue || 
				tokenType == ttFalse;
	}

	bool Parser::isAssignOperator(TokenType tokenType)
	{
		return ttAssignment == tokenType;
	}

	bool Parser::isPreOperator(TokenType tokenType)
	{
		return tokenType == ttMinus || 
			   tokenType == ttIncrement || 
			   tokenType == ttDecrement;
	}

	bool Parser::isPostOperator(TokenType tokenType)
	{
		return tokenType == ttIncrement || 
			   tokenType == ttDecrement ||
			   tokenType == ttOpenBracket || 
			   tokenType == ttDot;
	}

	bool Parser::isLogicalOperator(TokenType tokenType)
	{
		return tokenType == ttAnd || tokenType == ttOr;
	}

	bool Parser::isFunctionCall()
	{
		Token t, t1;

		getNextToken(&t);
		getNextToken(&t1);
		moveTo(&t);

		return t.Type == ttIdentifier && t1.Type == ttOpenParanthesis;
	}

	bool Parser::isComparisonOperator(TokenType tokenType)
	{
		return tokenType == ttEqual || 
			tokenType == ttNotEqual ||
			tokenType == ttGreater || 
			tokenType == ttLower ||
			tokenType == ttGreaterEqual ||
			tokenType == ttLowerEqual;
	}

	TreeNode *Parser::parseScript()
	{
		TreeNode *node = new TreeNode(ntCode);
		Token t;

		while (!_syntaxError)
		{
			getNextToken(&t);
			moveTo(&t);

			if (t.Type == ttSub)
				node->AddChild(parseFunctionDefinition());
			else if (t.Type == ttImport)
				node->AddChild(parseImport());
			else if (t.Type == ttEndOfFile)
				break;
			else
				unexpectedTokenError(&t);

			if (_syntaxError)
			{	
				getNextToken(&t);

				while (t.Type != ttEndOfStatement && t.Type != ttEndOfFile)
					getNextToken(&t);

				_syntaxError = false;
			}
		}

		return node;
	}

	TreeNode *Parser::parseFunctionDefinition(bool nameRequired)
	{
		TreeNode *node = new TreeNode(ntSub);

		Token t;
		getNextToken(&t);

		if (t.Type != ttSub)
		{
			expectedTokenError("sub", t.Position);
			return(node);
		}

		if (nameRequired)
		{
			node->AddChild(parseIdentifier());
			if (_syntaxError)
				return(node);
		}
		else
			node->SetType(ntAnonymousSub);

		node->AddChild(parseArgList(false));
		if (_syntaxError)
			return(node);

		node->AddChild(parseStatementsBlock());

		return node;
	}

	TreeNode *Parser::parseFunctionCall()
	{
		TreeNode *node = new TreeNode(ntFunctionCall);

		node->AddChild(parseIdentifier());
		if (_syntaxError)
			return node;

		node->AddChild(parseArgList(true));

		return node;
	}

	TreeNode *Parser::parseIdentifier()
	{
		TreeNode *node = new TreeNode(ntIdentifier);

		Token t;
		getNextToken(&t);

		if (t.Type != ttIdentifier)
		{
			unexpectedTokenError(&t);
			return(node);
		}

		node->SetToken(&t);
		node->UpdateData(t.Position, t.Length, t.Row);

		return node;
	}

	TreeNode *Parser::parseConstant()
	{
		TreeNode *node = new TreeNode(ntConstant);

		Token t;
		getNextToken(&t);

		if (!isConstant(t.Type))
		{
			unexpectedTokenError(&t);
			return node;
		}

		if (t.Type == ttString)
		{
			// trim "
			++t.Position;
			t.Length -= 2;
		}

		node->SetToken(&t);

		return node;
	}

	TreeNode *Parser::parseArgList(bool isFunctionCall)
	{
		TreeNode *node = new TreeNode(isFunctionCall ? ntFunctionCallArgList : ntFunctionDefinitionArgList);

		Token t;
		getNextToken(&t);

		if (t.Type != ttOpenParanthesis)
		{
			expectedTokenError("(", t.Position);
			return node;
		}

		getNextToken(&t);
		if (t.Type != ttCloseParanthesis)
		{
			moveTo(&t);

			while (true)
			{
				if (isFunctionCall)
					node->AddChild(parseAssignment());
				else
					node->AddChild(parseIdentifier());

				if (_syntaxError)
					return node;

				getNextToken(&t);
				
				if (t.Type == ttCloseParanthesis)
					break;
				else if (t.Type == ttComma)
					continue;
				else
				{
					expectedTokenError(") or ,", t.Position);
					return node;
				}
			}
		}

		return node;
	}


	TreeNode *Parser::parseExpressionStatement()
	{
		TreeNode *node = 0;
		
		Token t;
		getNextToken(&t);
		moveTo(&t);

		if (t.Type == ttIf)
			node = parseIfElse();
		else if (t.Type == ttWhile)
			node = parseWhile();
		else if (t.Type == ttForEach)
			node = parseForEach();
		else
		{
			if (t.Type == ttVar)
				node = parseVarDeclaration();
			else if (t.Type == ttReturn)
				node = parseReturnExpression();
			else
				node = parseAssignment();

			if (_syntaxError)
				return node;

			match(ttEndOfStatement);
		}
	
		return node;
	}

	TreeNode *Parser::parseReturnExpression()
	{
		TreeNode *node = new TreeNode(ntReturn);

		Token t;
		getNextToken(&t);

		if (t.Type != ttReturn)
		{
			unexpectedTokenError(&t);
			return node;
		}

		getNextToken(&t);
		moveTo(&t);

		if (t.Type != ttEndOfStatement)
			node->AddChild(parseAssignment());
		
		return node;
	}

	TreeNode *Parser::parseStatementsBlock()
	{
		TreeNode *node = new TreeNode(ntStatementsBlock);

		Token t;
		getNextToken(&t);
		node->SetToken(&t);

		if (t.Type != ttOpenStatementBlock)
		{
			expectedTokenError("{", t.Position);
			return node;
		}

		while (true)
		{
			getNextToken(&t);

			if (t.Type == ttCloseStatementBlock)
			{
				node->UpdateData(t.Position, t.Length, t.Row);
				break;
			}
			else
			{
				moveTo(&t);
				node->AddChild(parseExpressionStatement());
			}

			if (_syntaxError)
			{
				getNextToken(&t);
				while (t.Type != ttEndOfStatement && t.Type != ttEndOfFile &&
					   t.Type != ttCloseStatementBlock)
				{
					getNextToken(&t);
				}
				
				if (t.Type == ttEndOfFile)
				{
					unexpectedTokenError(&t);
					return node;
				}

				_syntaxError = false;
			}
		}

		return node;
	}

	TreeNode *Parser::parseVarDeclaration(bool needVarKeyword, bool needAssignment)
	{
		TreeNode *node = new TreeNode(ntVarDeclaration);
		Token t;

		if (needVarKeyword)
		{
			getNextToken(&t);

			if (t.Type != ttVar)
			{
				unexpectedTokenError(&t);
				return node;
			}
		}

		node->AddChild(parseIdentifier());
		if (_syntaxError || !needAssignment)
			return node;

		getNextToken(&t);

		if (t.Type == ttAssignment)
		{
			getNextToken(&t);
			moveTo(&t);

			if (t.Type == ttOpenStatementBlock)
			{
				node->AddChild(parseInitializationList());
				if (_syntaxError)
					return node;
			}
			else
			{
				node->AddChild(parseAssignment());
				if (_syntaxError)
					return node;
			}
		}
		else
		{
			unexpectedTokenError(&t);
			return node;
		}

		getNextToken(&t);

		if (t.Type == ttComma)
			node->AddChild(parseVarDeclaration(false));
		else
			moveTo(&t);

		return node;
	}

	TreeNode *Parser::parseAssignment()
	{
		TreeNode *node = parseExpression();
		if (_syntaxError)
			return node;

		Token t;
		getNextToken(&t);

		if (isAssignOperator(t.Type))
			node = new TreeNode(ntAssignment, &t, node, parseAssignment());
		else
			moveTo(&t);

		return node;
	}

	TreeNode *Parser::parseExpression()
	{
		TreeNode *node = parseExpressionTerm();
		if (_syntaxError)
			return node;

		while (true)
		{
			Token t;
			getNextToken(&t);

			if (t.Type == ttAnd || t.Type == ttOr)
			{
				node = new TreeNode(ntBinaryOperator, &t, node, parseExpressionTerm());
				if (_syntaxError)
					return node;
			}
			else
			{
				moveTo(&t);
				break;
			}
		}
		
		return node;
	}

	TreeNode *Parser::parseExpressionTerm()
	{
		TreeNode *node = parseBinaryOperator();
		if (_syntaxError)
			return node;

		while (true)
		{
			Token t;
			getNextToken(&t);

			if (isComparisonOperator(t.Type))
			{
				node = new TreeNode(ntBinaryOperator, &t, node, parseBinaryOperator());
				if (_syntaxError)
					return node;
			}
			else
			{
				moveTo(&t);
				break;
			}
		}

		return node;
	}

	TreeNode *Parser::parseBinaryOperator()
	{
		TreeNode *node = parseLowPriorOperator();
		if (_syntaxError)
			return node;

		while (true)
		{
			Token t;
			getNextToken(&t);

			if (t.Type == ttPlus || t.Type == ttMinus)
			{
				node = new TreeNode(ntBinaryOperator, &t, node, parseLowPriorOperator());
				if (_syntaxError)
					return node;
			}
			else
			{
				moveTo(&t);
				break;
			}
		}

		return node;
	}

	TreeNode *Parser::parseLowPriorOperator()
	{
		TreeNode *node = parseHighPriorOperator();
		if (_syntaxError)
			return node;

		while (true)
		{
			Token t;
			getNextToken(&t);

			if (t.Type == ttStar || t.Type == ttSlash)
			{
				node = new TreeNode(ntBinaryOperator, &t, node, parseHighPriorOperator());
				if (_syntaxError)
					return node;
			}
			else
			{
				moveTo(&t);
				break;
			}
		}
		
		return node;
	}

	TreeNode *Parser::parseHighPriorOperator()
	{
		TreeNode *node = parsePreOperator();

		while (true)
		{
			Token t;
			getNextToken(&t);

			if (t.Type == ttExponentiation)
			{
				node = new TreeNode(ntBinaryOperator, &t, node, parsePreOperator());
				if (_syntaxError)
					return node;
			}
			else if (t.Type == ttOpenBracket)
			{
				node = new TreeNode(ntIndexing, &t, node, parseAssignment());
				if (_syntaxError)
					return node;

				match(ttCloseBracket);
			}
			else if (t.Type == ttIncrement || t.Type == ttDecrement)
			{
				TreeNode *postOpNode = new TreeNode(ntPostOperator);
				postOpNode->SetToken(&t);
				postOpNode->AddChild(node);
				node = postOpNode;
			}
			else if (t.Type == ttDot)
			{
				Token t1, t2;
				getNextToken(&t1);
				getNextToken(&t2);
				moveTo(&t1);

				if (t2.Type == ttOpenParanthesis)
					node = new TreeNode(ntFunctionCall, &t1, node, parseFunctionCall());
				else
				{
					// only internal function call is now implemented
					expectedTokenError("(, function call", t2.Position);
					return node;
				}
			}
			else
			{
				moveTo(&t);
				break;
			}
		}

		return node;
	}

	TreeNode *Parser::parsePreOperator()
	{
		Token t;
		getNextToken(&t);

		if (isPreOperator(t.Type))
		{
			TreeNode *node = new TreeNode(ntPreOperator);

			node->SetToken(&t);
			node->AddChild(parsePreOperator());

			return node;
		}

		moveTo(&t);

		return parseExpressionValue();
	}

	TreeNode *Parser::parseExpressionValue()
	{
		TreeNode *node = 0;

		Token t;
		getNextToken(&t);
		moveTo(&t);

		if (t.Type == ttOpenParanthesis)
		{
			getNextToken(&t);

			node = parseExpression();
			if (_syntaxError)
				return node;

			getNextToken(&t);
			if (t.Type != ttCloseParanthesis)
			{
				moveTo(&t);
				expectedTokenError(")", t.Position);
			}
		}
		else if (t.Type == ttOpenStatementBlock)
				node = parseInitializationList();
		else if (t.Type == ttIdentifier)
		{
			if (isFunctionCall())
				node = parseFunctionCall();
			else
				node = parseIdentifier();
		}
		else if (t.Type == ttSub)
		{
			node = parseFunctionDefinition(false);
		}
		else if (isConstant(t.Type))
			node = parseConstant();
		else
			unexpectedTokenError(&t);

		return node;
	}

	TreeNode *Parser::parseIfElse()
	{
		TreeNode *node = new TreeNode(ntIfElse);

		Token t;
		getNextToken(&t);

		if (t.Type != ttIf)
		{
			unexpectedTokenError(&t);
			return node;
		}

		getNextToken(&t);
		if (t.Type != ttOpenParanthesis)
		{
			unexpectedTokenError(&t);
			return node;
		}

		node->AddChild(parseAssignment());
		
		getNextToken(&t);
		if (t.Type != ttCloseParanthesis)
		{
			expectedTokenError(")", t.Position);
			return node;
		}

		node->AddChild(parsePostConditionalStatements());

		getNextToken(&t);
		if (t.Type != ttElse)
		{
			moveTo(&t);
			return node;
		}

		node->AddChild(parsePostConditionalStatements());
		return node;
	}

	TreeNode *Parser::parsePostConditionalStatements()
	{
		Token t;

		getNextToken(&t);
		moveTo(&t);

		if (t.Type == ttOpenStatementBlock)
			return parseStatementsBlock();
		
		return parseExpressionStatement();
	}

	TreeNode *Parser::parseInitializationList()
	{
		TreeNode *node = new TreeNode(ntArrayInitList);

		Token t;
		getNextToken(&t);

		if (t.Type != ttOpenStatementBlock)
		{
			unexpectedTokenError(&t);
			return node;
		}

		node->UpdateData(t.Position, t.Length, t.Row);

		getNextToken(&t);
		if (t.Type == ttCloseStatementBlock)
		{
			node->UpdateData(t.Position, t.Length, t.Row);
			return node;
		}

		moveTo(&t);
		node->AddChild(parseAssignment());
		if (_syntaxError)
			return node;

		getNextToken(&t);
		moveTo(&t);
		if (t.Type == ttRange)
		{
			getNextToken(&t);

			node->SetType(ntRange);
			node->AddChild(parseAssignment());
			if (_syntaxError)
				return node;

			getNextToken(&t);
			if (t.Type == ttCloseStatementBlock)
			{
				node->UpdateData(t.Position, t.Length, t.Row);
				return node;
			}

			expectedTokenError("}", t.Position);
		}

		for (;;)
		{
			getNextToken(&t);
			if (t.Type == ttComma)
				continue;
			else if (t.Type == ttCloseStatementBlock)
			{
				node->UpdateData(t.Position, t.Length, t.Row);
				return node;
			}
			else if (t.Type == ttOpenStatementBlock)
			{
				moveTo(&t);
				node->AddChild(parseInitializationList());
				if (_syntaxError)
					return node;

				getNextToken(&t);
				if(t.Type == ttComma)
					continue;
				else if (t.Type == ttCloseStatementBlock)
				{
					node->UpdateData(t.Position, t.Length, t.Row);
					return node;
				}
				else
				{
					expectedTokenError(", or }", t.Position);
					return node;
				}
			}
			else
			{
				moveTo(&t);
				node->AddChild(parseAssignment());
				if (_syntaxError)
					return node;

				getNextToken(&t);
				if (t.Type == ttComma)
					continue;
				else if (t.Type == ttCloseStatementBlock)
				{
					node->UpdateData(t.Position, t.Length, t.Row);
					return node;
				}
				else
				{
					expectedTokenError(", or }", t.Position);
					return node;
				}
			}
		}

		return node;
	}

	TreeNode *Parser::parseWhile()
	{
		TreeNode *node = new TreeNode(ntWhile);

		Token t;
		getNextToken(&t);
		if (t.Type != ttWhile)
		{
			unexpectedTokenError(&t);
			return node;
		}

		getNextToken(&t);
		if (t.Type != ttOpenParanthesis)
		{
			expectedTokenError("(", t.Position);
			return node;
		}

		node->AddChild(parseAssignment());

		getNextToken(&t);
		if (t.Type != ttCloseParanthesis)
		{
			expectedTokenError(")", t.Position);
			return node;
		}

		node->AddChild(parsePostConditionalStatements());

		return node;
	}

	TreeNode *Parser::parseForEach()
	{
		TreeNode *node = new TreeNode(ntForEach);

		Token t;
		getNextToken(&t);
		if (t.Type != ttForEach)
		{
			unexpectedTokenError(&t);
			return node;
		}

		getNextToken(&t);
		if (t.Type != ttOpenParanthesis)
		{
			expectedTokenError("(", t.Position);
			return node;
		}

		node->AddChild(parseVarDeclaration(true, false));

		getNextToken(&t);
		if (t.Type != ttIn)
		{
			expectedTokenError("in", t.Position);
			return node;
		}

		node->AddChild(parseAssignment());

		getNextToken(&t);
		if (t.Type != ttCloseParanthesis)
		{
			expectedTokenError(")", t.Position);
			return node;
		}

		node->AddChild(parsePostConditionalStatements());

		return node;
	}

	TreeNode *Parser::parseImport()
	{
		TreeNode *node = new TreeNode(ntImport);

		Token t;
		getNextToken(&t);
		if (t.Type != ttImport)
		{
			expectedTokenError("import", t.Position);
			return node;
		}

		node->AddChild(parseIdentifier());
		match(ttEndOfStatement);

		return node;
	}

	const char *GetTreeNodeDefinition(NodeType nodeType)
	{
		switch (nodeType)
		{
		case ntSub:
			return "Sub";

		case ntCode:
			return "Code";

		case ntIdentifier:
			return "Identifier";

		case ntAssignment:
			return "Assignment";

		case ntFunctionCallArgList:
			return "FunctionCallArgList";

		case ntFunctionDefinitionArgList:
			return "FunctionDefinitionArgList";

		case ntStatementsBlock:
			return "StatementBlock";

		case ntVarDeclaration:
			return "VarDeclaration";

		case ntExpression:
			return "Expression";

		case ntValue:
			return "Value";

		case ntPreOperator:
			return "PreOperator";

		case ntPostOperator:
			return "PostOperator";

		case ntExpressionValue:
			return "ExpressionValue";

		case ntFunctionCall:
			return "FunctionCall";

		case ntConstant:
			return "Constant";

		case ntExpressionOperator:
			return "ExpressionOperator";

		case ntBinaryOperator:
			return "BinaryOperator";

		case ntReturn:
			return "Return";

		case ntIfElse:
			return "If-Else";

		case ntIndexing:
			return "Index";
		}

		return "<???>";
	}
}