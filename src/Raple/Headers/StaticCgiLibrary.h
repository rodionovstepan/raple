#ifndef __STATIC_CGI_LIBRARY_H_INCLUDED__
#define __STATIC_CGI_LIBRARY_H_INCLUDED__


//-------------------------------------
#include "StaticLibraryBase.h"
//-------------------------------------
#include <stdlib.h>
//-------------------------------------
using namespace Raple;
//-------------------------------------


namespace RapleLibraries
{
	class StaticCgiLibrary : public StaticLibraryBase
	{
	public:
		StaticCgiLibrary();
		~StaticCgiLibrary();

		virtual rstring GetName() const
		{
			return "cgi";
		}

	private:

		static int cgi_host(IVirtualMachine *vm);
		static int cgi_html(IVirtualMachine *vm);
		static int cgi_beginForm(IVirtualMachine *vm);
		static int cgi_endForm(IVirtualMachine *vm);
		static int cgi_submit(IVirtualMachine *vm);
		static int cgi_input(IVirtualMachine *vm);
		static int cgi_hidden(IVirtualMachine *vm);
		static int cgi_get(IVirtualMachine *vm);
		static int cgi_href(IVirtualMachine *vm);
	};
}


#endif //__STATIC_CGI_LIBRARY_H_INCLUDED__