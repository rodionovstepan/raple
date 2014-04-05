#ifndef __VIRTUAL_MACHINE_RESULT_H_INCLUDED__
#define __VIRTUAL_MACHINE_RESULT_H_INCLUDED__


namespace Raple
{
	enum VirtualMachineResult
	{
		vmrSuccess,
		vmrFailed,
		vmrNullEnvironment,
		vmrUnknownOpcode,
		vmrUnknownDataType,
		vmrUnknownStackFrameType,
		vmrCallstackUnderflow,
		vmrCallstackOverflow,
		vmrExitProgram,
		vmrTypeError
	};
}




#endif //__VIRTUAL_MACHINE_RESULT_H_INCLUDED__