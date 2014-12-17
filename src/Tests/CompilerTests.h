#ifndef __COMPILER_TESTS_H_INCLUDED__
#define __COMPILER_TESTS_H_INCLUDED__


//-------------------------------------
#include "../CppTest/cpptest.h"
#include "../Raple/Headers/Compiler.h"
#include "../Raple/Headers/Parser.h"
#include "../Raple/Headers/SourceCode.h"
#include "../Raple/Headers/CompileResult.h"
#include "../Raple/Headers/Opcode.h"
#include "../Raple/Headers/Var.h"
#include "../Raple/Sources/Var.cpp"
#include "../Raple/Sources/Compiler.cpp"
//-------------------------------------
#include "FakeLoggerForCompilerTests.h"
#include "FakeRuntimeEnvironment.h"
#include "FakeVirtualMachine.h"
//-------------------------------------
#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif
//-------------------------------------
using Raple::Sub;
using Raple::rstring;
using Raple::Compiler;
using Raple::Parser;
using Raple::TreeNode;
using Raple::SourceCode;
using Raple::CompileResult;
using Raple::Tokenizer;
using Raple::Opcode;

using namespace RapleTestsHelpers;
//-------------------------------------


namespace RapleTests
{
	class CompilerTests : public Test::Suite
	{
	public:

		CompilerTests()
		{
			_logger = new FakeLoggerForCompilerTests();
			_compiler = new Compiler(_logger);
			_vm = new FakeVirtualMachine();
			
			_tokenizer = new Tokenizer();
			_parser = new Parser(_tokenizer, _logger);

			addTest(CompilerTests::TestForValidation);
			addTest(CompilerTests::TestForAddingFunctionToEnvironment);
			addTest(CompilerTests::TestForSimpleArithmeticalExpressionCompilation);
			addTest(CompilerTests::TestForVarDeclarationCompilation);
			addTest(CompilerTests::TestForFunctionCallCompilation);
			addTest(CompilerTests::TestForVarDeclarationWithFunctionCallCompilation);
			addTest(CompilerTests::TestForStringDeclarationWithFunctionCallCompilation);
			addTest(CompilerTests::TestForMultipleVarsDeclarationAndSubCallCompilation);
			addTest(CompilerTests::TestForTwoSubsDeclarationCompilation);
			addTest(CompilerTests::TestForFunctionCallAsFunctionCallArgumentCompilation);
			addTest(CompilerTests::TestForUnaryOperatorCompilation);
			addTest(CompilerTests::TestForAssignmentInReturnExpressionCompilation);
			addTest(CompilerTests::TestForAssignmentInFunctionCallCompilation);
			addTest(CompilerTests::TestForArithmeticalExpressionInFunctionCallCompilation);
			addTest(CompilerTests::TestForExponentiationOperatorCompilation);
			addTest(CompilerTests::TestForMultipleVarDeclarationCompilation);
			addTest(CompilerTests::TestForMultipleFunctionCalls);
			addTest(CompilerTests::TestForIfCompilation);
			addTest(CompilerTests::TestForIfCompilation2);
			addTest(CompilerTests::TestForIfElseCompilation);
			addTest(CompilerTests::TestForIfConditionExpressionWithLogicalOperatorCompilation);
			addTest(CompilerTests::TestForIfConditionExpressionWithDifficultLogicalOperatorsCompilation);
			addTest(CompilerTests::TestForPostIncrementCompilationFailedIfNodeIsConstant);
			addTest(CompilerTests::TestForPostIncrementCompilation);
			addTest(CompilerTests::TestForPostIncrementCompilationInComplexExpression);
			addTest(CompilerTests::TestForArrayIndexing);
			addTest(CompilerTests::TestForArrayInitialization);
			addTest(CompilerTests::TestForArrayItemsAppending);
			addTest(CompilerTests::TestForArrayItemsAppending2);
			addTest(CompilerTests::TestForWhileCompilation);
			addTest(CompilerTests::TestForInternalCallCompilation);
			addTest(CompilerTests::TestForInternalCallCompilation2);
			addTest(CompilerTests::TestForAnonymInitListCompilation);
			addTest(CompilerTests::TestForImportCompilation);
			addTest(CompilerTests::TestForImportCompilationFully);
			addTest(CompilerTests::TestForFailIfLibIsNotImported);
			addTest(CompilerTests::TestForDynamicCall);
			addTest(CompilerTests::TestForDoubleCall);
			addTest(CompilerTests::TestForComboIfExpressionCompilation);
			addTest(CompilerTests::TestForForeachCompilation);
		}

