#ifndef __FAKE_VIRTUAL_MACHINE_H_INCLUDED__
#define __FAKE_VIRTUAL_MACHINE_H_INCLUDED__


//-------------------------------------
#include "../Raple/Headers/IVirtualMachine.h"
#include "../Raple/Headers/HashArray.h"
#include "FakeRuntimeEnvironment.h"
//-------------------------------------
using Raple::IVirtualMachine;
using Raple::HashArray;
//-------------------------------------


namespace RapleTestsHelpers
{
	class FakeVirtualMachine : public IVirtualMachine
	{
	public:

		FakeVirtualMachine()
		{
			_env = new FakeRuntimeEnvironment();
		}

		~FakeVirtualMachine()
		{
			delete _env;
		}

		virtual IRuntimeEnvironment *GetEnvironment()
		{
			return _env;
		}

		virtual Raple::VirtualMachineResult Execute(Sub *sub)
		{
			return Raple::vmrSuccess;
		}

		virtual Raple::Var *Pop()
		{
			return 0;
		}

		virtual void PushInt(int i)
		{
		}

		virtual void PushFloat(float f)
		{
		}

		virtual void PushString(const rstring &s)
		{
		}

		virtual void PushArrayPointer(const HashArray *array)
		{
		}

		virtual void PushNull()
		{
		}

		virtual void PushFunctionPointer(int id)
		{
		}

	private:

		IRuntimeEnvironment *_env;
	};
}


#endif //__FAKE_VIRTUAL_MACHINE_H_INCLUDED__