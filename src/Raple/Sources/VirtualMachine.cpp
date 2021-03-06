//-------------------------------------
#include "../Headers/VirtualMachine.h"
#include "../Headers/HashArray.h"
//-------------------------------------


namespace Raple
{
	VirtualMachine::VirtualMachine(IRuntimeEnvironment *environment, ILogger *logger)
	{
		_logger = logger;
		_environment = environment;

		_stackbase = new Var [Constants::StackSize];
		_stackptr = _stackbase;

		_callstackbase = new CallStackFrame [Constants::StackSize];
		_callstackptr = _callstackbase;

		_pointer = 0;
	}

	VirtualMachine::~VirtualMachine()
	{
		delete [] _stackbase;
		delete [] _callstackbase;
	}

	IRuntimeEnvironment *VirtualMachine::GetEnvironment()
	{
		return _environment;
	}

	void VirtualMachine::dumpStack()
	{
		Opcode op = _runningSub->GetBytecode()->GetInstruction(_pointer-1)->GetOpcode();
		rstring sub = _runningSub->GetName();
		
		printf("%s:\t%s: stack size = %d\n", sub.GetBuffer(), GetOpcodeDefinition(op), _stackptr - _stackbase);
	}

	VirtualMachineResult VirtualMachine::Execute(Sub *sub)
	{
		if (_environment == 0 || _logger == 0)
			return vmrNullEnvironment;

		_runningSub = sub;
		VirtualMachineResult result = vmrSuccess;

		for (;;)
		{
			const Instruction *instr = _runningSub->GetBytecode()->GetInstruction(_pointer++);

			//dumpStack();

			switch (instr->GetOpcode())
			{
			case opNop:
				break;

			case opPushInt:
			case opPushFloat:
			case opPushString:
			case opGetSub:
				result = executePush(instr);
				break;

			case opAdd:
			case opSub:
			case opDiv:
			case opProd:
			case opExp:
			case opEqual:
			case opNotEqual:
			case opGreater:
			case opLess:
			case opGreaterOrEqual:
			case opLessOrEqual:
				result = executeBinaryOp(instr->GetOpcode());
				break;

			case opNeg:
			case opPostInc:
			case opPostDec:
			case opPreInc:
			case opPreDec:
				result = executeUnaryOp(instr->GetOpcode(), instr->Argument());
				break;

			case opLogicAnd:
			case opLogicOr:
				result = executeLogicOp(instr->GetOpcode());
				break;

			case opGetLocal:
				result = executeGetLocal(instr->Argument());
				break;

			case opSetLocal:
				result = executeSetLocal(instr->Argument());
				break;

			case opRet:
				result = executeReturn(instr->Argument());
				break;

			case opCall:
				result = executeCall(instr);
				break;

			case opInternalCall:
				result = executeInternalCall(instr);
				break;

			case opStaticCall:
				result = executeStaticCall(instr);
				break;

			case opDynamicCall:
				result = executeDynamicCall(instr);
				break;

			case opGoto:
				_pointer = instr->Argument();
				break;

			case opJumpZero:
			case opJumpNotZero:
				result = executeJump(instr);
				break;

			case opNewArray:
				result = executeNewArray(instr->Argument());
				break;

			case opArraySize:
				result = executeArraySize(instr->Argument());
				break;

			case opSetElement:
				result = executeSetElement();
				break;

			case opGetElement:
				result = executeGetElement();
				break;

			case opGetNextElement:
				result = executeGetNextElement();
				break;

			case opRange:
				result = executeCreateRange();
				break;

			default:
				logUnknownOpcode(instr->GetOpcode());
				result = vmrUnknownOpcode;
				break;
			}

			if (result != vmrSuccess)
			{
				if (result == vmrExitProgram)
					result = vmrSuccess;

				break;
			}
		}

		return result;
	}

