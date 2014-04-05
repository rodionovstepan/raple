#ifndef __TOKEN_DEFINITION_H_INCLUDED__
#define __TOKEN_DEFINITION_H_INCLUDED__


//-------------------------------------
//-------------------------------------


namespace Raple
{
	enum TokenType
	{
		ttUnknown,

		ttEndOfFile,

		ttWhitespace,

		ttComment,

		ttIdentifier,

		ttHexConst,				// eg, 0h21
		ttBinConst,				// eg, 0b11001
		ttDecConst,				// eg, 21
		ttFloatConst,			// eg, 21.4
		ttString,				// eg, "Hello", 'Hello'
		ttNonTerminatedString,	// eg, "Hello

		ttPlus,					// +
		ttMinus,				// -
		ttStar,					// *
		ttSlash,				// /
		ttExponentiation,		// **
		ttIncrement,			// ++
		ttDecrement,			// --

		ttAssignment,			// =

		ttEqual,				// ==, <=>
		ttNotEqual,				// !=, <!>
		ttGreater,				// >
		ttLower,				// <
		ttGreaterEqual,			// >=
		ttLowerEqual,			// <=

		ttAnd,					// &&, and
		ttOr,					// ||, or
		
		ttEndOfStatement,		// ;
		ttDot,					// .
		ttComma,				// ,

		ttOpenStatementBlock,	// {
		ttCloseStatementBlock,	// }
		ttOpenParanthesis,		// (
		ttCloseParanthesis,		// )
		ttOpenBracket,			// [
		ttCloseBracket,			// ]

		ttSub,					// sub
		ttVar,					// var
		ttIf,					// if
		ttElse,					// else
		ttReturn,				// return
		ttWhile,				// while
		ttImport				// import
	};

	struct TokenWord
	{
		const char *word;
		TokenType type;
	};

	TokenWord const TokenWords[] = 
	{
		{ "+", ttPlus },
		{ "-", ttMinus },
		{ "*", ttStar },
		{ "/", ttSlash },
		{ "**", ttExponentiation },
		{ "++", ttIncrement },
		{ "--", ttDecrement },

		{ "=", ttAssignment },
		{ "<-", ttAssignment },
		{ ";", ttEndOfStatement },
		{ ",", ttComma },
		{ ".", ttDot },

		{ "==", ttEqual },
		{ "<=>", ttEqual },
		{ "!=", ttNotEqual },
		{ "<!>", ttNotEqual },
		{ ">", ttGreater },
		{ "<", ttLower },
		{ ">=", ttGreaterEqual },
		{ "<=", ttLowerEqual },

		{ "&&", ttAnd },
		{ "||", ttOr },

		{ "{", ttOpenStatementBlock },
		{ "}", ttCloseStatementBlock },
		{ "(", ttOpenParanthesis },
		{ ")", ttCloseParanthesis },
		{ "[", ttOpenBracket },
		{ "]", ttCloseBracket },

		{ "sub", ttSub },
		{ "var", ttVar },
		{ "if", ttIf },
		{ "else", ttElse },
		{ "return", ttReturn },
		{ "do", ttOpenStatementBlock },
		{ "end", ttCloseStatementBlock },
		{ "and", ttAnd },
		{ "or", ttOr },
		{ "while", ttWhile },
		{ "import", ttImport }
	};

	const int NumberOfTokenWords = sizeof(TokenWords) / sizeof(TokenWord);

	const char * const WhiteSpace = " \t\r\n";

	const int WhiteSpaceLength = 4;

	const char *GetTokenDefinition(TokenType tokenType);
}


#endif //__TOKEN_DEFINITION_H_INCLUDED__