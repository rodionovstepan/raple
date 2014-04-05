#ifndef __CALL_STACK_FRAME_H_INCLUDED__
#define __CALL_STACK_FRAME_H_INCLUDED__


//-------------------------------------
#include "Sub.h"
#include "Var.h"
//-------------------------------------


namespace Raple
{
	class CallStackFrame
	{
	public:

		Sub *Sub;
		int Pointer;
		bool ReturnValueExpected;
		Var *Stackbase;
		Var *Stackptr;
	};
}


#endif //__CALL_STACK_FRAME_H_INCLUDED__