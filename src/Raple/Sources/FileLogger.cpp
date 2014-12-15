//-------------------------------------
#include "../Headers/FileLogger.h"
//-------------------------------------
#include <stdio.h>
//-------------------------------------

namespace Raple
{
	void FileLogger::Log(const rstring &message) const
	{
		fprintf((FILE *)_logFile, "%s\n", message.GetBuffer());
	}

	void FileLogger::Warning(const rstring &message) const
	{
		fprintf((FILE *)_logFile, "Warning> %s\n", message.GetBuffer());
	}

	void FileLogger::Error(const rstring &message, int row) const
	{
		if (row > 0)
			fprintf((FILE *)_logFile, "Error> %s on line %d\n", message.GetBuffer(), row);
		else
			fprintf((FILE *)_logFile, "Error> %s\n", message.GetBuffer());
	}

	void FileLogger::Warning(const rstring &title, const rstring &message) const
	{
		fprintf((FILE *)_logFile, "%s> %s\n", title.GetBuffer(), message.GetBuffer());
	}

	void FileLogger::Error(const rstring &title, const rstring &message, int row) const
	{
		if (row > 0)
			fprintf((FILE *)_logFile, "%s> %s on line %d\n", title.GetBuffer(), message.GetBuffer(), row);
		else
			fprintf((FILE *)_logFile, "%s> %s\n", title.GetBuffer(), message.GetBuffer());
	}

	FileLogger::FileLogger()
	{
		fopen_s((FILE **)&_logFile, "raple_log.log", "w+");
	}

	FileLogger::~FileLogger()
	{
		if(_logFile != NULL)
			fclose((FILE *)_logFile);
	}
}