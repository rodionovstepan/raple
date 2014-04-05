#ifndef __I_RUNTIME_ENVIRONMENT_H_INCLUDED__
#define __I_RUNTIME_ENVIRONMENT_H_INCLUDED__


//-------------------------------------
#include "String.h"
#include "Sub.h"
#include "IStaticLibrary.h"
//-------------------------------------


namespace Raple
{
	class IRuntimeEnvironment
	{
	public:

		virtual ~IRuntimeEnvironment()
		{
		}

		virtual int AddSub(const rstring &name) = 0;
		virtual int AddAnonymousSub() = 0;
		virtual Sub *GetSub(int index) = 0;
		virtual int FindSub(const rstring &name) = 0;
		
		virtual bool IsLibraryExists(const rstring &name) = 0;
		virtual bool IsLibraryImported(const rstring &name) = 0;
		virtual void ImportLibrary(const rstring &name) = 0;
		virtual Sub *GetLibrarySub(unsigned int library, unsigned int sub) = 0;

		virtual void RegisterNativeFunction(const rstring &name, int argumentCount, NativeFunction nf) = 0;
		virtual void RegisterStaticLibrary(const IStaticLibrary *library) = 0;
	};
}


#endif //__I_RUNTIME_ENVIRONMENT_H_INCLUDED__