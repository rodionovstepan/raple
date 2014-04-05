#ifndef __VIRTUAL_MACHINE_H_INCLUDED__
#define __VIRTUAL_MACHINE_H_INCLUDED__


//-------------------------------------
#include "IVirtualMachine.h"
#include "ILogger.h"
#include "System.h"
#include "Constants.h"
#include "CallStackFrame.h"
#include "Var.h"
//-------------------------------------
#include <math.h>
//-------------------------------------


namespace Raple
{
	class VirtualMachine : public IVirtualMachine
	{
	public:

		VirtualMachine(IRuntimeEnvironment *environment, ILogger *logger);
		~VirtualMachine();

		virtual IRuntimeEnvironment *GetEnvironment();
		virtual VirtualMachineResult Execute(Sub *sub);

		virtual Var *Pop()
		{
			RapleAssert(stackDistance() > 0);
			return --_stackptr;
		}

		virtual Var *Top()
		{
			RapleAssert(stackDistance() > 0);
			return _stackptr - 1;
		}

		virtual void PushInt(int i) 
		{
			RapleAssert(stackDistance() < Constants::StackSize);
			(_stackptr++)->Int(i);
		}

		virtual void PushFloat(float f)
		{
			RapleAssert(stackDistance() < Constants::StackSize);
			(_stackptr++)->Float(f);
		}

		virtual void PushString(const rstring &s)
		{
			RapleAssert(stackDistance() < Constants::StackSize);
			(_stackptr++)->String(s);
		}

		virtual void PushArrayPointer(const HashArray *array)
		{
			RapleAssert(stackDistance() < Constants::StackSize);
			(_stackptr++)->Array(array);
		}

		virtual void PushFunctionPointer(int id)
		{
			RapleAssert(stackDistance() < Constants::StackSize);
			(_stackptr++)->Function(id);
		}

		virtual void PushNull()
		{
			RapleAssert(stackDistance() < Constants::StackSize);
			(_stackptr++)->Null();
		}
		
	private:

		int _pointer;

		Var *_stackbase;
		Var *_stackptr;

		CallStackFrame *_callstackbase;
		CallStackFrame *_callstackptr;

		unsigned int stackDistance() const
		{
			return _stackptr - _stackbase;
		}

		unsigned int callstackDistance() const
		{
			return _callstackptr - _callstackbase;
		}

		ILogger *_logger;
		IRuntimeEnvironment *_environment;
		Sub *_runningSub;

		void logUnknownDatatype(DataType dt);
		void logUnknownOpcode(Opcode opcode);
		void logError(const rstring &msg);
		VirtualMachineResult logNotNumericTypeError();
		void checkPop(unsigned int count);
		float getNumeric(Var *frame, DataType type);
		VirtualMachineResult callSub(Sub *sub, bool returnExpected);
		VirtualMachineResult returnFromSub(bool returned);
		
	private:

		inline VirtualMachineResult executePush(const Instruction *instruction);
		inline VirtualMachineResult executeBinaryOp(Opcode opcode);
		inline VirtualMachineResult executeUnaryOp(Opcode opcode);
		inline VirtualMachineResult executeGetLocal(unsigned int argument);
		inline VirtualMachineResult getCallArgumentValue(Var *stackFrame);
		inline VirtualMachineResult executeSetLocal(unsigned int argument);
		inline VirtualMachineResult executeCall(const Instruction *instruction);
		inline VirtualMachineResult executeInternalCall(const Instruction *instruction);
		inline VirtualMachineResult executeStaticCall(const Instruction *instruction);
		inline VirtualMachineResult executeDynamicCall(const Instruction *instruction);
		inline VirtualMachineResult executeReturn(bool returned);
		inline VirtualMachineResult executeJump(const Instruction *instruction);
		inline VirtualMachineResult executeNewArray(unsigned int argument);
		inline VirtualMachineResult executeSetElement();
		inline VirtualMachineResult executeGetElement();
	};
}


#endif //__VIRTUAL_MACHINE_H_INCLUDED__