#ifndef __BYTECODE_H_INCLUDED__
#define __BYTECODE_H_INCLUDED__


//-------------------------------------
#include "Instruction.h"
//-------------------------------------
#include <vector>
//-------------------------------------


namespace Raple
{
	class Bytecode
	{
	public:

		~Bytecode();

		void AddInstruction(Instruction *instr);
		void AddInstruction(Opcode opcode)
		{
			AddInstruction(new Instruction(opcode));
		}

		void Clear();

		Instruction *GetInstruction(unsigned int index) const
		{
			unsigned count = InstructionCount();

			if (count && index < count)
				return _instructions[index];

			return 0;
		}

		Instruction *LastInstruction() { return _instructions.back(); }
		unsigned int InstructionCount() const { return _instructions.size(); }

	private:

		std::vector<Instruction *> _instructions;
	};
}


#endif //__BYTECODE_H_INCLUDED__