		~CompilerTests()
		{
			delete _tokenizer;
			delete _logger;
			delete _parser;

			delete _vm;
			delete _compiler;
		}

	private:

		void TestForValidation()
		{
			Raple::CompileResult result = _compiler->CompileTree(0, 0, 0);

			assertEquals(Raple::crNullTree, result);
			assertTrue(_logger->IsErrorLogged);
			assertEquals("Syntax tree isn't correct!", _logger->ErrorMessage);

			SourceCode *code = new SourceCode("sub main(){}");
			TreeNode *root = ParseSourceCode(code);

			result = _compiler->CompileTree(root, 0, 0);

			assertEquals(Raple::crNullSourceCode, result);
			assertEquals("Source code is empty!", _logger->ErrorMessage);

			result = _compiler->CompileTree(root, code, 0);

			assertEquals(Raple::crNullVm, result);
			assertEquals("Raple virtual machine isn't correct!", _logger->ErrorMessage);

			delete root;
			delete code;
		}

		void TestForAddingFunctionToEnvironment()
		{
			CompileAndExpect("sub main(){}", 1);
			
			assertEquals(rstring("main"), _vm->GetEnvironment()->GetSub(0)->GetName());
			assertEquals(Raple::opRet, _vm->GetEnvironment()->GetSub(0)->GetBytecode()->GetInstruction(0)->GetOpcode());
		}

		void TestForSimpleArithmeticalExpressionCompilation()
		{
			Bytecode *bytecode = CompileAndExpect("sub main(){return (1+2)*3;}", 7);

			Opcode expectedOpcodes[7] = { 
											Raple::opPushInt, Raple::opPushInt, Raple::opAdd,
											Raple::opPushInt, Raple::opProd, Raple::opRet,
											Raple::opRet
										};

			int expectedIntConsts[7] = { 1, 2, 0, 3, 0, 0, 0 };

			for (int i = 0; i < 7; ++i)
				assertEquals(expectedIntConsts[i], bytecode->GetInstruction(i)->Int());

			CompareBytecodes(expectedOpcodes, bytecode, 7);
		}

		void TestForVarDeclarationCompilation()
		{
			Bytecode *bytecode = CompileAndExpect("sub main(){var x=1+2*3;}", 7);

			Opcode expectedOpcodes[7] =  {
											Raple::opPushInt, Raple::opPushInt, Raple::opPushInt, 
											Raple::opProd, Raple::opAdd, Raple::opSetLocal,
											Raple::opRet
										};

			// into 0th local var
			assertEquals(0, bytecode->GetInstruction(5)->Argument());
			assertEquals(1, _vm->GetEnvironment()->GetSub(0)->GetLocalsCount());

			CompareBytecodes(expectedOpcodes, bytecode, 7);
		}

		void TestForFunctionCallCompilation()
		{
			Bytecode *bytecode = CompileAndExpect("sub main(){foo(21);}", 3);

			assertEquals(Raple::opCall, bytecode->GetInstruction(1)->GetOpcode());
			assertEquals(21, bytecode->GetInstruction(1)->Argument());
			assertEquals(Raple::opRet, bytecode->GetInstruction(2)->GetOpcode());
		}

		void TestForVarDeclarationWithFunctionCallCompilation()
		{
			Bytecode *bytecode = CompileAndExpect("sub main(){var x=1+foo(7);}", 6);

			Opcode expectedOpcodes[6] = {
											Raple::opPushInt, Raple::opPushInt, Raple::opCall, 
											Raple::opAdd, Raple::opSetLocal, Raple::opRet
										};

			assertEquals(21, bytecode->GetInstruction(2)->Argument());
			assertEquals(0, bytecode->GetInstruction(4)->Argument());
			assertEquals(1, _vm->GetEnvironment()->GetSub(0)->GetLocalsCount());

			CompareBytecodes(expectedOpcodes, bytecode, 6);
		}

