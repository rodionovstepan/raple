#ifndef __PARSER_H_INCLUDED__
#define __PARSER_H_INCLUDED__


//-------------------------------------
#include "ParserBase.h"
//-------------------------------------


/*

	Grammar.
	
	DATATYPE      = 'var'
	FUNCTION      = 'sub' IDENTIFIER ARGLIST STATBLOCK
	ARGLIST       = '(' ((IDENTIFIER)? (, IDENTIFIER)*)? ')'
	STATBLOCK     = '{' (DECLARATION | STATEMENT)* '}'
	STATEMENT     = RETURN | ASSIGNMENT | DECLARATION | CONDITIONEXPR
	RETURN        = 'return' (EXPRESSION)? ';'
	DECLARATION   = DATATYPE IDENTIFIER '=' ASSIGNMENT (',' IDENTIFIER '=' ASSIGNMENT)* ';'
	ASSIGNMENT    = EXPRESSION (ASSIGNOPER EXPRESSION)*
	EXPRESSION    = VALUE (BINOPERATOR VALUE)*
	VALUE         = '(' ASSIGNMENT ')' | ( (PREOP)? IDENTIFIER | FUNCTIONCALL | CONSTANT )
	FUNCTIONCALL  = IDENTIFIER PARAMLIST
	PARAMLIST     = '(' ((ASSIGNMENT)? (, ASSIGNMENT)*)? ')'
	BINOPERATOR   = '+' | '-' | '*' | '/' | '**'
	PREOP		  = '-'
	ASSIGNOPER    = '=' | '<-'
	EQUALOPER     = '==' | '!=' | '<=>' | '<!>'
	ANDOR         = '&&' | '||' | 'and' | 'or'
	IFELSEEXPR    = 'if' '(' ASSIGNMENT ')' IFSTATEMENT ('else' IFSTATEMENT)?
	IFSTATEMENT   = STATBLOCK | STATEMENT
	WHILE         = 'while' '(' ASSIGNMENT ')' IFSTATEMENT
	IMPORT        = 'import' IDENTIFIER ';'
	FOREACH       = 'foreach' '(' IDENTIFIER 'in' ASSIGNMENT ')' IFSTATEMENT
*/

namespace Raple
{
	typedef TreeNode* (*ParsingFunction)();
	typedef bool (*TokenTypeSelectionFunction)(TokenType);

	class Parser : public ParserBase
	{
	private:

		//!
		const SourceCode *_sourceCode;

		//! 
		int _position;
		bool _syntaxError;

	public:

		Parser(ITokenizer *const tokenizer, const ILogger *logger);
		virtual ~Parser();

		virtual TreeNode *ParseScript(const SourceCode *sourceCode);

	private:

		TreeNode *parseWhile();
		TreeNode *parseForEach();
		TreeNode *parseFunctionDefinition(bool nameRequired = true);
		TreeNode *parseFunctionCall();
		TreeNode *parseIdentifier();
		TreeNode *parseArgList(bool isFunctionCall);
		TreeNode *parseConstant();
		TreeNode *parseStatementsBlock();
		TreeNode *parseAssignment();
		TreeNode *parseVarDeclaration(bool needVarKeyword = true, bool needAssignment = true);
		TreeNode *parseExpression();
		TreeNode *parseExpressionTerm();
		TreeNode *parseExpressionValue();
		TreeNode *parseExpressionStatement();
		TreeNode *parsePreOperator();
		TreeNode *parseBinaryOperator();
		TreeNode *parseHighPriorOperator();
		TreeNode *parseLowPriorOperator();
		TreeNode *parseComparisonOperator();
		TreeNode *parseReturnExpression();
		TreeNode *parseIfElse();
		TreeNode *parsePostConditionalStatements();
		TreeNode *parseInitializationList();
		TreeNode *parseImport();
		TreeNode *parseScript();

		TreeNode *parseExpressionDeeper(ParsingFunction parsingFunction, TokenTypeSelectionFunction isCorrectToken);
		

		void getNextToken(Token *token);
		void moveTo(const Token *token);
		void unexpectedTokenError(const Token *token);
		void expectedTokenError(const char *tokenText, int position);
		void match(TokenType type);

		bool isConstant(TokenType tokenType);
		bool isPreOperator(TokenType tokenType);
		bool isPostOperator(TokenType tokenType);
		bool isComparisonOperator(TokenType tokenType);
		bool isLogicalOperator(TokenType tokenType);
		bool isAssignOperator(TokenType tokenType);
		bool isFunctionCall();
	};
}


#endif //__PARSER_H_INCLUDED__