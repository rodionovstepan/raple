#ifndef __TOKENIZER_TESTS_H_INCLUDED__
#define __TOKENIZER_TESTS_H_INCLUDED__


//-------------------------------------
#include "../CppTest/cpptest.h"
#include "../Raple/Headers/Tokenizer.h"
#include "../Raple/Headers/String.h"
#include "../Raple/Headers/TokenDefinition.h"
#include "../Raple/Sources/Tokenizer.cpp"
//-------------------------------------
#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif
//-------------------------------------
using Raple::Tokenizer;
using Raple::String;
//-------------------------------------


namespace RapleTests
{
	class TokenizerTests : public Test::Suite
	{
	public:

		TokenizerTests()
		{
			addTest(TokenizerTests::ReturnsEndOfFileTokenTypeIfTextIsEmpty);
			addTest(TokenizerTests::ReturnsWhiteSpaceTokenType);
			addTest(TokenizerTests::ReturnsCommentTokenType);
			addTest(TokenizerTests::ReturnsCommentTokenType2);
			addTest(TokenizerTests::ReturnsDecimalConstTokenType);
			addTest(TokenizerTests::ReturnsHexadecimalConstTokenType);
			addTest(TokenizerTests::ReturnsBinaryConstTokenType);
			addTest(TokenizerTests::ReturnsFloatConstTokenType);
			addTest(TokenizerTests::ReturnsStringConstTokenType);
			addTest(TokenizerTests::ReturnsIdentifierTokenType);
			addTest(TokenizerTests::ReturnsKeywordTokenType);
			addTest(TokenizerTests::FullTokenizerTest);
			addTest(TokenizerTests::AlternativeOpenAndCloseStatementBlock);
			addTest(TokenizerTests::EqualityOperatorsTest);

			addTest(TokenizerTests::StringFormatTest);
		}

	private:

		void StringFormatTest()
		{
			String s;
			s.Format("str.fmt.tst %d", 21);

			assertTrue(s == "str.fmt.tst 21");
		}

		void ReturnsEndOfFileTokenTypeIfTextIsEmpty()
		{
			assertEquals(Raple::ttEndOfFile, _tokenizer.GetTokenType("", 0, NULL))
		}

		void ReturnsWhiteSpaceTokenType()
		{
			int tokenLength;

			Raple::TokenType tokenType = _tokenizer.GetTokenType("   ", 3, &tokenLength);

			assertEquals(3, tokenLength);
			assertEquals(Raple::ttWhitespace, tokenType);
		}

		void ReturnsCommentTokenType()
		{
			int tokenLength;

			Raple::TokenType tokenType = _tokenizer.GetTokenType("// comment", 10, &tokenLength);

			assertEquals(10, tokenLength);
			assertEquals(Raple::ttComment, tokenType);
		}

		void ReturnsCommentTokenType2()
		{
			int tokenLength;

			Raple::TokenType tokenType = _tokenizer.GetTokenType("# comment", 9, &tokenLength);

			assertEquals(9, tokenLength);
			assertEquals(Raple::ttComment, tokenType);
		}

		void ReturnsDecimalConstTokenType()
		{
			int tokenLength;

			Raple::TokenType tokenType = _tokenizer.GetTokenType("123", 3, &tokenLength);

			assertEquals(3, tokenLength);
			assertEquals(Raple::ttDecConst, tokenType);
		}

		void ReturnsHexadecimalConstTokenType()
		{
			int tokenLength;

			Raple::TokenType tokenType = _tokenizer.GetTokenType("0hFF", 4, &tokenLength);

			assertEquals(4, tokenLength);
			assertEquals(Raple::ttHexConst, tokenType);
		}

		void ReturnsBinaryConstTokenType()
		{
			int tokenLength;

			Raple::TokenType tokenType = _tokenizer.GetTokenType("0b101010", 8, &tokenLength);

			assertEquals(8, tokenLength);
			assertEquals(Raple::ttBinConst, tokenType);
		}

		void ReturnsFloatConstTokenType()
		{
			int tokenLength;

			Raple::TokenType tokenType = _tokenizer.GetTokenType("123.45", 6, &tokenLength);

			assertEquals(6, tokenLength);
			assertEquals(Raple::ttFloatConst, tokenType);
		}

