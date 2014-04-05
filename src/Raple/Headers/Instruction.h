#ifndef __INSTRUCTION_H_INCLUDED__
#define __INSTRUCTION_H_INCLUDED__


//-------------------------------------
#include "Opcode.h"
#include "String.h"
//-------------------------------------


namespace Raple
{
	class Instruction
	{
	public:

		Instruction(Opcode opcode, unsigned int arg = 0)
			: _opcode(opcode)
		{
			_arg = arg;

			IntConst = 0;
			FloatConst = 0.f;
			StringConst = 0;

			_string = false;
		}

		~Instruction()
		{
			if (_string)
				delete StringConst;
		}

	private:

		Opcode _opcode;
		unsigned int _arg;
		bool _string;

		union
		{
			int IntConst;
			float FloatConst;
			rstring *StringConst;
		};

	public:

		void Int(int intConst) 
		{ 
			_string = false;
			IntConst = intConst; 
		}

		void Float(float floatConst) 
		{ 
			_string = false;
			FloatConst = floatConst; 
		}

		void String(rstring *stringConst) 
		{ 
			_string = true;
			StringConst = stringConst; 
		}

		rstring *String() const
		{
			return StringConst;
		}

		int Int() const
		{
			return IntConst;
		}

		float Float() const
		{
			return FloatConst;
		}

		void Argument(unsigned int argument)
		{
			_arg = argument;
		}

		unsigned int Argument() const 
		{ 
			return _arg; 
		}

		Opcode GetOpcode() const 
		{ 
			return _opcode; 
		}
	};
}


#endif //__INSTRUCTION_H_INCLUDED__