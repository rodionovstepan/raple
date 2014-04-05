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

	void ConsoleLogger::Error(const rstring &message) const
	{
		printf("Error> %s\n", message.GetBuffer());
	}

	void ConsoleLogger::Warning(const rstring &title, const rstring &message) const
	{
		printf("%s> %s\n", title.GetBuffer(), message.GetBuffer());
	}

	void ConsoleLogger::Error(const rstring &title, const rstring &message) const
	{
		printf("%s> %s\n", title.GetBuffer(), message.GetBuffer());
	}
}