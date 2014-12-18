#ifndef __PARSER_TESTS_H_INCLUDED__
#define __PARSER_TESTS_H_INCLUDED__


//-------------------------------------
#include "../CppTest/cpptest.h"
#include "../Raple/Headers/Parser.h"
#include "../Raple/Headers/String.h"
#include "../Raple/Headers/TreeNode.h"
#include "../Raple/Headers/SourceCode.h"
#include "../Raple/Headers/Token.h"
#include "../Raple/Headers/Tokenizer.h"
#include "../Raple/Sources/Parser.cpp"
#include "../Raple/Sources/String.cpp"
#include "../Raple/Sources/TreeNode.cpp"
#include "../Raple/Sources/SourceCode.cpp"
//-------------------------------------
#include "FakeLoggerForParserTests.h"
//-------------------------------------
#include <memory>
//-------------------------------------
#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif
//-------------------------------------
using Raple::Parser;
using Raple::TreeNode;
using Raple::Token;
using Raple::Tokenizer;
using Raple::SourceCode;

using RapleTestsHelpers::FakeLoggerForParserTests;
using namespace Raple::Constants;
//-------------------------------------


namespace RapleTests
{
	class ParserTests : public Test::Suite
	{
	public:

		ParserTests()
		{
			_logger = new FakeLoggerForParserTests();
			_tokenizer = new Tokenizer();
			_parser = new Parser(_tokenizer, _logger);

			addTest(ParserTests::TreeNodeUpdateDataTest);
			addTest(ParserTests::TreeNodeChildsTest);
			addTest(ParserTests::CorrectlyParseEmptyFunctionDefinition);
			addTest(ParserTests::LogErrorIfUnexpectedEndOfFile);
			addTest(ParserTests::CorrectlyParseArithmeticalExpression);
			addTest(ParserTests::CorrectlyParseReturnExpression);
			addTest(ParserTests::DetectsSyntaxErrors);
			addTest(ParserTests::CorrectlyParseFunctionCall);
			addTest(ParserTests::CorrectlyParseUnaryOperator);
			addTest(ParserTests::CorrectlyParseAssignment);
			addTest(ParserTests::CorrectlyParseExponentiationOperation);
			addTest(ParserTests::CorrectlyParseMultipleVarDeclaration);
			addTest(ParserTests::CorrectlyParseIfElseStatements);
			addTest(ParserTests::CorrectlyParseOnlyIfStatements);
			addTest(ParserTests::CorrectlyParseArrayIndexing);
			addTest(ParserTests::CorrectlyParseArrayInitialization);
			addTest(ParserTests::CorrectlyParseEmptyArrayInitialization);
			addTest(ParserTests::CorrectlyParseMultiplyCommasInInitializationList);
			addTest(ParserTests::CorrectlyParseWhileCycle);
			addTest(ParserTests::CorrectlyParseInstanceFunctionCalling);
			addTest(ParserTests::CorrectlyParseReturnArrayList);
			addTest(ParserTests::CorrectlyParseTrueFalseContants);
			addTest(ParserTests::CorrectlyParseForEach);
			addTest(ParserTests::CorrectlyParseRange);
		}

		~ParserTests()
		{
			delete _logger;
			delete _tokenizer;
			delete _parser;
		}

	private:


		void TreeNodeUpdateDataTest()
		{
			std::auto_ptr<TreeNode> tn(new TreeNode(Raple::ntIdentifier));
		
			tn->SetToken(new Token(Raple::ttIdentifier, 0, 2, 3));
			tn->UpdateData(0, 0, 0);

			assertEquals(0, tn->GetToken()->Position);
			assertEquals(2, tn->GetToken()->Length);
			assertEquals(0, tn->GetToken()->Row);

			tn->UpdateData(3, 3, 4);

			assertEquals(0, tn->GetToken()->Position);
			assertEquals(6, tn->GetToken()->Length);
			assertEquals(4, tn->GetToken()->Row);
		}