	inline VirtualMachineResult VirtualMachine::executeBinaryOp(Opcode opcode)
	{
		checkPop(2);
			
		Var *frame1 = Pop();
		Var *frame2 = Pop();

		DataType t1 = frame1->GetDataType();
		DataType t2 = frame2->GetDataType();

		switch (opcode)
		{
		case opAdd:

			if ((t1 == t2) && (t1 == dtInteger))
				PushInt(frame1->Int() + frame2->Int());
			else if (Var::IsNumericType(t1) && Var::IsNumericType(t2))
				PushFloat(getNumeric(frame1, t1) + getNumeric(frame2, t2));
			else if (Var::IsStringType(t1) || Var::IsStringType(t2))
				PushString(*(frame2->String()) + *(frame1->String()));

			return vmrSuccess;

		case opSub:

			if ((t1 == t2) && (t1 == dtInteger))
				PushInt(frame2->Int() - frame1->Int());
			else if (Var::IsNumericType(t1) && Var::IsNumericType(t2))
				PushFloat(getNumeric(frame2, t2) - getNumeric(frame1, t1));
			else
				return logNotNumericTypeError();

			return vmrSuccess;

		case opProd:

			if ((t1 == t2) && (t1 == dtInteger))
				PushInt(frame1->Int() * frame2->Int());
			else if (Var::IsNumericType(t1) && Var::IsNumericType(t2))
				PushFloat(getNumeric(frame1, t1) * getNumeric(frame2, t2));
			else
				return logNotNumericTypeError();

			return vmrSuccess;

		case opDiv:

			if ((t1 == t2) && (t1 == dtInteger))
				PushInt(frame2->Int() / frame1->Int());
			else if (Var::IsNumericType(t1) && Var::IsNumericType(t2))
				PushFloat(getNumeric(frame2, t2) / getNumeric(frame1, t1));
			else
				return logNotNumericTypeError();

			return vmrSuccess;

		case opExp:

			if ((t1 == t2) && (t1 == dtInteger))
				PushInt((int)pow((float)frame2->Int(), frame1->Int()));
			else if (Var::IsNumericType(t1) && Var::IsNumericType(t2))
				PushFloat(pow(getNumeric(frame2, t2), getNumeric(frame1, t1)));
			else
				return logNotNumericTypeError();

			return vmrSuccess;

		case opEqual:

			if (Var::IsNumericType(t1) && Var::IsNumericType(t2))
				PushInt(getNumeric(frame1, t1) == getNumeric(frame2, t2));
			else if (Var::IsStringType(t1) && Var::IsStringType(t2))
				PushInt(*(frame1->String()) == *(frame2->String()));
			else
				PushInt(0);

			return vmrSuccess;

		case opNotEqual:

			if (Var::IsNumericType(t1) && Var::IsNumericType(t2))
				PushInt(getNumeric(frame1, t1) != getNumeric(frame2, t2));
			else if (Var::IsStringType(t1) && Var::IsStringType(t2))
				PushInt(*(frame1->String()) != *(frame2->String()));
			else
				PushInt(0);
				
			return vmrSuccess;

		case opGreater:

			if (Var::IsNumericType(t1) && Var::IsNumericType(t2))
				PushInt(getNumeric(frame2, t2) > getNumeric(frame1, t1));
			else
				return logNotNumericTypeError();

			return vmrSuccess;

		case opLess:

			if (Var::IsNumericType(t1) && Var::IsNumericType(t2))
				PushInt(getNumeric(frame2, t2) < getNumeric(frame1, t1));
			else
				return logNotNumericTypeError();

			return vmrSuccess;

		case opGreaterOrEqual:

			if (Var::IsNumericType(t1) && Var::IsNumericType(t2))
				PushInt(getNumeric(frame2, t2) >= getNumeric(frame1, t1));
			else
				return logNotNumericTypeError();

			return vmrSuccess;

		case opLessOrEqual:

			if (Var::IsNumericType(t1) && Var::IsNumericType(t2))
				PushInt(getNumeric(frame2, t2) <= getNumeric(frame1, t1));
			else
				return logNotNumericTypeError();

			return vmrSuccess;
		}

		logUnknownOpcode(opcode);
		return vmrUnknownOpcode;
	}

