//-------------------------------------
#include "../Headers/System.h"
#include "../Headers/Compiler.h"
#include "../Headers/Constants.h"
//-------------------------------------


namespace Raple
{
	Compiler::~Compiler()
	{
	}

	CompileResult Compiler::CompileTree(TreeNode *root, SourceCode *sourceCode, IVirtualMachine *vm)
	{
		CompileResult validationResult = inputValidation(root, sourceCode, vm);
		
		if (validationResult != crSuccess)
			return validationResult;
		
		_environment = vm->GetEnvironment();
		_virtualMachine = vm;
		_sourceCode = sourceCode;
		_currentSub = 0;

		int childCount = root->ChildCount();
		CompileResult result = crSuccess;

		for (int i = 0; i < childCount; ++i)
		{
			result = compileNode(root->GetChild(i));

			if (result != crSuccess)
				return result;
		}

		return result;
	}

	CompileResult Compiler::inputValidation(TreeNode *root, SourceCode *sourceCode, IVirtualMachine *vm) const
	{
		if (root == 0 || root->GetType() != ntCode)
		{
			_logger->Error(Constants::LogTitleValidationError, "Syntax tree isn't correct!");
			return crNullTree;
		}

		if (sourceCode == 0)
		{
			_logger->Error(Constants::LogTitleValidationError, "Source code is empty!");
			return crNullSourceCode;
		}

		if (vm == 0)
		{
			_logger->Error(Constants::LogTitleValidationError, "Raple virtual machine isn't correct!");
			return crNullVm;
		}

		if (vm->GetEnvironment() == 0)
		{
			_logger->Error(Constants::LogTitleValidationError, "Raple runtime environment isn't correct!");
			return crNullEnvironment;
		}

		return crSuccess;
	}

	CompileResult Compiler::compileNode(TreeNode *node)
	{
		if (node == 0)
			return crNullNode;

		switch (node->GetType())
		{
		case ntSub:
			return compileSub(node);

		case ntImport:
			return compileImport(node);

		default:
			_logger->Error(Constants::LogTitleCompilerError, "Unknown global node type " + rstring(GetTreeNodeDefinition(node->GetType())));
			return crUnknownNode;
		}
	}

	CompileResult Compiler::compileImport(TreeNode *importNode)
	{
		rstring libname = extractStringFromNode(importNode->GetChild(Constants::TreeIndexImportLibName));

		if (!_environment->IsLibraryExists(libname))
		{
			_logger->Error(Constants::LogTitleCompilerError, "Unknown library import");
			return crFailed;
		}

		_environment->ImportLibrary(libname);
		return crSuccess;
	}

	CompileResult Compiler::compileSub(TreeNode *subNode)
	{
		rstring subName = extractStringFromNode(subNode->GetChild(Constants::TreeIndexSubName));

		int subIndex = _environment->AddSub(subName);
		_currentSub = _environment->GetSub(subIndex);

		TreeNode *argsNode = subNode->GetChild(Constants::TreeIndexSubArguments);
		
		int argumentCount = argsNode->ChildCount();
		_currentSub->SetArgumentsCount(argumentCount);

		for (int i = 0; i < argumentCount; ++i)
			_currentSub->AddNewLocal(extractStringFromNode(argsNode->GetChild(i)), dtCallArgument);

		CompileResult result = compileSubStatements(subNode->GetChild(Constants::TreeIndexSubBody));

		if (result != crSuccess)
			return result;

		addCodeInstruction(opRet);
		return crSuccess;
	}

	CompileResult Compiler::compileAnonymousSub(TreeNode *anonymousSubNode)
	{
		int subid = _environment->AddAnonymousSub();
		Sub *sub = _environment->GetSub(subid);

		TreeNode *argsNode = anonymousSubNode->GetChild(Constants::TreeIndexAnonymousSubArguments);
		
		int argumentCount = argsNode->ChildCount();
		sub->SetArgumentsCount(argumentCount);

		for (int i = 0; i < argumentCount; ++i)
			sub->AddNewLocal(extractStringFromNode(argsNode->GetChild(i)), dtCallArgument);

		Sub *ownerSub = _currentSub;
		_currentSub = sub;

		CompileResult result = compileSubStatements(anonymousSubNode->GetChild(Constants::TreeIndexAnonymousSubBody));
		_currentSub = ownerSub;

		if (result != crSuccess)
			return result;

		sub->GetBytecode()->AddInstruction(opRet);

		addCodeInstruction(opGetSub, subid);

		return crSuccess;
	}

