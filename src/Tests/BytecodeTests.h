#ifndef __BYTECODE_TESTS_H_INCLUDED__
#define __BYTECODE_TESTS_H_INCLUDED__


//-------------------------------------
#include "../CppTest/cpptest.h"
#include "../Raple/Headers/Bytecode.h"
#include "../Raple/Headers/Instruction.h"
#include "../Raple/Headers/Opcode.h"
#include "../Raple/Sources/Bytecode.cpp"
//-------------------------------------
#include <memory>
//-------------------------------------
#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif
//-------------------------------------
using Raple::Bytecode;
using Raple::Instruction;
using Raple::Opcode;
//-------------------------------------


namespace RapleTests
{
	class BytecodeTests : public Test::Suite
	{
	public:

		BytecodeTests()
		{
			addTest(BytecodeTests::FullBytecodeTest);
		}

	private:

		void FullBytecodeTest()
		{
			Bytecode bytecode;

			assertEquals(0, bytecode.InstructionCount());

			bytecode.AddInstruction(new Instruction(Raple::opNop));
			bytecode.AddInstruction(new Instruction(Raple::opNop, 1));

			assertEquals(2, bytecode.InstructionCount());

			assertEquals(0, bytecode.GetInstruction(0)->Argument());
			assertEquals(1, bytecode.GetInstruction(1)->Argument());

			bytecode.Clear();

			assertEquals(0, bytecode.InstructionCount());
		}
	};
}


#endif //__BYTECODE_TESTS_H_INCLUDED__