	inline VirtualMachineResult VirtualMachine::executeUnaryOp(Opcode opcode, unsigned int argument)
	{
		checkPop(1);

		Var *frame = Pop();

		switch (opcode)
		{
		case opNeg:
				
			if (frame->GetDataType() == dtFloat)
				PushFloat(-frame->Float());
			else if (frame->GetDataType() == dtInteger)
				PushInt(-frame->Int());
			else
			{
				logUnknownDatatype(frame->GetDataType());
				return vmrUnknownDataType;
			}

			return vmrSuccess;

		case opPostInc:
		case opPreInc:

			if (frame->GetDataType() == dtFloat)
			{
				Var *var = _runningSub->GetLocal(argument);
				RapleAssert(var != 0);

				var->Float(frame->Float() + 1.);
				PushFloat(opcode == opPostInc ? frame->Float() : var->Float());
			}
			else if (frame->GetDataType() == dtInteger)
			{
				Var *var = _runningSub->GetLocal(argument);
				RapleAssert(var != 0);

				var->Int(frame->Int() + 1);
				PushInt(opcode == opPostInc ? frame->Int() : var->Int());
			}
			else
			{
				logUnknownDatatype(frame->GetDataType());
				return vmrUnknownDataType;
			}

			return vmrSuccess;

		case opPostDec:
		case opPreDec:

			if (frame->GetDataType() == dtFloat)
			{
				Var *var = _runningSub->GetLocal(argument);
				RapleAssert(var != 0);

				var->Float(frame->Float() - 1.);
				PushFloat(opcode == opPostDec ? frame->Float() : var->Float());
			}
			else if (frame->GetDataType() == dtInteger)
			{
				Var *var = _runningSub->GetLocal(argument);
				RapleAssert(var != 0);

				var->Int(frame->Int() - 1);
				PushInt(opcode == opPostDec ? frame->Int() : var->Int());
			}
			else
			{
				logUnknownDatatype(frame->GetDataType());
				return vmrUnknownDataType;
			}

			return vmrSuccess;
		}

		logUnknownOpcode(opcode);
		return vmrUnknownOpcode;
	}

	inline VirtualMachineResult VirtualMachine::executePush(const Instruction *instruction)
	{
		switch (instruction->GetOpcode())
		{
		case opPushInt:
			PushInt(instruction->Int());
			break;
				
		case opPushFloat:
			PushFloat(instruction->Float());
			break;

		case opPushString:
			PushString(*(instruction->String()));
			break;

		case opGetSub:
			PushFunctionPointer(instruction->Argument());
			break;
		}

		return vmrSuccess;
	}

	inline VirtualMachineResult VirtualMachine::executeSetLocal(unsigned int argument)
	{
		Var *local = _runningSub->GetLocal(argument);
		RapleAssert(local != 0);

		local->CopyFrom(Pop());
		return vmrSuccess;
	}

	inline VirtualMachineResult VirtualMachine::executeGetLocal(unsigned int argument)
	{
		Var *local = _runningSub->GetLocal(argument);
		RapleAssert(local != 0);

		switch (local->GetDataType())
		{
		case dtInteger:
			PushInt(local->Int());
			break;

		case dtFloat:
			PushFloat(local->Float());
			break;

		case dtString:
			PushString(*(local->String()));
			break;

		case dtCallArgument:
			return getCallArgumentValue(_stackbase + argument);

		case dtArray:
			PushArrayPointer(local->Array());
			break;

		case dtFunction:
			PushFunctionPointer(local->Int());
			break;

		default:
			logUnknownDatatype(local->GetDataType());
			return vmrUnknownDataType;
		}

		return vmrSuccess;
	}

	inline VirtualMachineResult VirtualMachine::getCallArgumentValue(Var *stackFrame)
	{
		switch (stackFrame->GetDataType())
		{
		case dtFloat:
			PushFloat(stackFrame->Float());
			break;

		case dtInteger:
			PushInt(stackFrame->Int());
			break;

		case dtString:
			PushString(*(stackFrame->String()));
			break;

		case dtArray:
			PushArrayPointer(stackFrame->Array());
			break;

		case dtFunction:
			PushFunctionPointer(stackFrame->Int());
			break;

		default:
			logUnknownDatatype(stackFrame->GetDataType());
			return vmrUnknownStackFrameType;
		}

		return vmrSuccess;
	}

