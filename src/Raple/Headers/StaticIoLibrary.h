#ifndef __STATIC_IO_LIBRARY_H_INCLUDED__
#define __STATIC_IO_LIBRARY_H_INCLUDED__


//-------------------------------------
#include "StaticLibraryBase.h"
//-------------------------------------
#include <stdlib.h>
//-------------------------------------
using namespace Raple;
//-------------------------------------


namespace RapleLibraries
{
	class StaticIoLibrary : public StaticLibraryBase
	{
	public:

		StaticIoLibrary();
		~StaticIoLibrary();

		virtual rstring GetName() const
		{
			return "io";
		}

	private:

		static int io_read(IVirtualMachine *vm);
		static int io_print(IVirtualMachine *vm);
		static int io_println(IVirtualMachine *vm);
		static int io_ls(IVirtualMachine *vm);

		static int io_remove(IVirtualMachine *vm);
		static int io_copy(IVirtualMachine *vm);
		static int io_fexists(IVirtualMachine *vm);
		static int io_dexists(IVirtualMachine *vm);
		static int io_getfname(IVirtualMachine *vm);
		static int io_getfext(IVirtualMachine *vm);

		//----
		static void print_var(Var *var);
	};
};



#endif //__STATIC_IO_LIBRARY_H_INCLUDED__