#ifndef __I_RAPLE_ENGINE_H_INCLUDED__
#define __I_RAPLE_ENGINE_H_INCLUDED__


//-------------------------------------
#include "ISourceCodeProvider.h"
#include "ILibrary.h"
#include "IStaticLibrary.h"
//-------------------------------------


namespace Raple
{
	class IRapleEngine
	{
	public:

		virtual ~IRapleEngine()
		{
		}

		virtual void ExecuteScript(const ISourceCodeProvider *provider) = 0;
		virtual void RegisterLibrary(const ILibrary *registrator) = 0;
		virtual void RegisterStaticLibrary(const IStaticLibrary *library) = 0;
	};
}


#endif //__I_RAPLE_ENGINE_H_INCLUDED__