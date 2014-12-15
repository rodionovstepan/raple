//-------------------------------------
#include "../Headers/FileSourceCodeProvider.h"
//-------------------------------------
#include <stdio.h>
//-------------------------------------


namespace Raple
{
	FileSourceCodeProvider::FileSourceCodeProvider(const char *fname)
	{
		if (!loadFromFile(fname))
			_sourceCode = 0;
	}

	SourceCode *FileSourceCodeProvider::GetSourceCode() const
	{
		return _sourceCode;
	}

	bool FileSourceCodeProvider::loadFromFile(const char *fname)
	{
		FILE *input;
		fopen_s(&input, fname, "r");

		if (input == 0)
			return false;

		fseek(input, 0, SEEK_END);
		unsigned int fsize = ftell(input);
		fseek(input, 0, SEEK_SET);

		char *source = new char [fsize+1];
		source[fsize] = 0;

		source[fread(source, sizeof (char), fsize, input)] = 0;
		fclose(input);

		_sourceCode = new SourceCode(source);
		delete [fsize+1] source;

		return true;
	}
}