		void ReturnsStringConstTokenType()
		{
			int tokenLength;

			Raple::TokenType tokenType = _tokenizer.GetTokenType("\"is string\"", 11, &tokenLength);

			assertEquals(11, tokenLength);
			assertEquals(Raple::ttString, tokenType);
		}

		void ReturnsIdentifierTokenType()
		{
			int tokenLength;

			Raple::TokenType tokenType = _tokenizer.GetTokenType("identifier", 10, &tokenLength);

			assertEquals(10, tokenLength);
			assertEquals(Raple::ttIdentifier, tokenType);

			tokenType = _tokenizer.GetTokenType("_subident", 9, &tokenLength);

			assertEquals(9, tokenLength);
			assertEquals(Raple::ttIdentifier, tokenType);

			tokenType = _tokenizer.GetTokenType("var123", 6, &tokenLength);

			assertEquals(6, tokenLength);
			assertEquals(Raple::ttIdentifier, tokenType);
		}

		void ReturnsKeywordTokenType()
		{
			int tokenLength;

			Raple::TokenType tokenType = _tokenizer.GetTokenType("sub", 3, &tokenLength);

			assertEquals(3, tokenLength);
			assertEquals(Raple::ttSub, tokenType);

			tokenType = _tokenizer.GetTokenType("return", 6, &tokenLength);

			assertEquals(6, tokenLength);
			assertEquals(Raple::ttReturn, tokenType);
		}

		void FullTokenizerTest()
		{
			Raple::TokenType tokenTypes[11] = {Raple::ttSub, Raple::ttIdentifier, Raple::ttOpenParanthesis,
											   Raple::ttIdentifier, Raple::ttCloseParanthesis, Raple::ttOpenStatementBlock,
											   Raple::ttReturn, Raple::ttHexConst, Raple::ttEndOfStatement,
											   Raple::ttCloseStatementBlock, Raple::ttEndOfFile};

			int tokenLengths[11] = {3, 4, 1, 4, 1, 1, 6, 4, 1, 1, 0};

			const char *source = "sub main(args) { return 0hFF; }";

			int tokenLength;
			int position = 0;
			Raple::TokenType tokenType;

			for (int i = 0; i < 11; ++i)
			{
				do
				{
					tokenType = _tokenizer.GetTokenType(source + position, strlen(source) - position, &tokenLength);
					position += tokenLength;
				}
				while (tokenType == Raple::ttWhitespace ||
					   tokenType == Raple::ttComment);

				assertEquals(tokenTypes[i], tokenType);
				assertEquals(tokenLengths[i], tokenLength);
			}
		}

		void AlternativeOpenAndCloseStatementBlock()
		{
			const char *source = "sub main()\n"\
								 "do\n"\
								 "   \n"\
								 "end";

			Raple::TokenType expected[6] = 
											{
												Raple::ttSub, Raple::ttIdentifier, 
												Raple::ttOpenParanthesis, Raple::ttCloseParanthesis, 
												Raple::ttOpenStatementBlock, Raple::ttCloseStatementBlock
											};

			Raple::TokenType tokenType;
			int tokenLength;
			int position = 0;

			for (int i = 0; i < 6; ++i)
			{
				do
				{
					tokenType = _tokenizer.GetTokenType(source + position, strlen(source) - position, &tokenLength);
					position += tokenLength;
				}
				while (tokenType == Raple::ttWhitespace ||
					   tokenType == Raple::ttComment);

				assertEquals(expected[i], tokenType);
			}
		}

		void EqualityOperatorsTest()
		{
			int tlen;
			Raple::TokenType tt = _tokenizer.GetTokenType(">=", 2, &tlen);

			assertEquals(Raple::ttGreaterEqual, tt);
			assertEquals(2, tlen);

			tt = _tokenizer.GetTokenType("<", 1, &tlen);

			assertEquals(Raple::ttLower, tt);
			assertEquals(1, tlen);
		}

		Tokenizer _tokenizer;
	};
}

#endif //__TOKENIZER_TESTS_H_INCLUDED__