		void TreeNodeChildsTest()
		{
			TreeNode *root = new TreeNode(Raple::ntCode);
			TreeNode *sub = new TreeNode(Raple::ntSub);
			TreeNode *subIdent = new TreeNode(Raple::ntIdentifier);
			TreeNode *subArglist = new TreeNode(Raple::ntFunctionDefinitionArgList);
			TreeNode *subArglistIdent = new TreeNode(Raple::ntIdentifier);
			TreeNode *subStatements = new TreeNode(Raple::ntStatementsBlock);

			root->AddChild(sub);
			sub->AddChild(subIdent);
			sub->AddChild(subArglist);
			subArglist->AddChild(subArglistIdent);
			sub->AddChild(subStatements);

			assertEquals(1, root->ChildCount());
			assertEquals(3, sub->ChildCount());
			assertEquals(1, subArglist->ChildCount());
			assertEquals(0, subStatements->ChildCount());

			assertEquals(Raple::ntFunctionDefinitionArgList, sub->GetChild(0)->Next->GetType());
			assertEquals((TreeNode *)0, sub->GetChild(2)->Next);

			delete root;
		}

		void CorrectlyParseEmptyFunctionDefinition()
		{
			std::auto_ptr<SourceCode> code(new SourceCode("sub main() {}"));
			std::auto_ptr<TreeNode> root(_parser->ParseScript(code.get()));

			assertEquals(Raple::ntCode, root->GetType());
		
			// one function definition node
			assertEquals(1, root->ChildCount());
			assertEquals(Raple::ntSub, root->GetChild(TreeIndexCode)->GetType());
		
			// sub identifier 'main' starts at 4 and ends at 8
			assertEquals(4, root->GetChild(TreeIndexCode)->GetChild(TreeIndexSubName)->GetToken()->Position);
			assertEquals(4, root->GetChild(TreeIndexCode)->GetChild(TreeIndexSubName)->GetToken()->Length);
			assertEquals(Raple::ntIdentifier, root->GetChild(TreeIndexCode)->GetChild(TreeIndexSubName)->GetType());

			// sub main has no arguments
			assertEquals(0, root->GetChild(TreeIndexCode)->GetChild(TreeIndexSubArguments)->ChildCount());
			assertEquals(Raple::ntFunctionDefinitionArgList, root->GetChild(TreeIndexCode)->GetChild(TreeIndexSubArguments)->GetType());

			// sub main has no statements
			assertEquals(0, root->GetChild(TreeIndexCode)->GetChild(TreeIndexSubBody)->ChildCount());
			assertEquals(Raple::ntStatementsBlock, root->GetChild(TreeIndexCode)->GetChild(TreeIndexSubBody)->GetType());
		}

		void LogErrorIfUnexpectedEndOfFile()
		{
			std::auto_ptr<SourceCode> code(new SourceCode("sub main() {"));
			std::auto_ptr<TreeNode> root(_parser->ParseScript(code.get()));

			assertEquals(true, _logger->IsMessageLogged);
			assertEquals("Unexpected token '<end of file>' at (1,13)", _logger->Message);

			_logger->IsMessageLogged = false;
			_logger->Message = "";
		}

		void CorrectlyParseArithmeticalExpression()
		{
			std::auto_ptr<SourceCode> code(new SourceCode("sub main() {var x = (1+2*3)*3;}"));
			std::auto_ptr<TreeNode> root(_parser->ParseScript(code.get()));

			// sub main has 1 statement - var declaration
			assertEquals(1, root->GetChild(TreeIndexCode)->GetChild(TreeIndexSubBody)->ChildCount());

			TreeNode *node = root->GetChild(TreeIndexCode)->GetChild(TreeIndexSubBody)->GetChild(0);

			// var declaration has 2 child: identifier & assignment
			assertEquals(2, node->ChildCount());
			assertEquals(Raple::ntIdentifier, node->GetChild(0)->GetType());
			assertEquals(Raple::ntBinaryOperator, node->GetChild(1)->GetType());
			assertEquals(Raple::ttStar, node->GetChild(1)->GetToken()->Type);

			node = node->GetChild(1);
		
			// (.)*3
			assertEquals(Raple::ntConstant, node->GetChild(1)->GetType());
			assertEquals(Raple::ntBinaryOperator, node->GetChild(0)->GetType());
			assertEquals(Raple::ttPlus, node->GetChild(0)->GetToken()->Type);

			// (...)
			node = node->GetChild(0);
			assertEquals(Raple::ntConstant, node->GetChild(0)->GetType());
			assertEquals(Raple::ntBinaryOperator, node->GetChild(1)->GetType());
		
			// *
			node = node->GetChild(1);
			assertEquals(Raple::ttStar, node->GetToken()->Type);
			assertEquals(Raple::ntConstant, node->GetChild(0)->GetType());
			assertEquals(Raple::ntConstant, node->GetChild(1)->GetType());
		}

