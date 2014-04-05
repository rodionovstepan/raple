#ifndef __TOKENIZER_H_INCLUDED__
#define __TOKENIZER_H_INCLUDED__


//-------------------------------------
#include "ITokenizer.h"
//-------------------------------------


namespace Raple
{
	class Tokenizer : public ITokenizer
	{
		void parseToken();

		bool isWhiteSpace();
		bool isComment();
		bool isConstant();
		bool isIdentifier();
		bool isKeyWord();

		const char *_source;
		int _length;
		int _tokenLength;
		TokenType _tokenType;

	public:

		virtual TokenType GetTokenType(const char *source, int length, int *tokenLength);
	};
}

#endif //__TOKENIZER_H_INCLUDED__