	inline VirtualMachineResult VirtualMachine::executeCall(const Instruction *instruction)
	{
		Sub *func = _environment->GetSub(instruction->Argument());
		RapleAssert(func != 0);
			
		return callSub(func, instruction->Int() != 0);
	}

	inline VirtualMachineResult VirtualMachine::executeInternalCall(const Instruction *instruction)
	{
		checkPop(1);

		Var *local = Top();
		Sub *func = local->GetInternalFunction(instruction->Argument());
		if (func == 0)
		{
			logError("Undefined function call");
			return vmrFailed;
		}
		
		return callSub(func, instruction->Int() != 0);
	}

	inline VirtualMachineResult VirtualMachine::executeStaticCall(const Instruction *instruction)
	{
		checkPop(1);

		Var *libId = Pop();
		Sub *staticlibsub = _environment->GetLibrarySub(libId->Int(), instruction->Argument());
		if (staticlibsub == 0)
		{
			logError("Undefined function call");
			return vmrFailed;
		}

		return callSub(staticlibsub, instruction->Int() != 0);
	}

	inline VirtualMachineResult VirtualMachine::executeDynamicCall(const Instruction *instruction)
	{
		checkPop(1);

		Var *local = Pop();
		Sub *dynamic = _environment->GetSub(local->Int());

		if (dynamic == 0 || local->GetDataType() != dtFunction)
		{
			logError("Dynamic call failed");
			return vmrFailed;
		}

		return callSub(dynamic, instruction->Int() != 0);
	}

	inline VirtualMachineResult VirtualMachine::executeReturn(bool returned)
	{
		return returnFromSub(returned);
	}

	inline VirtualMachineResult VirtualMachine::executeJump(const Instruction *instruction)
	{
		checkPop(1);

		Var *headFrame = Pop();
		if (headFrame->GetDataType() != dtInteger)
			return vmrUnknownStackFrameType;

		if ((bool)headFrame->Int() == (instruction->GetOpcode() == opJumpNotZero))
			_pointer = instruction->Argument();

		return vmrSuccess;
	}

	inline VirtualMachineResult VirtualMachine::executeSetElement()
	{
		checkPop(3);

		Var *array = Pop();
		Var *key = Pop();
		Var *value = Pop();

		RapleAssert(array != 0 && array->GetDataType() == dtArray);

		array->Array()->Set(key, value);

		return vmrSuccess;
	}

	inline VirtualMachineResult VirtualMachine::executeGetElement()
	{
		checkPop(2);

		Var *array = Pop();
		Var *key = Pop();

		RapleAssert(array != 0 && array->GetDataType() == dtArray);

		Var *value = array->Array()->Get(key);
		if (value == 0)
			PushNull();
		else if (!PushVal(value))
			return vmrUnknownDataType;

		return vmrSuccess;
	}

	inline VirtualMachineResult VirtualMachine::executeGetNextElement()
	{
		checkPop(2);
		Var *val = Pop();
		Var *idx = Pop();
		
		RapleAssert(val != 0 && idx != 0 && val->GetDataType() == dtArray && idx->GetDataType() == dtInteger);

		HashArray *arr = val->Array();
		unsigned int n = arr->Size();
		unsigned int index = idx->Int();

		if (index >= n)
		{
			PushNull();
			PushInt(n);
			return vmrSuccess;
		}

		for (unsigned int i = index; i < n; ++i)
		{
			if (arr->GetByIndex(i)->GetKey() != 0)
			{
				if (!PushVal(arr->GetByIndex(i)->GetValue()))
					return vmrUnknownDataType;

				PushInt(i);
				return vmrSuccess;
			}
		}

		PushNull();
		PushInt(n);
		return vmrSuccess;
	}