		void TestForStringDeclarationWithFunctionCallCompilation()
		{
			Bytecode *bytecode = CompileAndExpect("sub main(){var str=\"string\";foo(str);}", 5);
			
			Opcode expectedOpcodes[5] = {
											Raple::opPushString, Raple::opSetLocal, Raple::opGetLocal,
											Raple::opCall, Raple::opRet
										};

			assertEquals(0, bytecode->GetInstruction(2)->Argument());
			assertEquals(21, bytecode->GetInstruction(3)->Argument());

			CompareBytecodes(expectedOpcodes, bytecode, 5);
		}

		void TestForMultipleVarsDeclarationAndSubCallCompilation()
		{
			Bytecode *bytecode = CompileAndExpect("sub main(){var a=7,b=14,c=a+b;foo(c);}", 11);

			Opcode expectedOpcodes[11] = {
											Raple::opPushInt, Raple::opSetLocal, Raple::opPushInt,
											Raple::opSetLocal, Raple::opGetLocal, Raple::opGetLocal,
											Raple::opAdd, Raple::opSetLocal, Raple::opGetLocal,
											Raple::opCall, Raple::opRet
										 };

			CompareBytecodes(expectedOpcodes, bytecode, 11);
		}

		void TestForMultipleFunctionCalls()
		{
			Bytecode *bytecode = CompileAndExpect("sub main(){var a=1;sub1(a);sub2(a);}", 7);

			Opcode expectedOpcodes[7] = {
											Raple::opPushInt, Raple::opSetLocal, Raple::opGetLocal,
											Raple::opCall, Raple::opGetLocal, Raple::opCall,
											Raple::opRet
										 };

			CompareBytecodes(expectedOpcodes, bytecode, 7);
		}

		void TestForTwoSubsDeclarationCompilation()
		{
			// main bytecode
			Bytecode *bytecode = CompileAndExpect("sub main(){} sub bar(){return;}", 1);

			assertEquals(Raple::opRet, bytecode->GetInstruction(0)->GetOpcode());

			// bar bytecode
			bytecode = _vm->GetEnvironment()->GetSub(1)->GetBytecode();
			assertEquals(2, bytecode->InstructionCount());
			assertEquals(Raple::opRet, bytecode->GetInstruction(0)->GetOpcode());
		}

		void TestForFunctionCallAsFunctionCallArgumentCompilation()
		{
			Bytecode *bytecode = CompileAndExpect("sub main(){foo(bar());}", 3);

			Opcode expectedOpcodes[3] = {
											Raple::opCall, Raple::opCall, 
											Raple::opRet
										};

			CompareBytecodes(expectedOpcodes, bytecode, 3);
		}

		void TestForUnaryOperatorCompilation()
		{
			Bytecode *bytecode = CompileAndExpect("sub main(){foo(-5);}", 3);

			Opcode expectedOpcodes[3] = {
											Raple::opPushInt, Raple::opCall, 
											Raple::opRet
										};

			CompareBytecodes(expectedOpcodes, bytecode, 3);
		}

		void TestForAssignmentInReturnExpressionCompilation()
		{
			Bytecode *bytecode = CompileAndExpect("sub main(){var x=5;x=3;}", 5);

			Opcode expectedOpcodes[5] = {
											Raple::opPushInt, Raple::opSetLocal, Raple::opPushInt, 
											Raple::opSetLocal, Raple::opRet
										};

			CompareBytecodes(expectedOpcodes, bytecode, 5);
		}

		void TestForAssignmentInFunctionCallCompilation()
		{
			Bytecode *bc = CompileAndExpect("sub main(){var x=2;foo(x=5);}", 7);

			Opcode expectedOpcodes[7] = {
											Raple::opPushInt, Raple::opSetLocal, Raple::opPushInt,
											Raple::opSetLocal, Raple::opGetLocal, Raple::opCall,
											Raple::opRet
										};

			CompareBytecodes(expectedOpcodes, bc, 7);
		}

		void TestForArithmeticalExpressionInFunctionCallCompilation()
		{
			Bytecode *bc = CompileAndExpect("sub main(){var x=5;foo(x*2+1);}", 9);

			Opcode expectedOpcodes[9] = {
											Raple::opPushInt, Raple::opSetLocal, Raple::opGetLocal, 
											Raple::opPushInt, Raple::opProd, Raple::opPushInt,
											Raple::opAdd, Raple::opCall, Raple::opRet
										};

			CompareBytecodes(expectedOpcodes, bc, 9);
		}

