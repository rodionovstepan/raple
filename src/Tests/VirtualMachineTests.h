#ifndef __VIRTUAL_MACHINE_TESTS_H_INCLUDED__
#define __VIRTUAL_MACHINE_TESTS_H_INCLUDED__


//-------------------------------------
#include "../CppTest/cpptest.h"
#include "FakeLoggerForCompilerTests.h"
#include "FakeRuntimeEnvironment.h"
#include "../Raple/Headers/Opcode.h"
#include "../Raple/Headers/Sub.h"
#include "../Raple/Headers/VirtualMachine.h"
#include "../Raple/Sources/VirtualMachine.cpp"
//-------------------------------------
//-------------------------------------
#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif
//-------------------------------------
using namespace RapleTestsHelpers;
using Raple::VirtualMachine;
using Raple::Opcode;
//-------------------------------------


namespace RapleTests
{
	class VirtualMachineTests : public Test::Suite
	{
	public:

		VirtualMachineTests()
		{
			addTest(VirtualMachineTests::Test1);
			addTest(VirtualMachineTests::PushIntTest);
		}

		void Test1()
		{
			FakeLoggerForCompilerTests logger;
			FakeRuntimeEnvironment runenv;

			VirtualMachine vm(&runenv, &logger);

			Sub sub("main");
			sub.GetBytecode()->AddInstruction(Raple::opNop);
			sub.GetBytecode()->AddInstruction(Raple::opRet);

			int result = (int) vm.Execute(&sub);

			assertEquals(0, result);
		}

		void PushIntTest()
		{
			FakeLoggerForCompilerTests logger;
			FakeRuntimeEnvironment runenv;

			VirtualMachine vm(&runenv, &logger);

			Sub sub("main");
			sub.GetBytecode()->AddInstruction(Raple::opPushInt);
			sub.GetBytecode()->LastInstruction()->Int(21);
			sub.GetBytecode()->AddInstruction(Raple::opRet);

			int result = (int) vm.Execute(&sub);

			assertEquals(0, result);
		}
	};
}






#endif //__VIRTUAL_MACHINE_TESTS_H_INCLUDED__