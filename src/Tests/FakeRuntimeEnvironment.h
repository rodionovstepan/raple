#ifndef __FAKE_RUNTIME_ENVIRONMENT_H_INCLUDED__
#define __FAKE_RUNTIME_ENVIRONMENT_H_INCLUDED__


//-------------------------------------
#include "../Raple/Headers/IRuntimeEnvironment.h"
#include "../Raple/Headers/IStaticLibrary.h"
//-------------------------------------
using Raple::IRuntimeEnvironment;
using Raple::rstring;
using Raple::Sub;
using Raple::NativeFunction;
using Raple::IStaticLibrary;
//-------------------------------------


namespace RapleTestsHelpers
{
	class FakeRuntimeEnvironment : public IRuntimeEnvironment
	{
	public:

		FakeRuntimeEnvironment()
		{
			_main = 0;
			_bar = 0;
			_foo = new Sub("foo", 1);
			_sub1 = new Sub("sub1", 1);
			_sub2 = new Sub("sub2", 1);
			_libSub = new Sub("libsub", 0);
			_lsimported = false;
		}

		~FakeRuntimeEnvironment()
		{
			delete _foo;

			if (_main != 0)
				delete _main;

			if (_bar != 0)
				delete _bar;

			delete _sub1;
			delete _sub2;
			delete _libSub;
		}

		virtual int AddSub(const rstring &name)
		{
			if (name == "main")
			{
				if (_main != 0)
					delete _main;

				_main = new Sub(name);
				return 0;
			}
			
			if (_bar != 0)
				delete _bar;

			_bar = new Sub(name);
			return 1;
		}

		virtual int AddAnonymousSub()
		{
			return AddSub("anonym");
		}

		virtual Sub *GetSub(int index)
		{
			if (index == 21)
				return _foo;
			else if (index == 1)
				return _bar;
			else if (index == 34)
				return _sub1;
			else if (index == 35)
				return _sub2;

			return _main;
		}

		virtual int FindSub(const rstring &name)
		{
			if (name == "foo")
				return 21;
			else if (name == "bar")
				return 1;
			else if (name == "sub1")
				return 34;
			else if (name == "sub2")
				return 35;

			return -1;
		}

		virtual bool IsLibraryExists(const rstring &name)
		{
			return true;
		}

		virtual bool IsLibraryImported(const rstring &name)
		{
			if (name == "foo")
				return _lsimported;
			
			return false;
		}

		virtual void ImportLibrary(const rstring &name)
		{
			if (name == "foo")
				_lsimported = true;
		}

		virtual Sub *GetLibrarySub(unsigned int library, unsigned int sub)
		{
			if (_lsimported)
				return _libSub;

			return 0;
		}

		virtual void RegisterStaticLibrary(const IStaticLibrary *library)
		{
		}

		virtual void RegisterNativeFunction(const rstring &name, int argumentCount, NativeFunction nf)
		{
		}

	private:

		Sub *_main;
		Sub *_bar;
		Sub *_foo;

		Sub *_sub1;
		Sub *_sub2;

		Sub *_libSub;
		bool _lsimported;
	};
}



#endif //__FAKE_RUNTIME_ENVIRONMENT_H_INCLUDED__