		void TestForExponentiationOperatorCompilation()
		{
			Bytecode *bc = CompileAndExpect("sub main(){return 1+2*3**4;}", 9);

			Opcode expectedOpcodes[9] = {
											Raple::opPushInt, Raple::opPushInt, Raple::opPushInt,
											Raple::opPushInt, Raple::opExp, Raple::opProd,
											Raple::opAdd, Raple::opRet, Raple::opRet
										};

			CompareBytecodes(expectedOpcodes, bc, 9);
		}

		void TestForMultipleVarDeclarationCompilation()
		{
			Bytecode *bc = CompileAndExpect("sub main(){var x=6,y=1,z=3;}", 7);

			Opcode expectedOpcodes[7] = {
											Raple::opPushInt, Raple::opSetLocal, Raple::opPushInt,
											Raple::opSetLocal, Raple::opPushInt, Raple::opSetLocal,
											Raple::opRet
										};

			int args[7] = { 0, 0, // x
							0, 1, // y
							0, 2, // z
							0 };

			for (int i = 0; i < 7; ++i)
				assertEquals(args[i], bc->GetInstruction(i)->Argument());

			int consts[7] = {
								6, 0, // x
								1, 0, // y
								3, 0, // z
								0
							};

			for (int i = 0; i < 7; ++i)
				assertEquals(consts[i], bc->GetInstruction(i)->Int());

			CompareBytecodes(expectedOpcodes, bc, 7);
		}

		void TestForWhileCompilation()
		{
			Bytecode *bc = CompileAndExpect("sub main(){while(1==2){return 2;}}", 8);

			Opcode expected[8] = {
				Raple::opPushInt, Raple::opPushInt, Raple::opEqual,
				Raple::opJumpZero, Raple::opPushInt, Raple::opRet,
				Raple::opGoto, Raple::opRet
			};

			int args[8] = {
				0, 0, 0, 7, 
				0, 1, 
				0, // goto to first (zero) instructon
				0
			};

			for (int i = 0; i < 8; ++i)
				assertEquals(args[i], bc->GetInstruction(i)->Argument());

			CompareBytecodes(expected, bc, 8);
		}

		void TestForIfCompilation()
		{
			Bytecode *bc = CompileAndExpect("sub main(){if(1==2) return 0;}", 7);

			Opcode expectedOpcodes[7] = {
				Raple::opPushInt, Raple::opPushInt, Raple::opEqual, 
				Raple::opJumpZero, Raple::opPushInt, Raple::opRet, 
				Raple::opRet
			};

			int args[7] = {
				0, 0, 0, 6, 0, 1, 0
			};

			for (int i = 0; i < 7; ++i)
				assertEquals(args[i], bc->GetInstruction(i)->Argument());

			CompareBytecodes(expectedOpcodes, bc, 7);
		}
		
		void TestForIfCompilation2()
		{
			Bytecode *bc = CompileAndExpect("sub main(){if(1==2) return 0; return 2;}", 9);

			Opcode expectedOpcodes[9] = {
				Raple::opPushInt, Raple::opPushInt, Raple::opEqual, 
				Raple::opJumpZero, Raple::opPushInt, Raple::opRet, 
				Raple::opPushInt, Raple::opRet, Raple::opRet
			};

			int args[9] = {
				0, 0, 0, 6, 0, 1, 0, 1, 0
			};

			for (int i = 0; i < 9; ++i)
				assertEquals(args[i], bc->GetInstruction(i)->Argument());

			CompareBytecodes(expectedOpcodes, bc, 9);
		}

		void TestForIfElseCompilation()
		{
			Bytecode *bc = CompileAndExpect("sub main() { if(1)foo('yes');else foo('no'); }", 8);

			Opcode expected[8] = {
				Raple::opPushInt, Raple::opJumpZero, Raple::opPushString,
				Raple::opCall, Raple::opGoto, Raple::opPushString, 
				Raple::opCall, 	Raple::opRet
			};

			// 21 - это id функции foo для тестов
			int args[8] = {
				0, 5, 0, 21, 7, 0, 21, 0
			};

			for (int i = 0; i < 8; ++i)
				assertEquals(args[i], bc->GetInstruction(i)->Argument());

			CompareBytecodes(expected, bc, 8);
		}

