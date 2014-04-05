#ifndef __SYSTEM_H_INCLUDED__
#define __SYSTEM_H_INCLUDED__


//-------------------------------------
#include <assert.h>
//-------------------------------------
#define _RAPLE_VERSION	""
#define _RAPLE_DATE		""
//-------------------------------------

#define _RAPLE_DEBUG

#ifdef _RAPLE_DEBUG
	#define RapleAssert(expression) StandartRapleAssert(expression)
#else
	#define RapleAssert(expression)
#endif


#define StandartRapleAssert(expression) assert(expression)

#define rhash unsigned int

namespace Raple
{
}



#endif //__SYSTEM_H_INCLUDED__