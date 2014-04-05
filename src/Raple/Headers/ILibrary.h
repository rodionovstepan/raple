#ifndef __I_LIBRARY_H_INCLUDED__
#define __I_LIBRARY_H_INCLUDED__


//-------------------------------------
#include "IRuntimeEnvironment.h"
//-------------------------------------


namespace Raple
{
	class ILibrary
	{
	public:

		virtual ~ILibrary()
		{
		}

		virtual void Register(IRuntimeEnvironment *environment) const = 0;
	};
}



#endif //__I_LIBRARY_H_INCLUDED__