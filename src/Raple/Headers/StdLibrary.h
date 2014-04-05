#ifndef __STD_LIBRARY_H_INCLUDED__
#define __STD_LIBRARY_H_INCLUDED__


//-------------------------------------
#include "ILibrary.h"
#include "IVirtualMachine.h"
#include "System.h"
//-------------------------------------
#include <iostream>
//-------------------------------------
using namespace Raple;
//-------------------------------------


namespace RapleLibraries
{
	class StdLibrary : public ILibrary
	{
	public:

		~StdLibrary()
		{
		}

		void Register(IRuntimeEnvironment *environment) const
		{
		}
	};
}




#endif //__STD_LIBRARY_H_INCLUDED__