		void CorrectlyParseReturnExpression()
		{
			SourceCode *code = new SourceCode("sub main() {return;}");
			TreeNode *root = _parser->ParseScript(code);

			// 1 statement - return
			assertEquals(1, root->GetChild(TreeIndexCode)->GetChild(TreeIndexSubBody)->ChildCount());

			TreeNode *returnNode = root->GetChild(TreeIndexCode)->GetChild(TreeIndexSubBody)->GetChild(0);

			// hasn't return value
			assertEquals(0, returnNode->ChildCount());
			delete root;

			code->SetCode("sub main() {return 1+2;}");
			root = _parser->ParseScript(code);

			// 1 statement - return
			assertEquals(1, root->GetChild(TreeIndexCode)->GetChild(TreeIndexSubBody)->ChildCount());

			returnNode = root->GetChild(TreeIndexCode)->GetChild(TreeIndexSubBody)->GetChild(0);

			// returns 1 binary operation
			assertEquals(1, returnNode->ChildCount());
			assertEquals(Raple::ntBinaryOperator, returnNode->GetChild(0)->GetType());
			assertEquals(Raple::ttPlus, returnNode->GetChild(0)->GetToken()->Type);

			delete root;
			delete code;
		}

		void DetectsSyntaxErrors()
		{
			SourceCode *code = new SourceCode("sdhfs sdds2");
			TreeNode *root = _parser->ParseScript(code);

			assertTrue(_parser->HasErrorsWhileParsing());
			assertTrue(_logger->IsMessageLogged);
			assertEquals("Unexpected token '<identifier>' at (1,1)", _logger->Message);

			delete root;
			delete code;
		}

		void CorrectlyParseFunctionCall()
		{
			SourceCode *code = new SourceCode("sub main(){foo(x);}");
			TreeNode *root = _parser->ParseScript(code);

			assertEquals(Raple::ntCode, root->GetType());
			assertEquals(1, root->ChildCount());
			assertEquals(Raple::ntSub, root->GetChild(TreeIndexCode)->GetType());

			TreeNode *callNode = root->GetChild(TreeIndexCode)->GetChild(TreeIndexSubBody)->GetChild(0);
			assertEquals(Raple::ntFunctionCall, callNode->GetType());

			TreeNode *id = callNode->GetChild(0);
			TreeNode *arg = callNode->GetChild(1);

			// length 'foo' = 3
			assertEquals(3, id->GetToken()->Length);
			assertEquals(1, arg->ChildCount());

			TreeNode *argid = arg->GetChild(0);
			assertEquals(1, argid->GetToken()->Length);

			delete code;
			delete root;
		}

		void CorrectlyParseUnaryOperator()
		{
			SourceCode *code = new SourceCode("sub main(){foo(-x);}");
			TreeNode *root = _parser->ParseScript(code);

			TreeNode *callNode = root->GetChild(TreeIndexCode)->GetChild(TreeIndexSubBody)->GetChild(0);
			TreeNode *arg = callNode->GetChild(1)->GetChild(0);

			assertEquals(Raple::ntPreOperator, arg->GetType());
			assertEquals(Raple::ttMinus, arg->GetToken()->Type);

			delete code;
			delete root;
		}

		void CorrectlyParseAssignment()
		{
			SourceCode *code = new SourceCode("sub main(){x=5;}");
			TreeNode *root = _parser->ParseScript(code);

			TreeNode *assignNode = root->GetChild(TreeIndexCode)->GetChild(TreeIndexSubBody)->GetChild(0);

			assertEquals(2, assignNode->ChildCount());
			assertEquals(Raple::ntAssignment, assignNode->GetType());
			assertEquals(Raple::ntIdentifier, assignNode->GetChild(0)->GetType());
			assertEquals(Raple::ntConstant, assignNode->GetChild(1)->GetType());

			delete code;
			delete root;
		}

