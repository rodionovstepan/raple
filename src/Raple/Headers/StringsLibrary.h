#ifndef __STRINGS_LIBRARY_H_INCLUDED__
#define __STRINGS_LIBRARY_H_INCLUDED__


//-------------------------------------
#include "ILibrary.h"
#include "IVirtualMachine.h"
//-------------------------------------
#include <string.h>
//-------------------------------------
using namespace Raple;
//-------------------------------------


namespace RapleLibraries
{
	class StringsLibrary : public ILibrary
	{
		static int strlib_strlen(IVirtualMachine *vm)
		{
			Var *frame = vm->Pop();
			
			if (frame->GetDataType() == dtString)
			{
				rstring *s = frame->String();
				if (s != 0)
					vm->PushInt(strlen(s->GetBuffer()));
				else
					vm->PushInt(0);
			}
			else
				vm->PushInt(0);

			return 0;
		}

	public:

		~StringsLibrary()
		{
		}

		void Register(IRuntimeEnvironment *environment) const
		{
			environment->RegisterNativeFunction("strlen", 1, StringsLibrary::strlib_strlen);
		}
	};
}


#endif //__STRINGS_LIBRARY_H_INCLUDED__