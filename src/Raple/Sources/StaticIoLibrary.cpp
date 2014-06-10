//-------------------------------------
#include "../Headers/StaticIoLibrary.h"
#include "../Headers/IVirtualMachine.h"
#include "../Headers/HashArray.h"
#include "../Headers/tinydir.h"
//-------------------------------------
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
//-------------------------------------


namespace RapleLibraries
{
	StaticIoLibrary::StaticIoLibrary()
	{
		registerSub("print", 1, StaticIoLibrary::io_print);
		registerSub("println", 1, StaticIoLibrary::io_println);
		registerSub("ls", 1, StaticIoLibrary::io_ls);
		registerSub("remove", 1, StaticIoLibrary::io_remove);
		registerSub("copy", 2, StaticIoLibrary::io_copy);
		registerSub("fexists", 1, StaticIoLibrary::io_fexists);
		registerSub("dexists", 1, StaticIoLibrary::io_dexists);
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

	int StaticIoLibrary::io_ls(IVirtualMachine *vm)
	{
		Var *v = vm->Pop();
		if (Var::IsStringType(v->GetDataType()) == false)
			return 1;

		tinydir_dir dir;
		tinydir_open(&dir, v->String()->GetBuffer());

		int i = 0;
		HashArray *result = new HashArray();

		while (dir.has_next)
		{
			tinydir_file file;
			tinydir_readfile(&dir, &file);

			result->Set(Var::CreateInt(i++), Var::CreateString(file.name));

			tinydir_next(&dir);
		}

		tinydir_close(&dir);

		vm->PushArrayPointer(result);
		return 0;
	}

	int StaticIoLibrary::io_remove(IVirtualMachine *vm)
	{
		Var *v = vm->Pop();
		if (Var::IsStringType(v->GetDataType()) == false)
		{
			vm->PushInt(-1);
			return 1;
		}

		vm->PushInt(remove(v->String()->GetBuffer()));
		return 0;
	}

	int StaticIoLibrary::io_copy(IVirtualMachine *vm)
	{
		Var *to = vm->Pop();
		Var *from = vm->Pop();
		if (Var::IsStringType(from->GetDataType()) == false || Var::IsStringType(to->GetDataType()) == false)
		{
			vm->PushInt(-1);
			return 1;
		}

		std::ifstream in(from->String()->GetBuffer(), std::ios::binary);
		if (in.good())
		{
			std::ofstream out(to->String()->GetBuffer(), std::ios::binary);
			out << in.rdbuf();

			in.close();
			out.close();
		}

		return 0;
	}

	int StaticIoLibrary::io_fexists(IVirtualMachine *vm)
	{
		Var *path = vm->Pop();
		if (Var::IsStringType(path->GetDataType()) == false)
		{
			vm->PushInt(0);
			return 1;
		}

		struct stat info;
		if (stat(path->String()->GetBuffer(), &info) != 0 || info.st_mode & S_IFDIR)
			vm->PushInt(0);
		else
			vm->PushInt(1);

		return 0;
	}

	int StaticIoLibrary::io_dexists(IVirtualMachine *vm)
	{
		Var *path = vm->Pop();
		if (Var::IsStringType(path->GetDataType()) == false)
		{
			vm->PushInt(0);
			return 1;
		}

		struct stat info;
		if (stat(path->String()->GetBuffer(), &info) != 0 || info.st_mode & S_IFDIR == false)
			vm->PushInt(0);
		else
			vm->PushInt(1);

		return 0;
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