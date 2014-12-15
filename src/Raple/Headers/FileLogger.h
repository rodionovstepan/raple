#ifndef __FILE_LOGGER_H_INCLUDED__
#define __FILE_LOGGER_H_INCLUDED__

//-------------------------------------
#include "ILogger.h"
//-------------------------------------

namespace Raple
{
	class FileLogger : public ILogger
	{
		void *_logFile;

	public:

		FileLogger();
		~FileLogger();

		virtual void Log(const rstring &message) const;
		virtual void Warning(const rstring &title, const rstring &message) const;
		virtual void Warning(const rstring &message) const;
		virtual void Error(const rstring &title, const rstring &message, int row = 0) const;
		virtual void Error(const rstring &message, int row = 0) const;
	};
}


#endif //__FILE_LOGGER_H_INCLUDED__