		void CorrectlyParseExponentiationOperation()
		{
			SourceCode *code = new SourceCode("sub main(){x=3*2**5;}");
			TreeNode *root = _parser->ParseScript(code);

			TreeNode *assignNode = root->GetChild(TreeIndexCode)->GetChild(TreeIndexSubBody)->GetChild(0);

			assertEquals(2, assignNode->ChildCount());
			assertEquals(Raple::ntAssignment, assignNode->GetType());
			assertEquals(Raple::ntIdentifier, assignNode->GetChild(0)->GetType());
			assertEquals(Raple::ntBinaryOperator, assignNode->GetChild(1)->GetType());

			//           *
			//          / \
			//         3   **
			//            /  \
			//           2    5

			TreeNode *mulNode = assignNode->GetChild(1);

			assertEquals(Raple::ttStar, mulNode->GetToken()->Type);
			assertEquals(Raple::ntConstant, mulNode->GetChild(0)->GetType());
			assertEquals(Raple::ntBinaryOperator, mulNode->GetChild(1)->GetType());

			TreeNode *expNode = mulNode->GetChild(1);

			assertEquals(Raple::ttExponentiation, expNode->GetToken()->Type);
			assertEquals(Raple::ntConstant, expNode->GetChild(0)->GetType());
			assertEquals(Raple::ntConstant, expNode->GetChild(1)->GetType());

			delete code;
			delete root;
		}

		void CorrectlyParseMultipleVarDeclaration()
		{
			SourceCode *code = new SourceCode("sub main(){var x=12,y=13,z=14;}");
			TreeNode *root = _parser->ParseScript(code);

			TreeNode *body = root->GetChild(TreeIndexCode)->GetChild(TreeIndexSubBody);

			assertEquals(1, body->ChildCount());

			assertEquals(3, body->GetChild(0)->ChildCount());
			assertEquals(Raple::ntVarDeclaration, body->GetChild(0)->GetType());

			assertEquals(3, body->GetChild(0)->GetChild(2)->ChildCount());
			assertEquals(Raple::ntVarDeclaration, body->GetChild(0)->GetChild(2)->GetType());

			assertEquals(2, body->GetChild(0)->GetChild(2)->GetChild(2)->ChildCount());
			assertEquals(Raple::ntVarDeclaration, body->GetChild(0)->GetChild(2)->GetChild(2)->GetType());

			delete code;
			delete root;
		}

		void CorrectlyParseIfElseStatements()
		{
			SourceCode *code = new SourceCode("sub main(){if(1==2 && 2!=3) return 0; else return 1;}");
			TreeNode *root = _parser->ParseScript(code);
			TreeNode *body = root->GetChild(TreeIndexCode)->GetChild(TreeIndexSubBody);
			assertEquals(1, body->ChildCount());
			
			TreeNode *ifelse = body->GetChild(0);
			assertEquals(Raple::ntIfElse, ifelse->GetType());
			assertEquals(3, ifelse->ChildCount());

			TreeNode *condition = ifelse->GetChild(TreeIndexIfElseCondition);
			TreeNode *ifstat = ifelse->GetChild(TreeIndexIfElseIfStatements);
			TreeNode *elsestat = ifelse->GetChild(TreeIndexIfElseElseStatements);

			assertEquals(Raple::ntBinaryOperator, condition->GetType());
			assertEquals(Raple::ntReturn, ifstat->GetType());
			assertEquals(Raple::ntReturn, elsestat->GetType());

			assertEquals(2, condition->ChildCount());
			assertEquals(Raple::ttAnd, condition->GetToken()->Type);
			assertEquals(Raple::ttEqual, condition->GetChild(0)->GetToken()->Type);
			assertEquals(Raple::ttNotEqual, condition->GetChild(1)->GetToken()->Type);

			delete code;
			delete root;
		}

		void CorrectlyParseOnlyIfStatements()
		{
			SourceCode *code = new SourceCode("sub main(){if(1==2) return 0;}");
			TreeNode *root = _parser->ParseScript(code);
			TreeNode *body = root->GetChild(TreeIndexCode)->GetChild(TreeIndexSubBody);
			assertEquals(1, body->ChildCount());

			TreeNode *ifexpr = body->GetChild(0);
			assertEquals(Raple::ntIfElse, ifexpr->GetType());

			TreeNode *condition = ifexpr->GetChild(TreeIndexIfElseCondition);
			TreeNode *ifstat = ifexpr->GetChild(TreeIndexIfElseIfStatements);

			assertEquals(Raple::ntBinaryOperator, condition->GetType());
			assertEquals(Raple::ntReturn, ifstat->GetType());
			assertEquals(2, condition->ChildCount());

			assertEquals(Raple::ttEqual, condition->GetToken()->Type);
			assertEquals(Raple::ntConstant, condition->GetChild(0)->GetType());
			assertEquals(Raple::ntConstant, condition->GetChild(1)->GetType());

			delete code;
			delete root;
		}

