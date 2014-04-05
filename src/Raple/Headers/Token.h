#ifndef __TOKEN_H_INCLUDED__
#define __TOKEN_H_INCLUDED__


//-------------------------------------
#include "TokenDefinition.h"
//-------------------------------------


namespace Raple
{
	struct Token
	{
		TokenType Type;
		int Position;
		int Length;

		Token()
		{
			Type = ttUnknown;
			Position = 0;
			Length = 0;
		}

		Token(TokenType tokenType, int position, int length)
		{
			Type = tokenType;
			Position = position;
			Length = length;
		}
	};
}


#endif //__TOKEN_H_INCLUDED__