		void TestForIfConditionExpressionWithLogicalOperatorCompilation()
		{
			Bytecode *bc = CompileAndExpect("sub main(){if(1==2&&2==3)return;return;}", 11);

			Opcode expected[11] = {
				Raple::opPushInt, Raple::opPushInt, Raple::opEqual,
				Raple::opPushInt, Raple::opPushInt, Raple::opEqual, 
				Raple::opLogicAnd, Raple::opJumpZero, Raple::opRet, 
				Raple::opRet, Raple::opRet
			};

			int args[11] = {
				0, 0, 0, 0, 0, 0, 0, 9, 0, 0, 0
			};

			for (int i = 0; i < 11; ++i)
				assertEquals(args[i], bc->GetInstruction(i)->Argument());

			CompareBytecodes(expected, bc, 11);
		}

		void TestForComboIfExpressionCompilation()
		{
			Bytecode *bc = CompileAndExpect("sub main(){if (1 != 1 && 1 != 2) return 2; return 1;}", 13);

			Opcode expected[13] = {
				Raple::opPushInt, Raple::opPushInt, Raple::opNotEqual,
				Raple::opPushInt, Raple::opPushInt, Raple::opNotEqual, 
				Raple::opLogicAnd, Raple::opJumpZero, Raple::opPushInt,
				Raple::opRet, Raple::opPushInt, Raple::opRet, Raple::opRet
			};

			int args[13] = {
				0, 0, 0, 0, 0, 0, 0, 10, 0, 1, 0, 1, 0
			};

			for (int i = 0; i < 13; ++i)
				assertEquals(args[i], bc->GetInstruction(i)->Argument());

			CompareBytecodes(expected, bc, 13);
		}

		void TestForIfConditionExpressionWithDifficultLogicalOperatorsCompilation()
		{
			Bytecode *bc = CompileAndExpect("sub main(){if((1==2&&2==3)||3==3)return;return;}", 15);

			Opcode expected[15] = {
				Raple::opPushInt, Raple::opPushInt, Raple::opEqual,
				Raple::opPushInt, Raple::opPushInt, Raple::opEqual, 
				Raple::opLogicAnd, Raple::opPushInt, Raple::opPushInt, 
				Raple::opEqual, Raple::opLogicOr, Raple::opJumpZero,
				Raple::opRet, Raple::opRet, Raple::opRet
			};

			int args[15] = {
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 13, 0, 0, 0
			};

			for (int i = 0; i < 15; ++i)
				assertEquals(args[i], bc->GetInstruction(i)->Argument());

			CompareBytecodes(expected, bc, 15);
		}

		void TestForPostIncrementCompilationFailedIfNodeIsConstant()
		{
			CompileAndExpect("sub main(){var a=2++;}", 0, Raple::crFailed);
		}

		void TestForPostIncrementCompilation()
		{
			Bytecode *bc = CompileAndExpect("sub main(a){return a++;}", 4);
			
			Opcode expected[4] = { 
				Raple::opGetLocal, Raple::opPostInc, Raple::opRet, 
				Raple::opRet
			};
			
			CompareBytecodes(expected, bc, 4);
		}

		void TestForPostIncrementCompilationInComplexExpression()
		{
			Bytecode *bc = CompileAndExpect("sub main(a){return 2*a++ -3;}", 8);

			Opcode expected[8] = {
				Raple::opPushInt, Raple::opGetLocal, Raple::opPostInc,
				Raple::opProd, Raple::opPushInt, Raple::opSub,
				Raple::opRet, Raple::opRet
			};

			CompareBytecodes(expected, bc, 8);
		}

		void TestForArrayIndexing()
		{
			Bytecode *bc = CompileAndExpect("sub main(a){return a['key'];}", 5);

			Opcode expected[5] = {
				Raple::opPushString, Raple::opGetLocal, Raple::opGetElement, 
				Raple::opRet, Raple::opRet
			};

			CompareBytecodes(expected, bc, 5);
		}

