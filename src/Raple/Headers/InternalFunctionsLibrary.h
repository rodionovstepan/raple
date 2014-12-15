#ifndef __INTERNAL_FUNCTIONS_LIBRARY_H_INCLDUDED__
#define __INTERNAL_FUNCTIONS_LIBRARY_H_INCLDUDED__


//-------------------------------------
#include "Sub.h"
#include "Var.h"
#include "IVirtualMachine.h"
#include "System.h"
#include "HashArray.h"
//-------------------------------------
#include <map>
//-------------------------------------



namespace Raple
{
	class InternalFunctionsLibrary
	{
		static InternalFunctionsLibrary *_instance;
		std::map<DataType, std::map<rhash, Sub *>> _lib;

	public:
		~InternalFunctionsLibrary();

		void RegisterAll();
		static InternalFunctionsLibrary *Library();
		Sub *GetInternalFunction(DataType dataType, rhash hash);

	protected:
		InternalFunctionsLibrary()
		{
		}

		void registerFunction(DataType dataType, const rstring &name, unsigned int argCount, NativeFunction function)
		{
			//
			//	+1 к argCount нужно для того, чтобы сместить стек на 1
			//	дополнительную позицию, чтобы захватить саму переменную,
			//	у которой вызывается функция
			//
			_lib[dataType][name.Hash()] = new Sub(name, argCount+1, function);
		}

		static Var *popVar(IVirtualMachine *vm, DataType requiredType)
		{
			Var *var = vm->Pop();
			RapleAssert(var->GetDataType() == requiredType);
			return var;
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////
		//			STRING
		/////////////////////////////////////////////////////////////////////////////////////////////////

		static int string_length(IVirtualMachine *vm)
		{
			Var *str = popVar(vm, dtString);
			vm->PushInt(str->String()->Length());
			return 0;
		}

		static int string_indexOf(IVirtualMachine *vm)
		{
			Var *str = popVar(vm, dtString);
			Var *c = popVar(vm, dtString);

			char symbol = c->String()->operator[](0);
			vm->PushInt(str->String()->IndexOf(symbol));
			return 0;
		}

		static int string_type(IVirtualMachine *vm)
		{
			popVar(vm, dtString);
			vm->PushString("string");
			return 0;
		}

		static int string_toint(IVirtualMachine *vm)
		{
			Var *str = popVar(vm, dtString);
			vm->PushInt(atoi(str->String()->GetBuffer()));
			return 0;
		}

		static int string_tostring(IVirtualMachine *vm)
		{
			Var *str = popVar(vm, dtString);
			vm->PushString(str->String()->GetBuffer());
			return 0;
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////
		//			ARRAY
		/////////////////////////////////////////////////////////////////////////////////////////////////

		static int array_size(IVirtualMachine *vm)
		{
			Var *array = popVar(vm, dtArray);
			vm->PushInt(array->Array()->ItemCount());
			return 0;
		}

		static int array_type(IVirtualMachine *vm)
		{
			popVar(vm, dtArray);
			vm->PushString("array");
			return 0;
		}

		static int array_add(IVirtualMachine *vm)
		{
			Var *array = popVar(vm, dtArray);
			Var *var = vm->Pop();
			array->Array()->Add(var);
			return 0;
		}

		static int array_tostring(IVirtualMachine *vm)
		{
			Var *array = popVar(vm, dtArray);
			vm->PushString("not impl yet");
			return 0;
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////
		//			INT
		/////////////////////////////////////////////////////////////////////////////////////////////////

		static int int_type(IVirtualMachine *vm)
		{
			popVar(vm, dtInteger);
			vm->PushString("int");
			return 0;
		}

		static int int_tostring(IVirtualMachine *vm)
		{
			Var *var = popVar(vm, dtInteger);
			char buf[32];
			sprintf_s(buf, 31, "%d", var->Int());
			vm->PushString(buf);
			return 0;
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////
		//			FLOAT
		/////////////////////////////////////////////////////////////////////////////////////////////////

		static int float_type(IVirtualMachine *vm)
		{
			popVar(vm, dtFloat);
			vm->PushString("float");
			return 0;
		}

		static int float_tostring(IVirtualMachine *vm)
		{
			Var *var = popVar(vm, dtFloat);
			char buf[32];
			sprintf_s(buf, 31, "%f", var->Float());
			vm->PushString(buf);
			return 0;
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////
		//			NULL
		/////////////////////////////////////////////////////////////////////////////////////////////////

		static int null_type(IVirtualMachine *vm)
		{
			popVar(vm, dtNull);
			vm->PushString("null");
			return 0;
		}

		static int null_tostring(IVirtualMachine *vm)
		{
			popVar(vm, dtNull);
			vm->PushString("");
			return 0;
		}
	};
}



#endif //__INTERNAL_FUNCTIONS_LIBRARY_H_INCLDUDED__