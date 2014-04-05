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

		FileSourceCodeProvider(int argc, char **argv);
		~FileSourceCodeProvider()
		{
		}

		virtual SourceCode *GetSourceCode() const;

	private:

		bool loadFromFile(int argc, char **argv);
	};
}


#endif //__FILE_SOURCE_CODE_PROVIDER_H_INCLUDED__