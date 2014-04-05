#ifndef __FAKE_LOGGER_FOR_COMPILER_TESTS_H_INCLUDED__
#define __FAKE_LOGGER_FOR_COMPILER_TESTS_H_INCLUDED__


//-------------------------------------
#include "../Raple/Headers/ILogger.h"
//-------------------------------------
using Raple::ILogger;
//-------------------------------------

namespace RapleTestsHelpers
{
	class FakeLoggerForCompilerTests : public ILogger
	{
	public:

		FakeLoggerForCompilerTests()
		{
			IsErrorLogged = false;
			ErrorMessage = "";
		}

		~FakeLoggerForCompilerTests()
		{
		}

		virtual void Log(const rstring &message) const
		{
		}

		virtual void Error(const rstring &message) const
		{
		}

		virtual void Warning(const rstring &message) const
		{
		}

		virtual void Warning(const rstring &title, const rstring &message) const
		{
		}

		virtual void Error(const rstring &title, const rstring &message) const
		{
			IsErrorLogged = true;
			ErrorMessage = message;
		}

		mutable bool IsErrorLogged;
		mutable rstring ErrorMessage;
	};
}


#endif //__FAKE_LOGGER_FOR_COMPILER_TESTS_INCLUDED__