	inline VirtualMachineResult VirtualMachine::executeLogicOp(Opcode opcode)
	{
		checkPop(2);

		Var *v1 = Pop();
		Var *v2 = Pop();
		int i1 = v1->Int(), i2 = v2->Int();

		if (opcode == opLogicAnd)
		{			
			if (i1 != 0 && i2 != 0)
				PushInt(1);
			else
				PushInt(0);
		}
		else if (opcode == opLogicOr)
		{
			if (i1 != 0 || i2 != 0)
				PushInt(1);
			else 
				PushInt(0);
		}
		else
			return vmrUnknownOpcode;

		return vmrSuccess;
	}

	inline VirtualMachineResult VirtualMachine::executeNewArray(unsigned int argument)
	{
		Var *local = _runningSub->GetLocal(argument);
		RapleAssert(local != 0);

		local->CreateArray();
		return vmrSuccess;
	}

	inline VirtualMachineResult VirtualMachine::executeArraySize(unsigned int argument)
	{
		Var *local = _runningSub->GetLocal(argument);
		RapleAssert(local != 0 && local->GetDataType() == dtArray);

		PushInt(local->Array()->ItemCount());
		return vmrSuccess;
	}

	inline VirtualMachineResult VirtualMachine::executeCreateRange()
	{
		checkPop(2);

		int right = Pop()->Int();
		int left = Pop()->Int();

		HashArray *array = new HashArray();
		if (left > right)
		{
			for (; right <= left; --left)
				array->Add(Var::CreateInt(left));
		}
		else 
		{
			for (; left <= right; ++left)
				array->Add(Var::CreateInt(left));
		}

		PushArrayPointer(array);
		return vmrSuccess;
	}

	float VirtualMachine::getNumeric(Var *frame, DataType type)
	{
		if (type == dtFloat)
			return frame->Float();

		return (float)frame->Int();
	}

	void VirtualMachine::logUnknownDatatype(DataType dt)
	{
		logError(rstring("Unknown data type [") + GetDataTypeDefinition(dt) + "]");
	}

	void VirtualMachine::logUnknownOpcode(Opcode opcode)
	{
		logError(rstring("Unknown opcode [") + GetOpcodeDefinition(opcode) + "]");
	}

	VirtualMachineResult VirtualMachine::logNotNumericTypeError()
	{
		logError("Type error: type should be numeric");
		return vmrTypeError;
	}

	void VirtualMachine::logError(const rstring &msg)
	{
		_logger->Error(Constants::LogTitleVirtualMachineError, msg);
	}

	void VirtualMachine::checkPop(unsigned int count)
	{
		RapleAssert(stackDistance() >= count);
	}

	VirtualMachineResult VirtualMachine::callSub(Sub *sub, bool returnExpected)
	{
		if (callstackDistance() == Constants::StackSize)
			return vmrCallstackOverflow;

		_callstackptr->Sub = _runningSub;
		_callstackptr->Stackbase = _stackbase;
		_callstackptr->Stackptr = _stackptr - sub->GetArgumentsCount();
		_callstackptr->Pointer = _pointer;
		_callstackptr->ReturnValueExpected = returnExpected;

		_stackbase = _callstackptr->Stackptr;
		_runningSub = sub;
		_pointer = 0;
		
		++_callstackptr;

		if (sub->IsNativeFunction())
		{
			Var *check = _stackptr - sub->GetArgumentsCount();

			int result = sub->GetNativeFunction()(this);
			if (result != 0)
				return vmrFailed;

			int returned = _stackptr - check;
			return returnFromSub(returned > 0);
		}

		return vmrSuccess;
	}

	VirtualMachineResult VirtualMachine::returnFromSub(bool returned)
	{
		if (callstackDistance() == 0)
			return vmrExitProgram;
		
		--_callstackptr;
		if (_callstackptr->ReturnValueExpected)
		{
			if (returned)
				_stackbase->CopyFrom(_stackptr-1);
			else
				_stackbase->Null();
		}

		_runningSub->Return();
		_stackptr = _callstackptr->Stackptr + (_callstackptr->ReturnValueExpected ? 1 : 0);
		_stackbase = _callstackptr->Stackbase;
		_runningSub = _callstackptr->Sub;
		_pointer = _callstackptr->Pointer;
		
		return vmrSuccess;
	}
}