		void CorrectlyParseArrayIndexing()
		{
			SourceCode *code = new SourceCode("sub main(){a[2]++;}");
			// сначала получить значение по индексу, а потом уже инкремент

			TreeNode *root = _parser->ParseScript(code);
			TreeNode *body = root->GetChild(TreeIndexCode)->GetChild(TreeIndexSubBody);
			assertEquals(1, body->ChildCount());

			TreeNode *expr = body->GetChild(0);
			assertEquals(Raple::ntPostOperator, expr->GetType());
			assertEquals(Raple::ttIncrement, expr->GetToken()->Type);

			TreeNode *indexing = expr->GetChild(0);
			assertEquals(Raple::ntIndexing, indexing->GetType());
			assertEquals(2, indexing->ChildCount());
			assertEquals(Raple::ntIdentifier, indexing->GetChild(0)->GetType());
			assertEquals(Raple::ntConstant, indexing->GetChild(1)->GetType());
			
			delete code;
			delete root;
		}

		void CorrectlyParseArrayInitialization()
		{
			SourceCode code("sub main(){var a={1,2.345,'abc'};}");

			TreeNode *root = _parser->ParseScript(&code);
			TreeNode *body = root->GetChild(TreeIndexCode)->GetChild(TreeIndexSubBody);
			assertEquals(1, body->ChildCount());

			TreeNode *expr = body->GetChild(0);

			assertEquals(Raple::ntVarDeclaration, expr->GetType());
			assertEquals(2, expr->ChildCount());
			assertEquals(Raple::ntIdentifier, expr->GetChild(0)->GetType());
			assertEquals(Raple::ntArrayInitList, expr->GetChild(1)->GetType());
			assertEquals(3, expr->GetChild(1)->ChildCount());

			delete root;
		}

		void CorrectlyParseEmptyArrayInitialization()
		{
			SourceCode code("sub main(){var a={};}");

			TreeNode *root = _parser->ParseScript(&code);
			TreeNode *body = root->GetChild(TreeIndexCode)->GetChild(TreeIndexSubBody);
			assertEquals(1, body->ChildCount());

			TreeNode *expr = body->GetChild(0);
			assertEquals(Raple::ntVarDeclaration, expr->GetType());
			assertEquals(2, expr->ChildCount());
			assertEquals(Raple::ntIdentifier, expr->GetChild(0)->GetType());
			assertEquals(Raple::ntArrayInitList, expr->GetChild(1)->GetType());
			assertEquals(0, expr->GetChild(1)->ChildCount());

			delete root;
		}

		void CorrectlyParseMultiplyCommasInInitializationList()
		{
			SourceCode code("sub main(){var a={'one',,2,,,'three'};}");

			TreeNode *root = _parser->ParseScript(&code);
			TreeNode *body = root->GetChild(TreeIndexCode)->GetChild(TreeIndexSubBody);
			assertEquals(1, body->ChildCount());

			TreeNode *expr = body->GetChild(0);
			assertEquals(Raple::ntVarDeclaration, expr->GetType());
			assertEquals(2, expr->ChildCount());
			assertEquals(Raple::ntIdentifier, expr->GetChild(0)->GetType());
			assertEquals(Raple::ntArrayInitList, expr->GetChild(1)->GetType());
			assertEquals(3, expr->GetChild(1)->ChildCount());

			delete root;
		}

		void CorrectlyParseWhileCycle()
		{
			SourceCode code("sub main(){while(1<2){a=2;b=3;}}");

			TreeNode *root = _parser->ParseScript(&code);
			TreeNode *body = root->GetChild(TreeIndexCode)->GetChild(TreeIndexSubBody);
			assertEquals(1, body->ChildCount());

			TreeNode *whileExpr = body->GetChild(0);
			assertEquals(Raple::ntWhile, whileExpr->GetType());
			assertEquals(Raple::ntBinaryOperator, whileExpr->GetChild(0)->GetType());
			assertEquals(Raple::ntStatementsBlock, whileExpr->GetChild(1)->GetType());

			delete root;
		}

