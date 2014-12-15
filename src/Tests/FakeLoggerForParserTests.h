#ifndef __FAKE_LOGGER_FOR_PARSER_TESTS_H_INCLUDED__
#define __FAKE_LOGGER_FOR_PARSER_TESTS_H_INCLUDED__


//-------------------------------------
#include "../Raple/Headers/ILogger.h"
//-------------------------------------
using Raple::ILogger;
using Raple::rstring;
//-------------------------------------


namespace RapleTestsHelpers
{
	class FakeLoggerForParserTests : public ILogger
	{
	public:

		FakeLoggerForParserTests()
		{
			IsMessageLogged = false;
			Message = "";
		}

		virtual ~FakeLoggerForParserTests()
		{
		}

		virtual void Log(const rstring &message) const
		{
			catchMessage(message);
		}

		virtual void Error(const rstring &message, int row = 0) const
		{
			catchMessage(message);
		}

		virtual void Warning(const rstring &message) const
		{
			catchMessage(message);
		}

		virtual void Warning(const rstring &title, const rstring &message) const
		{
			catchMessage(message);
		}

		virtual void Error(const rstring &title, const rstring &message, int row = 0) const
		{
			catchMessage(message);
		}

		mutable bool IsMessageLogged;
		mutable rstring Message;

	private:

		void catchMessage(const rstring &message) const
		{
			IsMessageLogged = true;
			Message = message;
		}
	};
}


#endif //__FAKE_LOGGER_FOR_PARSER_TESTS_H_INCLUDED__