	CompileResult Compiler::compileSubStatements(TreeNode *subNode)
	{
		TreeNode *item = subNode->GetChild(0);

		while (item != 0)
		{
			CompileResult result = crSuccess;

			switch (item->GetType())
			{
			case ntVarDeclaration:
				result = compileVarDeclaration(item);
				break;

			case ntFunctionCall:
				result = compileFunctionCall(item, 0);
				break;

			default:
				result = compileExpression(item);
			}

			if (result != crSuccess)
				return result;

			item = item->Next;
		}

		return crSuccess;
	}

	CompileResult Compiler::compileReturn(TreeNode *returnNode)
	{
		CompileResult result = crSuccess;

		int returnExpessions = returnNode->ChildCount();
		if (returnExpessions > 0)
			result = compileExpression(returnNode->GetChild(0));

		addCodeInstruction(opRet, returnExpessions);

		return result;
	}

	CompileResult Compiler::compileExpression(TreeNode *expressionNode, bool expectReturnValue)
	{
		if (expressionNode == 0)
			return crNullNode;

		TreeNode *item = expressionNode;
		CompileResult result = crSuccess;

		switch (item->GetType())
		{
		case ntBinaryOperator:
			result = compileBinaryOperator(item);
			break;

		case ntPreOperator:
			result = compileUnaryOperator(item);
			break;

		case ntPostOperator:
			result = compilePostOperator(item);
			break;

		case ntConstant:
			result = compileConstant(item);
			break;

		case ntIdentifier:
			result = compileLocalAccess(item);
			break;

		case ntFunctionCall:
			result = compileFunctionCall(item, expectReturnValue);
			break;

		case ntAssignment:
			result = compileAssignment(item);
			break;

		case ntIfElse:
			result = compileIf(item);
			break;

		case ntWhile:
			result = compileWhile(item);
			break;

		case ntReturn:
			result = compileReturn(item);
			break;

		case ntIndexing:
			result = compileArrayIndexing(item);
			break;
			
		case ntArrayInitList:
			result = compileAnonymousArrayInitList(item);
			break;

		case ntAnonymousSub:
			result = compileAnonymousSub(item);
			break;

		default:
			_logger->Error(Constants::LogTitleCompilerError, "Unknown expression node type " + rstring(GetTreeNodeDefinition(item->GetType())));
			return crUnknownNode;
		}

		return result;
	}

	CompileResult Compiler::compileConstant(TreeNode *constantNode)
	{
		switch (constantNode->GetToken()->Type)
		{
		case ttDecConst:
			addCodePushIntConstant(constantNode);
			break;

		case ttFloatConst:
			addCodePushFloatConstant(constantNode);
			break;

		case ttString:
			addCodePushStringConstant(constantNode);
			break;

		default:
			_logger->Error(Constants::LogTitleCompilerError, "Unknown constant token " + 
								rstring(GetTokenDefinition(constantNode->GetToken()->Type)));
			return crUnknownNode;
		}

		return crSuccess;
	}
	
