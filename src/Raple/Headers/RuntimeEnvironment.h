#ifndef __RUNTIME_ENVIRONMENT_H_INCLUDED__
#define __RUNTIME_ENVIRONMENT_H_INCLUDED__


//-------------------------------------
#include "IRuntimeEnvironment.h"
//-------------------------------------
#include <vector>
#include <map>
//-------------------------------------


namespace Raple
{
	class RuntimeEnvironment : public IRuntimeEnvironment
	{
	public:

		~RuntimeEnvironment();

		virtual int AddSub(const rstring &name);
		virtual int AddAnonymousSub();
		virtual Sub *GetSub(int index);
		virtual int FindSub(const rstring &name);

		virtual bool IsLibraryExists(const rstring &name);
		virtual bool IsLibraryImported(const rstring &name);
		virtual void ImportLibrary(const rstring &name);
		virtual Sub *GetLibrarySub(unsigned int library, unsigned int sub);

		virtual void RegisterNativeFunction(const rstring &name, int argumentCount, NativeFunction nf);
		virtual void RegisterStaticLibrary(const IStaticLibrary *library);

	private:

		std::vector<Sub *> _subs;
		std::map<unsigned int, IStaticLibrary *> _knownLibs;
		std::map<unsigned int, IStaticLibrary *> _importedLibs;
	};
}



#endif //__RUNTIME_ENVIRONMENT_H_INCLUDED__