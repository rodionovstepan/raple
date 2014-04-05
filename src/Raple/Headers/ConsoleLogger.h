#ifndef __CONSOLE_LOGGER_H_INCLUDED__
#define __CONSOLE_LOGGER_H_INCLUDED__

//-------------------------------------
#include "ILogger.h"
//-------------------------------------

namespace Raple
{
	class ConsoleLogger : public ILogger
	{
	public:

		~ConsoleLogger()
		{
		}

		virtual void Log(const rstring &message) const;
		virtual void Warning(const rstring &title, const rstring &message) const;
		virtual void Warning(const rstring &message) const;
		virtual void Error(const rstring &title, const rstring &message) const;
		virtual void Error(const rstring &message) const;
	};
}


#endif //__CONSOLE_LOGGER_H_INCLUDED__