	CompileResult Compiler::compileBinaryOperator(TreeNode *binaryOperatorNode)
	{
		RapleAssert(binaryOperatorNode->ChildCount() >= 2);
		RapleAssert(binaryOperatorNode->GetChild(0) != 0);
		RapleAssert(binaryOperatorNode->GetChild(1) != 0);

		TokenType tt = binaryOperatorNode->GetToken()->Type;
		if (tt == ttAnd || tt == ttOr)
			return compileLogicalOperator(binaryOperatorNode);

		CompileResult result = compileExpression(binaryOperatorNode->GetChild(0));
		if (result != crSuccess)
			return result;

		result = compileExpression(binaryOperatorNode->GetChild(1));
		if (result != crSuccess)
			return result;

		switch (tt)
		{
		case ttPlus:
			addCodeInstruction(opAdd);
			break;

		case ttStar:
			addCodeInstruction(opProd);
			break;

		case ttMinus:
			addCodeInstruction(opSub);
			break;

		case ttSlash:
			addCodeInstruction(opDiv);
			break;

		case ttExponentiation:
			addCodeInstruction(opExp);
			break;

		case ttEqual:
			addCodeInstruction(opEqual);
			break;

		case ttNotEqual:
			addCodeInstruction(opNotEqual);
			break;

		case ttGreater:
			addCodeInstruction(opGreater);
			break;

		case ttLower:
			addCodeInstruction(opLess);
			break;

		case ttGreaterEqual:
			addCodeInstruction(opGreaterOrEqual);
			break;

		case ttLowerEqual:
			addCodeInstruction(opLessOrEqual);
			break;

		default:
			_logger->Error(Constants::LogTitleCompilerError, "Unknown binary operation token " + 
								rstring(GetTokenDefinition(binaryOperatorNode->GetToken()->Type)));
			return crUnknownToken;
		}

		return crSuccess;
	}

	CompileResult Compiler::compileLogicalOperator(TreeNode *logicalOperatorNode)
	{
		RapleAssert(logicalOperatorNode->ChildCount() == 2);

		CompileResult result = compileExpression(logicalOperatorNode->GetChild(0));
		if (result != crSuccess)
			return result;

		unsigned int gotoInstrIndex = _currentSub->GetBytecode()->InstructionCount();

		switch (logicalOperatorNode->GetToken()->Type)
		{
		case ttAnd:
			addCodeInstruction(opJumpZero);
			break;
		case ttOr:
			addCodeInstruction(opJumpNotZero);
			break;

		default:
			return crUnknownToken;
		}

		result = compileExpression(logicalOperatorNode->GetChild(1));
		if (result != crSuccess)
			return result;

		setGotoIndexToInstruction(gotoInstrIndex);
		return crSuccess;
	}
	
	CompileResult Compiler::compileUnaryOperator(TreeNode *unaryOperatorNode)
	{
		RapleAssert(unaryOperatorNode->ChildCount() == 1);
		
		TreeNode *node = unaryOperatorNode->GetChild(0);
		if (node->GetType() == ntConstant)
		{
			node->GetToken()->Position -= 1;
			node->GetToken()->Length += 1;

			return compileConstant(node);
		}

		CompileResult result = compileExpression(node);
		if (result != crSuccess)
			return result;

		switch (unaryOperatorNode->GetToken()->Type)
		{
		case ttMinus:
			addCodeInstruction(opNeg);
			break;

		default:
			_logger->Error(Constants::LogTitleCompilerError, "Unknown unary operation token " + 
								rstring(GetTokenDefinition(unaryOperatorNode->GetToken()->Type)));
			return crUnknownToken;
		}

		return crSuccess;
	}

	CompileResult Compiler::compilePostOperator(TreeNode *postOperatorNode)
	{
		RapleAssert(postOperatorNode->ChildCount() == 1);

		TreeNode *node = postOperatorNode->GetChild(0);
		if (node->GetType() == ntConstant)
		{
			_logger->Error(Constants::LogTitleCompilerError, 
						   "Post operator" + rstring(GetTokenDefinition(postOperatorNode->GetToken()->Type)) + 
						   " for constant!");

			return crFailed;
		}

		CompileResult result = compileExpression(node);
		if (result != crSuccess)
			return result;

		switch (postOperatorNode->GetToken()->Type)
		{
		case ttIncrement:
			addCodeInstruction(opInc);
			break;
			
		case ttDecrement:
			addCodeInstruction(opDec);

		default:
			_logger->Error(Constants::LogTitleCompilerError, 
						   "Unknown post operator token " + 
								rstring(GetTokenDefinition(postOperatorNode->GetToken()->Type)));
			return crUnknownToken;
		}

		return crSuccess;
	}

