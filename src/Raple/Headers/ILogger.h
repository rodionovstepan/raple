#ifndef __I_LOGGER_H_INCLUDED__
#define __I_LOGGER_H_INCLUDED__


//-------------------------------------
#include "String.h"
//-------------------------------------


namespace Raple
{
	class ILogger
	{
	public:

		virtual ~ILogger()
		{
		}

		virtual void Log(const rstring &message) const = 0;
		virtual void Warning(const rstring &title, const rstring &message) const = 0;
		virtual void Warning(const rstring &message) const = 0;
		virtual void Error(const rstring &title, const rstring &message) const = 0;
		virtual void Error(const rstring &message) const = 0;
	};
}


#endif //__I_LOGGER_H_INCLUDED__