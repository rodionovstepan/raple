#ifndef __I_TOKENIZER_H_INCLUDED__
#define __I_TOKENIZER_H_INCLUDED__


//-------------------------------------
#include "TokenDefinition.h"
//-------------------------------------


namespace Raple
{
	class ITokenizer
	{
	public:

		virtual ~ITokenizer()
		{
		}

		virtual TokenType GetTokenType(const char *source, int length, int *tokenLength) = 0;
	};
}

#endif //__I_TOKENIZER_H_INCLUDED__