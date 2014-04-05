#ifndef __COMPILER_H_INCLUDED__
#define __COMPILER_H_INCLUDED__


//-------------------------------------
#include "ICompiler.h"
#include "ILogger.h"
//-------------------------------------


namespace Raple
{
	class Compiler : public ICompiler
	{
	public:

		Compiler(const ILogger *logger)
		{
			_logger = logger;
		}

		virtual ~Compiler();

		virtual CompileResult CompileTree(TreeNode *root,
										  SourceCode *sourceCode,
										  IVirtualMachine *vm);

	private:

		CompileResult compileNode(TreeNode *node);
		CompileResult compileSub(TreeNode *subNode);
		CompileResult compileSubStatements(TreeNode *subNode);
		CompileResult compileReturn(TreeNode *returnNode);
		
		CompileResult compileExpression(TreeNode *expressionNode, bool expectReturnValue = true);
		
		CompileResult compileFunctionCall(TreeNode *callNode, bool expectReturnValue);
		CompileResult compileFunctionCallArgument(TreeNode *argumentNode);
		CompileResult compileFunctionCallArgumentList(TreeNode *argumentList, unsigned int subArgumentCount, bool needToCompare = true);
		CompileResult compileInternalFunctionCall(TreeNode *node, rhash localId, bool expectReturnValue);
		CompileResult compileStaticLibraryFunctionCall(TreeNode *node, rhash libraryId, bool expectReturnValue);
		CompileResult compileGlobalFunctionCall(TreeNode *node, const rstring &identifier, bool expectReturnValue);
		CompileResult compileDoubleFunctionCall(TreeNode *firstCall, TreeNode *secondCall, bool expectReturnValue);
		
		CompileResult compileAnonymousSub(TreeNode *anonymousSubNode);
		CompileResult compileVarDeclaration(TreeNode *varDeclarationNode);
		CompileResult compileLocalAccess(TreeNode *node);
		CompileResult compileAssignment(TreeNode *assignmentNode);
		CompileResult compileBinaryOperator(TreeNode *binaryOperatorNode);
		CompileResult compileUnaryOperator(TreeNode *unaryOperatorNode);
		CompileResult compilePostOperator(TreeNode *postOperatorNode);
		CompileResult compileLogicalOperator(TreeNode *logicalOperatorNode);
		CompileResult compileConstant(TreeNode *constantNode);
		CompileResult compileIf(TreeNode *ifNode);
		CompileResult compilePostConditionalStatements(TreeNode *statementsNode);
		CompileResult compileArrayIndexing(TreeNode *indexingNode);
		CompileResult compileArrayInitializationList(TreeNode *initlistNode, int arrayId);
		CompileResult compileAnonymousArrayInitList(TreeNode *anonInitlist);
		CompileResult compileArrayItemAssignment(TreeNode *assignmentNode);
		CompileResult compileWhile(TreeNode *whileNode);

		CompileResult compileImport(TreeNode *importNode);
		
		CompileResult inputValidation(TreeNode *root, SourceCode *sourceCode, IVirtualMachine *vm) const;
		void setGotoIndexToInstruction(unsigned int instructionIndex);
		
		const ILogger *_logger;
		IRuntimeEnvironment *_environment;
		IVirtualMachine *_virtualMachine;
		SourceCode *_sourceCode;
		Sub *_currentSub;

	private:

		void addCodeInstruction(Opcode opcode, unsigned int arg = 0);
		void addCodeInstructionWithInt(Opcode opcode, int intValue, unsigned int arg = 0);
		void addCodeInstructionWithFloat(Opcode opcode, float floatValue, unsigned int arg = 0);
		void addCodeInstructionWithString(Opcode opcode, rstring *string, unsigned int arg = 0);
		void addCodePushIntConstant(TreeNode *node);
		void addCodePushFloatConstant(TreeNode *node);
		void addCodePushStringConstant(TreeNode *node);
		rstring extractStringFromNode(TreeNode *node);
		unsigned int getHashFromNode(TreeNode *node);

		int getLocalIdentificatorByToken(Token *token);
	};
}



#endif //__COMPILER_H_INCLUDED__