	CompileResult Compiler::compileFunctionCall(TreeNode *callNode, bool expectReturnValue)
	{
		bool isGlobalCall = callNode->GetChild(Constants::TreeIndexFunctionCallArguments)->GetType() == ntFunctionCallArgList;
		TreeNode *typeNode = callNode->GetChild(Constants::TreeIndexFunctionCallName);
		rstring identifier = extractStringFromNode(typeNode);
		
		if (isGlobalCall)	// global call; e.g., println(foo);
			return compileGlobalFunctionCall(callNode, identifier, expectReturnValue);

		else if (typeNode->GetType() == ntIdentifier)	// internal function call; e.g., foo.bar(); or math.min(,);
		{
			int id = _currentSub->FindLocal(identifier);
			
			if (id != Constants::SubUnknownLocalId)
				return compileInternalFunctionCall(callNode->GetChild(Constants::TreeIndexFunctionCallInternalCall), id, expectReturnValue);
			
			if (_environment->IsLibraryImported(identifier))
				return compileStaticLibraryFunctionCall(callNode->GetChild(Constants::TreeIndexFunctionCallInternalCall), identifier.Hash(), expectReturnValue);

			_logger->Error(Constants::LogTitleCompilerError, "Unknown local var or static library identifier");
			return crFailed;
		}

		// double-call; e.g., foo().bar(); or {1,2,3}.size();
		return compileDoubleFunctionCall(typeNode, callNode->GetChild(Constants::TreeIndexFunctionCallInternalCall), expectReturnValue);
	}

	CompileResult Compiler::compileDoubleFunctionCall(TreeNode *firstCall, TreeNode *secondCall, bool expectReturnValue)
	{
		CompileResult result = compileExpression(firstCall, true);
		if (result != crSuccess)
			return result;

		unsigned int internalHash = getHashFromNode(secondCall->GetChild(Constants::TreeIndexFunctionCallName));

		compileFunctionCallArgumentList(secondCall->GetChild(Constants::TreeIndexFunctionCallArguments), 0, false);

		addCodeInstructionWithInt(opInternalCall, expectReturnValue, internalHash);

		return crSuccess;
	}

	CompileResult Compiler::compileGlobalFunctionCall(TreeNode *node, const rstring &identifier, bool expectReturnValue)
	{
		int id = _virtualMachine->GetEnvironment()->FindSub(identifier);
		if (id == Constants::EnvironmentUnknownSubId)
		{
			id = _currentSub->FindLocal(identifier);
			if (id == Constants::SubUnknownLocalId)
			{
				_logger->Error(Constants::LogTitleCompilerError, "Unknown sub identifier");
				return crFailed;
			}
			
			// try to execute dynamic call
			TreeNode *argsNode = node->GetChild(Constants::TreeIndexFunctionCallArguments);

			CompileResult result = compileFunctionCallArgumentList(argsNode, 0, false);
			if (result != crSuccess)
				return result;

			addCodeInstruction(opGetLocal, id);
			addCodeInstructionWithInt(opDynamicCall, expectReturnValue);
			return crSuccess;
		}

		unsigned int subArgumentCount = _virtualMachine->GetEnvironment()->GetSub(id)->GetArgumentsCount();
		CompileResult result = compileFunctionCallArgumentList(node->GetChild(Constants::TreeIndexFunctionCallArguments), subArgumentCount);
		if (result != crSuccess)
			return result;

		addCodeInstructionWithInt(opCall, expectReturnValue, id);

		return crSuccess;
	}

	CompileResult Compiler::compileInternalFunctionCall(TreeNode *node, rhash localId, bool expectReturnValue)
	{
		unsigned int internalHash = getHashFromNode(node->GetChild(Constants::TreeIndexFunctionCallName));
		CompileResult result = compileFunctionCallArgumentList(node->GetChild(Constants::TreeIndexFunctionCallArguments), 0, false);
		if (result != crSuccess)
			return result;
		
		addCodeInstruction(opGetLocal, localId);
		addCodeInstructionWithInt(opInternalCall, expectReturnValue, internalHash);

		return crSuccess;
	}

