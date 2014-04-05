#ifndef __I_PARSER_H_INCLUDED__
#define __I_PARSER_H_INCLUDED__


//-------------------------------------
#include "TreeNode.h"
#include "SourceCode.h"
//-------------------------------------


namespace Raple
{
	class IParser
	{
	public:

		virtual ~IParser()
		{
		}

		virtual TreeNode *ParseScript(const SourceCode *sourceCode) = 0;
	};
}


#endif //__I_PARSER_H_INCLUDED__