		void CorrectlyParseInstanceFunctionCalling()
		{
			SourceCode code("sub main(a){a.len();}");

			TreeNode *root = _parser->ParseScript(&code);
			TreeNode *body = root->GetChild(TreeIndexCode)->GetChild(TreeIndexSubBody);
			assertEquals(1, body->ChildCount());

			TreeNode *call = body->GetChild(0);
			assertEquals(Raple::ntFunctionCall, call->GetType());
			assertEquals(2, call->ChildCount());
			assertEquals(Raple::ntIdentifier, call->GetChild(0)->GetType());
			assertEquals(Raple::ntFunctionCall, call->GetChild(1)->GetType());

			TreeNode *internalCall = call->GetChild(1);
			assertEquals(2, internalCall->ChildCount());
			assertEquals(Raple::ntIdentifier, internalCall->GetChild(0)->GetType());
			assertEquals(Raple::ntFunctionCallArgList, internalCall->GetChild(1)->GetType());

			delete root;
		}

		void CorrectlyParseReturnArrayList()
		{
			SourceCode code("sub main(){return {1,2};}");

			TreeNode *root = _parser->ParseScript(&code);
			TreeNode *body = root->GetChild(TreeIndexCode)->GetChild(TreeIndexSubBody);
			assertEquals(1, body->ChildCount());

			TreeNode *returnNode = body->GetChild(0);
			assertEquals(Raple::ntReturn, returnNode->GetType());
			assertEquals(1, returnNode->ChildCount());
			assertEquals(Raple::ntArrayInitList, returnNode->GetChild(0)->GetType());
		}

		void CorrectlyParseTrueFalseContants()
		{
			SourceCode code("sub main(){return true;}");

			TreeNode *root = _parser->ParseScript(&code);
			TreeNode *body = root->GetChild(TreeIndexCode)->GetChild(TreeIndexSubBody);
			assertEquals(1, body->ChildCount());

			TreeNode *retNode = body->GetChild(0);
			assertEquals(Raple::ntReturn, retNode->GetType());
			assertEquals(1, retNode->ChildCount());
			assertEquals(Raple::ntConstant, retNode->GetChild(0)->GetType());
			assertEquals(Raple::ttTrue, retNode->GetChild(0)->GetToken()->Type);
		}

		void CorrectlyParseForEach()
		{
			SourceCode code("sub main(){ foreach(var x in {1,2,3}) {} }");

			TreeNode *root = _parser->ParseScript(&code);
			TreeNode *body = root->GetChild(TreeIndexCode)->GetChild(TreeIndexSubBody);
			assertEquals(1, body->ChildCount());

			TreeNode *foreachNode = body->GetChild(0);
			assertEquals(Raple::ntForEach, foreachNode->GetType());
			assertEquals(3, foreachNode->ChildCount());
			assertEquals(Raple::ntVarDeclaration, foreachNode->GetChild(0)->GetType());
			assertEquals(Raple::ntArrayInitList, foreachNode->GetChild(1)->GetType());
			assertEquals(Raple::ntStatementsBlock, foreachNode->GetChild(2)->GetType());
		}

		void CorrectlyParseRange()
		{
			SourceCode code("sub main(){ foreach(var x in {1 -> 2}) {} }");

			TreeNode *root = _parser->ParseScript(&code);
			TreeNode *body = root->GetChild(TreeIndexCode)->GetChild(TreeIndexSubBody);
			assertEquals(1, body->ChildCount());

			TreeNode *foreachNode = body->GetChild(0);
			assertEquals(Raple::ntForEach, foreachNode->GetType());
			assertEquals(3, foreachNode->ChildCount());
			
			TreeNode *range = foreachNode->GetChild(1);
			assertEquals(Raple::ntRange, range->GetType());
			assertEquals(2, range->ChildCount());
		}


		Parser *_parser;
		Tokenizer *_tokenizer;
		FakeLoggerForParserTests *_logger;
	};
}

#endif //__PARSER_TESTS_H_INCLUDED__