	CompileResult Compiler::compileStaticLibraryFunctionCall(TreeNode *node, rhash libraryId, bool expectReturnValue)
	{
		unsigned int staticLibSubHash = getHashFromNode(node->GetChild(Constants::TreeIndexFunctionCallName));
		CompileResult result = compileFunctionCallArgumentList(node->GetChild(Constants::TreeIndexFunctionCallArguments), 0, false);
		if (result != crSuccess)
			return result;

		addCodeInstructionWithInt(opPushInt, libraryId);
		addCodeInstructionWithInt(opStaticCall, expectReturnValue, staticLibSubHash);

		return crSuccess;
	}

	CompileResult Compiler::compileFunctionCallArgumentList(TreeNode *argumentList, unsigned int subArgumentCount, bool needToCompare)
	{
		int argumentCount = argumentList->ChildCount();
		if (needToCompare && argumentCount != subArgumentCount)
		{
			_logger->Error(Constants::LogTitleCompilerError, "Number of arguments doesn't match");
			return crFailed;
		}

		for (int i = 0; i < argumentCount; ++i)
		{
			CompileResult result = compileFunctionCallArgument(argumentList->GetChild(i));

			if (result != crSuccess)
				return result;
		}

		return crSuccess;
	}

	CompileResult Compiler::compileFunctionCallArgument(TreeNode *argumentNode)
	{
		CompileResult result = compileExpression(argumentNode);

		if (result != crSuccess)
			return result;

		//! бля, не нравится мне такое решение!
		//! приходится два раза доставать строку по данным из токена
		if (argumentNode->GetType() == ntAssignment)
		{
			int id = getLocalIdentificatorByToken(argumentNode->GetChild(0)->GetToken());
			if (id == Constants::SubUnknownLocalId)
			{
				_logger->Error(Constants::LogTitleCompilerError, "Undeclared identifier");
				return crFailed;
			}

			addCodeInstruction(opGetLocal, id);
		}

		return crSuccess;
	}

	CompileResult Compiler::compileVarDeclaration(TreeNode *varDeclarationNode)
	{
		TreeNode *vardecNode = varDeclarationNode;
		CompileResult result = crSuccess;

		if (vardecNode->ChildCount() < 2)
		{
			_logger->Error(Constants::LogTitleCompilerError, "Var declaration without assignment");
			return crFailed;
		}

		TreeNode *valueNode = vardecNode->GetChild(Constants::TreeIndexVarDeclarationValue);
		bool isArrayDeclaration = valueNode->GetType() == ntArrayInitList;

		Token *identifierToken = vardecNode->GetChild(Constants::TreeIndexVarDeclarationIdentifier)->GetToken();
		rstring identifier = _sourceCode->TakePart(identifierToken->Position, identifierToken->Length);

		unsigned int id = _currentSub->AddNewLocal(identifier);
		
		if (isArrayDeclaration)
			result = compileArrayInitializationList(valueNode, id);
		else
			result = compileExpression(valueNode);

		if (result != crSuccess)
			return result;

		if (!isArrayDeclaration)
			addCodeInstruction(opSetLocal, id);

		// это означается множественное определение переменной
		// например, var a=2,b=3;
		// в третьем ноде будет храниться определение b=3.
		if (vardecNode->ChildCount() == 3)
			return compileVarDeclaration(vardecNode->GetChild(Constants::TreeIndexVarDeclarationAdditionalNode));

		return result;
	}

	CompileResult Compiler::compileLocalAccess(TreeNode *node)
	{
		int id = getLocalIdentificatorByToken(node->GetToken());
		if (id == Constants::SubUnknownLocalId)
		{
			_logger->Error(Constants::LogTitleCompilerError, "Undeclared identifier");
			return crFailed;
		}

		addCodeInstruction(opGetLocal, id);

		return crSuccess;
	}

	CompileResult Compiler::compileAssignment(TreeNode *assignmentNode)
	{
		RapleAssert(assignmentNode->ChildCount() == 2);

		TreeNode *assignee = assignmentNode->GetChild(Constants::TreeIndexAssignmentAssignee);
		if (assignee->GetType() == ntIndexing)
			return compileArrayItemAssignment(assignmentNode);

		CompileResult result = compileExpression(assignmentNode->GetChild(Constants::TreeIndexAssignmentValue));
		if (result != crSuccess)
			return result;

		int id = getLocalIdentificatorByToken(assignee->GetToken());
		if (id == Constants::SubUnknownLocalId)
		{
			_logger->Error(Constants::LogTitleCompilerError, "Undeclared identifier");
			return crFailed;
		}

		addCodeInstruction(opSetLocal, id);

		return crSuccess;
	}

