#ifndef __I_SOURCE_CODE_PROVIDER_H_INCLUDED__
#define __I_SOURCE_CODE_PROVIDER_H_INCLUDED__


//-------------------------------------
#include "SourceCode.h"
//-------------------------------------


namespace Raple
{
	class ISourceCodeProvider
	{
	public:

		virtual ~ISourceCodeProvider()
		{
		}

		virtual SourceCode *GetSourceCode() const = 0;
	};
}


#endif //__I_SOURCE_CODE_PROVIDER_H_INCLUDED__