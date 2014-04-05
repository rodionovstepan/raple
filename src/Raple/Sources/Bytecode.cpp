//-------------------------------------
#include "../Headers/Bytecode.h"
//-------------------------------------


namespace Raple
{
	Bytecode::~Bytecode()
	{
		Clear();
	}

	void Bytecode::Clear()
	{
		for (std::vector<Instruction *>::iterator I = _instructions.begin();
			 I != _instructions.end();
			 ++I)
		{
			delete *I;
		}

		_instructions.clear();
	}

	void Bytecode::AddInstruction(Instruction *instr)
	{
		_instructions.push_back(instr);
	}
}