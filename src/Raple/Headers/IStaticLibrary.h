#ifndef __I_STATIC_LIBRARY_H_INCLUDED__
#define __I_STATIC_LIBRARY_H_INCLUDED__


//-------------------------------------
#include "String.h"
#include "System.h"
//-------------------------------------
//-------------------------------------


namespace Raple
{
	class Sub;

	class IStaticLibrary
	{
	public:
		virtual ~IStaticLibrary()
		{
		}

		virtual rstring GetName() const = 0;
		virtual Sub *GetSub(rhash hash) = 0;
	};
}



#endif //__I_STATIC_LIBRARY_H_INCLUDED__