#ifndef __I_VIRTUAL_MACHINE_H_INCLUDED__
#define __I_VIRTUAL_MACHINE_H_INCLUDED__


//-------------------------------------
#include "IRuntimeEnvironment.h"
#include "VirtualMachineResult.h"
#include "Sub.h"
#include "Var.h"
//-------------------------------------


namespace Raple
{
	class IVirtualMachine
	{
	public:

		virtual ~IVirtualMachine()
		{
		}

		virtual IRuntimeEnvironment *GetEnvironment() = 0;
		virtual VirtualMachineResult Execute(Sub *sub) = 0;

		virtual Var *Pop() = 0;
		virtual void PushInt(int i) = 0;
		virtual void PushFloat(float f) = 0;
		virtual void PushString(const rstring &s) = 0;
		virtual void PushArrayPointer(const HashArray *array) = 0;
		virtual void PushNull() = 0;
		virtual void PushFunctionPointer(int id) = 0;
	};
}



#endif //__I_VIRTUAL_MACHINE_H_INCLUDED__