		void TestForArrayInitialization()
		{
			Bytecode *bc = CompileAndExpect("sub main(){var a={1,2,3};}", 14);

			Opcode expected[14] = {
				Raple::opNewArray, Raple::opPushInt, 
				Raple::opPushInt, Raple::opGetLocal, Raple::opSetElement, Raple::opPushInt, 
				Raple::opPushInt, Raple::opGetLocal, Raple::opSetElement, Raple::opPushInt, 
				Raple::opPushInt, Raple::opGetLocal, Raple::opSetElement, Raple::opRet
			};

			// "a" как единственная локальная переменная будет иметь индекс 0

			// аргументы все будут 0
			for (int i = 0; i < 14; ++i)
				assertEquals(0, bc->GetInstruction(i)->Argument());

			int values[14] = {
				0, 1, 0, 0, 0, 2, 1,  0, 0, 3, 2, 0, 0, 0
			};

			for (int i = 0; i < 14; ++i)
				if (bc->GetInstruction(i)->GetOpcode() == Raple::opPushInt)
					assertEquals(values[i], bc->GetInstruction(i)->Int());

			CompareBytecodes(expected, bc, 14);
		}

		void TestForArrayItemsAppending()
		{
			Bytecode *bc = CompileAndExpect("sub main(){var a={1};a['key']=3;}", 10);

			Opcode expected[10] = {
				Raple::opNewArray, Raple::opPushInt, Raple::opPushInt, 
				Raple::opGetLocal, Raple::opSetElement, Raple::opPushInt, 
				Raple::opPushString, Raple::opGetLocal, Raple::opSetElement, 
				Raple::opRet
			};

			CompareBytecodes(expected, bc);
		}

		void TestForArrayItemsAppending2()
		{
			Bytecode *bc = CompileAndExpect("sub main(){var a={};a[0]=1;a['1']=2;}", 10);

			Opcode expected[10] = {
				Raple::opNewArray, Raple::opPushInt, Raple::opPushInt, 
				Raple::opGetLocal, Raple::opSetElement, Raple::opPushInt, 
				Raple::opPushString, Raple::opGetLocal, Raple::opSetElement, 
				Raple::opRet
			};

			CompareBytecodes(expected, bc);

			assertTrue(0 == strcmp(bc->GetInstruction(6)->String()->GetBuffer(), "1"));
		}

		void TestForInternalCallCompilation()
		{
			Bytecode *bc = CompileAndExpect("sub main(){var s='raple';foo(s.len());}", 6);

			Opcode expected[6] = {
				Raple::opPushString, Raple::opSetLocal, Raple::opGetLocal, 
				Raple::opInternalCall, Raple::opCall, Raple::opRet
			};

			CompareBytecodes(expected, bc);

			unsigned int lenHash = rstring("len").Hash();
			assertEquals(lenHash, bc->GetInstruction(3)->Argument());
		}

		void TestForInternalCallCompilation2()
		{
			Bytecode *bc = CompileAndExpect("sub main(){var l=foo(2).length();}", 5);

			Opcode expected[5] = {
				Raple::opPushInt, Raple::opCall, Raple::opInternalCall, 
				Raple::opSetLocal, Raple::opRet
			};

			CompareBytecodes(expected, bc);

			unsigned int lenHash = rstring("length").Hash();
			assertEquals(lenHash, bc->GetInstruction(2)->Argument());
		}

		void CompilerTests::TestForAnonymInitListCompilation()
		{
			Bytecode *bc = CompileAndExpect("sub main() {return {1,2};}", 12);

			Opcode expected[12] = {
				Raple::opNewArray, Raple::opPushInt, Raple::opPushInt,
				Raple::opGetLocal, Raple::opSetElement, Raple::opPushInt, 
				Raple::opPushInt, Raple::opGetLocal, Raple::opSetElement,
				Raple::opGetLocal, Raple::opRet, Raple::opRet
			};

			CompareBytecodes(expected, bc);
		}

		void CompilerTests::TestForImportCompilation()
		{
			assertFalse(_vm->GetEnvironment()->IsLibraryImported("foo"));

			CompileAndExpect("import foo;sub main(){}", 1);

			assertTrue(_vm->GetEnvironment()->IsLibraryImported("foo"));
		}

		void CompilerTests::TestForImportCompilationFully()
		{
			Bytecode *bc = CompileAndExpect("import foo; sub main(){foo.bar();}", 3);

			Opcode expected[3] = {
				Raple::opPushInt, Raple::opStaticCall, Raple::opRet
			};

			CompareBytecodes(expected, bc);

			assertEquals(rstring("foo").Hash(), bc->GetInstruction(0)->Int());
			assertEquals(rstring("bar").Hash(), bc->GetInstruction(1)->Argument());
		}