	CompileResult Compiler::compileArrayItemAssignment(TreeNode *assignmentNode)
	{
		RapleAssert(assignmentNode->ChildCount() == 2);
		TreeNode *assignee = assignmentNode->GetChild(Constants::TreeIndexAssignmentAssignee);
		RapleAssert(assignee != 0);

		CompileResult result = compileExpression(assignmentNode->GetChild(Constants::TreeIndexAssignmentValue));
		if (result != crSuccess)
			return result;

		result = compileExpression(assignee->GetChild(Constants::TreeIndexArrayIndexingKey));
		if (result != crSuccess)
			return result;

		TreeNode *identifier = assignee->GetChild(Constants::TreeIndexArrayIndexingArray);
		int id = getLocalIdentificatorByToken(identifier->GetToken());
		if (id == Constants::SubUnknownLocalId)
		{
			_logger->Error(Constants::LogTitleCompilerError, "Undeclared identifier");
			return crFailed;
		}

		addCodeInstruction(opGetLocal, id);
		addCodeInstruction(opSetElement);

		return crSuccess;
	}

	CompileResult Compiler::compileIf(TreeNode *ifNode)
	{
		RapleAssert(ifNode->ChildCount() >= 2);

		CompileResult result = compileExpression(ifNode->GetChild(Constants::TreeIndexIfElseCondition));
		if (result != crSuccess)
			return result;

		unsigned int gotoInstrIndex = _currentSub->GetBytecode()->InstructionCount();
		addCodeInstruction(opJumpZero);

		result = compilePostConditionalStatements(ifNode->GetChild(Constants::TreeIndexIfElseIfStatements));
		if (result != crSuccess)
			return result;

		setGotoIndexToInstruction(gotoInstrIndex);

		if (ifNode->ChildCount() == 2)
		{
			setGotoIndexToInstruction(gotoInstrIndex);
			return crSuccess;
		}

		unsigned int jumpToElseInstrIndex = _currentSub->GetBytecode()->InstructionCount();

		addCodeInstruction(opGoto);
		setGotoIndexToInstruction(gotoInstrIndex);

		result = compilePostConditionalStatements(ifNode->GetChild(Constants::TreeIndexIfElseElseStatements));
		if (result != crSuccess)
			return result;

		setGotoIndexToInstruction(jumpToElseInstrIndex);

		return crSuccess;
	}

	CompileResult Compiler::compileWhile(TreeNode *whileNode)
	{
		RapleAssert(whileNode->ChildCount() == 2);

		unsigned int conditionInstrIndex = _currentSub->GetBytecode()->InstructionCount();

		CompileResult result = compileExpression(whileNode->GetChild(Constants::TreeIndexWhileCondition));
		if (result != crSuccess)
			return result;

		unsigned int gotoInstrIndex = _currentSub->GetBytecode()->InstructionCount();
		addCodeInstruction(opJumpZero);

		result = compilePostConditionalStatements(whileNode->GetChild(Constants::TreeIndexWhileStatements));
		if (result != crSuccess)
			return result;

		addCodeInstruction(opGoto, conditionInstrIndex);
		setGotoIndexToInstruction(gotoInstrIndex);
		
		return crSuccess;
	}

	CompileResult Compiler::compileArrayIndexing(TreeNode *indexingNode)
	{
		RapleAssert(indexingNode->ChildCount() == 2);

		CompileResult result = compileExpression(indexingNode->GetChild(Constants::TreeIndexArrayIndexingKey));
		if (result != crSuccess)
			return result;

		TreeNode *subject = indexingNode->GetChild(Constants::TreeIndexArrayIndexingArray);
		if (subject->GetType() == ntIdentifier)
		{
			int id = getLocalIdentificatorByToken(subject->GetToken());
			if (id == Constants::SubUnknownLocalId)
			{
				_logger->Error(Constants::LogTitleCompilerError, "Undeclared identifier");
				return crFailed;
			}

			addCodeInstruction(opGetLocal, id);
		}
		else if (subject->GetType() == ntFunctionCall)
		{
			result = compileFunctionCall(subject, true);
			if (result != crSuccess)
				return result;
		}
		else if (subject->GetType() == ntIndexing)
		{
			result = compileArrayIndexing(subject);
			if (result != crSuccess)
				return result;
		}

		addCodeInstruction(opGetElement);
		return crSuccess;
	}

