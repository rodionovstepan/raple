//-------------------------------------
#include "../Headers/StaticIoLibrary.h"
#include "../Headers/IVirtualMachine.h"
#include "../Headers/HashArray.h"
//-------------------------------------
#include <iostream>
//-------------------------------------


namespace RapleLibraries
{
	StaticIoLibrary::StaticIoLibrary()
	{
		registerSub("print", 1, StaticIoLibrary::io_print);
		registerSub("println", 1, StaticIoLibrary::io_println);
	}

	StaticIoLibrary::~StaticIoLibrary()
	{
	}

	int StaticIoLibrary::io_print(IVirtualMachine *vm)
	{
		print_var(vm->Pop());
		return 0;
	}

	int StaticIoLibrary::io_println(IVirtualMachine *vm)
	{
		int print = io_print(vm);
		std::cout << "\n";

		return print;
	}

	void StaticIoLibrary::print_var(Var *var)
	{
		DataType t = var->GetDataType();

		if (t == dtInteger)
			std::cout << var->Int();
		else if (t == dtFloat)
			std::cout << var->Float();
		else if (t == dtString)
			std::cout << var->String()->GetBuffer();
		else if (t == dtArray)
		{
			auto ha = var->Array();
			unsigned int size = ha->Size();

			for (unsigned int i = 0; i < size; ++i)
			{
				auto* item = ha->GetByIndex(i);
				if (item != 0 && item->GetKey() != 0)
				{
					print_var(item->GetKey());
					std::cout << " => ";
					print_var(item->GetValue());
					std::cout << "\n";
				}
			}
		}
		else if (t == dtNull)
			std::cout << "null";
	}
}