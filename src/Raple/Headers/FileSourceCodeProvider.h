#ifndef __FILE_SOURCE_CODE_PROVIDER_H_INCLUDED__
#define __FILE_SOURCE_CODE_PROVIDER_H_INCLUDED__


//-------------------------------------
#include "ISourceCodeProvider.h"
//-------------------------------------


namespace Raple
{
	class FileSourceCodeProvider : public ISourceCodeProvider
	{
		SourceCode *_sourceCode;

	public:

		FileSourceCodeProvider(const char *fname);
		~FileSourceCodeProvider()
		{
		}

		virtual SourceCode *GetSourceCode() const;

	private:

		bool loadFromFile(const char *fname);
	};
}


#endif //__FILE_SOURCE_CODE_PROVIDER_H_INCLUDED__