		void CompilerTests::TestForFailIfLibIsNotImported()
		{
			CompileAndExpect("sub main(){xoxo.xaxa();}", 0, Raple::crFailed);
		}

		void CompilerTests::TestForDynamicCall()
		{
			Bytecode *bc = CompileAndExpect("sub main(){var pf=sub(){};pf();}", 5);

			Opcode expected[5] = {
				Raple::opGetSub, Raple::opSetLocal,	Raple::opGetLocal, 
				Raple::opDynamicCall, Raple::opRet
			};

			CompareBytecodes(expected, bc);
		}

		void CompilerTests::TestForDoubleCall()
		{
			Bytecode *bc = CompileAndExpect("sub main(){var x=21;var v=x.tostr().type();}", 7);

			Opcode expected[7] = {
				Raple::opPushInt, Raple::opSetLocal, Raple::opGetLocal,
				Raple::opInternalCall, Raple::opInternalCall, Raple::opSetLocal, 
				Raple::opRet
			};

			CompareBytecodes(expected, bc);
		}

		void TestForForeachCompilation()
		{
			Bytecode *bc = CompileAndExpect("sub main(){foreach(var n in {1,2}){return n;}}", 26);

			Opcode expected[26] = {
				Raple::opPushInt, Raple::opSetLocal, Raple::opNewArray,
				Raple::opPushInt, Raple::opPushInt, Raple::opGetLocal,
				Raple::opSetElement, Raple::opPushInt, Raple::opPushInt, 
				Raple::opGetLocal, Raple::opSetElement, Raple::opGetLocal,
				Raple::opGetLocal, Raple::opGetNextElement, Raple::opPushInt,
				Raple::opAdd, Raple::opSetLocal, Raple::opSetLocal, Raple::opGetLocal,
				Raple::opArraySize, Raple::opGreater, Raple::opJumpNotZero,
				Raple::opGetLocal, Raple::opRet, Raple::opGoto, Raple::opRet
			};

			unsigned int args[26] = {
				0, 1, 2, 0, 0, 2, 0, 0, 0, 2, 0, 1, 2, 0, 0, 0, 1, 0, 1, 2, 0, 25, 0, 1, 11, 0
			};

			for (int i = 0; i < 26; ++i)
				assertEquals(args[i], bc->GetInstruction(i)->Argument());

			CompareBytecodes(expected, bc);
		}
		
		////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////

		Bytecode *CompileAndExpect(const char *code, unsigned int expectedOpcodesCount, Raple::CompileResult expectedResult = Raple::crSuccess)
		{
			SourceCode *sourceCode = new SourceCode(code);
			TreeNode *root = ParseSourceCode(sourceCode);

			Raple::CompileResult actualResult = _compiler->CompileTree(root, sourceCode, _vm);
			Bytecode *bytecode = _vm->GetEnvironment()->GetSub(0)->GetBytecode();

			AssertEqualsResults(expectedResult, actualResult);
			AssertEqualsInts(expectedOpcodesCount, bytecode->InstructionCount());

			delete sourceCode;
			delete root;

			return bytecode;
		}

		void CompareBytecodes(Opcode expected[], const Bytecode *actual, int count)
		{
			for (int i = 0; i < count; ++i)
				assertEquals(expected[i], actual->GetInstruction(i)->GetOpcode());
		}

		void CompareBytecodes(Opcode expected[], const Bytecode *actual)
		{
			const int count = actual->InstructionCount();

			for (int i = 0; i < count; ++i)
				assertEquals(expected[i], actual->GetInstruction(i)->GetOpcode());
		}

		void AssertEqualsResults(Raple::CompileResult expected, Raple::CompileResult actual)
		{
			assertEquals(expected, actual);
		}

		void AssertEqualsInts(unsigned int expected, unsigned int actual)
		{
			assertEquals(expected, actual);
		}

		TreeNode *ParseSourceCode(const SourceCode *sourceCode) 
		{
			TreeNode *root = _parser->ParseScript(sourceCode);
			AssertEqualsInts((int)false, (int)_parser->HasErrorsWhileParsing());
			return root;
		}

		Compiler *_compiler;
		IVirtualMachine *_vm;

		Tokenizer *_tokenizer;
		FakeLoggerForCompilerTests *_logger;
		Parser *_parser;
	};
}

#endif //__COMPILER_TESTS_H_INCLUDED__