	CompileResult Compiler::compileArrayInitializationList(TreeNode *initlistNode, int arrayId)
	{
		addCodeInstruction(opNewArray, arrayId);

		CompileResult result;
		const int listCount = initlistNode->ChildCount();

		for (int i = 0; i < listCount; ++i)
		{
			result = compileExpression(initlistNode->GetChild(i));
			if (result != crSuccess)
				return result;

			// при инициализации массива ключом доступа к нему
			// является его номер в списке инициализации
			addCodeInstructionWithInt(opPushInt, i);
			addCodeInstruction(opGetLocal, arrayId);
			addCodeInstruction(opSetElement);
		}

		return crSuccess;
	}

	CompileResult Compiler::compileAnonymousArrayInitList(TreeNode *anonInitlist)
	{
		unsigned int id = _currentSub->AddNewAnonymousLocal();
		CompileResult result = compileArrayInitializationList(anonInitlist, id);
		if (result != crSuccess)
			return result;

		addCodeInstruction(opGetLocal, id);
		return crSuccess;
	}

	void Compiler::setGotoIndexToInstruction(unsigned int instructionIndex)
	{
		_currentSub
			->GetBytecode()
			->GetInstruction(instructionIndex)
			->Argument(_currentSub->GetBytecode()->InstructionCount());
	}
	
	CompileResult Compiler::compilePostConditionalStatements(TreeNode *statementsNode)
	{
		if (statementsNode->GetType() == ntStatementsBlock)
			return compileSubStatements(statementsNode);

		return compileExpression(statementsNode);
	}

	void Compiler::addCodePushIntConstant(TreeNode *node)
	{
		addCodeInstructionWithInt(opPushInt, extractStringFromNode(node).ToInt());
	}

	void Compiler::addCodePushFloatConstant(TreeNode *node)
	{
		addCodeInstructionWithFloat(opPushFloat, extractStringFromNode(node).ToFloat());
	}

	rstring Compiler::extractStringFromNode(TreeNode *node)
	{
		return _sourceCode->TakePart(node->GetToken()->Position, node->GetToken()->Length);
	}

	void Compiler::addCodePushStringConstant(TreeNode *node)
	{
		Token *token = node->GetToken();		
		addCodeInstructionWithString(opPushString, new rstring(_sourceCode->TakePart(token->Position, token->Length)));
	}

	void Compiler::addCodeInstruction(Opcode opcode, unsigned int arg)
	{
		_currentSub->GetBytecode()->AddInstruction(new Instruction(opcode, arg));
	}

	void Compiler::addCodeInstructionWithInt(Opcode opcode, int intValue, unsigned int arg)
	{
		Instruction *instruction = new Instruction(opcode, arg);
		instruction->Int(intValue);
		_currentSub->GetBytecode()->AddInstruction(instruction);
	}

	void Compiler::addCodeInstructionWithFloat(Opcode opcode, float floatValue, unsigned int arg)
	{
		Instruction *instruction = new Instruction(opcode, arg);
		instruction->Float(floatValue);
		_currentSub->GetBytecode()->AddInstruction(instruction);
	}

	void Compiler::addCodeInstructionWithString(Opcode opcode, rstring *string, unsigned int arg)
	{
		Instruction *instruction = new Instruction(opcode, arg);
		instruction->String(string);
		_currentSub->GetBytecode()->AddInstruction(instruction);
	}

	int Compiler::getLocalIdentificatorByToken(Token *token)
	{
		return _currentSub->FindLocal(_sourceCode->TakePart(token->Position, token->Length));
	}

	unsigned int Compiler::getHashFromNode(TreeNode *node)
	{
		return extractStringFromNode(node).Hash();
	}
}
