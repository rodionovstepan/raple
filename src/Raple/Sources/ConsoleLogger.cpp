//-------------------------------------
#include "../Headers/ConsoleLogger.h"
//-------------------------------------
#include <stdio.h>
//-------------------------------------

namespace Raple
{
	void ConsoleLogger::Log(const rstring &message) const
	{
		printf("%s\n", message.GetBuffer());
	}

	void ConsoleLogger::Warning(const rstring &message) const
	{
		printf("Warning> %s\n", message.GetBuffer());
	}

	void ConsoleLogger::Error(const rstring &message, int row) const
	{
		if (row > 0)
			printf("Error> %s on line %d\n", message.GetBuffer(), row);
		else
			printf("Error> %s\n", message.GetBuffer());
	}

	void ConsoleLogger::Warning(const rstring &title, const rstring &message) const
	{
		printf("%s> %s\n", title.GetBuffer(), message.GetBuffer());
	}

	void ConsoleLogger::Error(const rstring &title, const rstring &message, int row) const
	{
		if (row > 0)
			printf("%s> %s on line %d\n", title.GetBuffer(), message.GetBuffer(), row);
		else
			printf("%s> %s\n", title.GetBuffer(), message.GetBuffer());
	}
}