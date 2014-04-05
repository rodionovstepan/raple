#ifndef __FUNCTION_H_INCLUDED__
#define __FUNCTION_H_INCLUDED__


//-------------------------------------
#include "String.h"
#include "Bytecode.h"
#include "Var.h"
//-------------------------------------
#include <vector>
//-------------------------------------

namespace Raple
{
	class IVirtualMachine;

	typedef int (*NativeFunction)(IVirtualMachine *vm);

	class Sub
	{
	public:

		Sub(const rstring &name, unsigned int argsCount = 0, NativeFunction nativeFunction = 0)
			: _name(name), _argsCount(argsCount), _nativeFunction(nativeFunction)
		{
			_bytecode = new Bytecode();
		}

		~Sub()
		{
			delete _bytecode;
			
			for (std::vector<Var *>::iterator I = _locals.begin(); I != _locals.end(); ++I)
				delete *I;
		}

		Bytecode *GetBytecode() const { return _bytecode; }
		NativeFunction GetNativeFunction() const { return _nativeFunction; }

		bool IsNativeFunction() const 
		{ 
			return _nativeFunction != 0; 
		}

		const rstring &GetName() const 
		{ 
			return _name; 
		}

		void SetArgumentsCount(unsigned int argsCount) 
		{ 
			_argsCount = argsCount; 
		}
		
		void SetNativeFunction(NativeFunction nf)
		{
			_nativeFunction = nf;
		}

		unsigned int GetArgumentsCount() const 
		{ 
			return _argsCount; 
		}

		unsigned int GetLocalsCount() const 
		{ 
			return _locals.size(); 
		}
		
		unsigned int AddNewLocal(const rstring &name, DataType dataType = dtNull)
		{
			int lastIndex = GetLocalsCount();

			_locals.push_back(new Var(name, dataType));

			return lastIndex;
		}

		unsigned int AddNewAnonymousLocal(DataType dataType = dtNull)
		{
			rstring name;
			name.Format("__anon_lv_%d", _locals.size());

			return AddNewLocal(name, dataType);
		}

		Var *GetLocal(unsigned int id) const
		{
			unsigned int count = GetLocalsCount();
			
			if (count && id < count)
				return _locals[id];
			
			return 0;
		}

		//! Todo: optimize!
		int FindLocal(const rstring &name) const
		{
			int count = GetLocalsCount();

			for (int i = 0; i < count; ++i)
				if (_locals[i]->GetName() == name)
					return i;

			return -1;
		}

	private:

		rstring _name;
		unsigned int _argsCount;

		NativeFunction _nativeFunction;

		Bytecode *_bytecode;

		std::vector<Var *> _locals;
	};
}



#endif //__FUNCTION_H_INCLUDED__