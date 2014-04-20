#ifndef __STATIC_MATH_LIBRARY_H_INCLUDED__
#define __STATIC_MATH_LIBRARY_H_INCLUDED__


//-------------------------------------
#include "StaticLibraryBase.h"
#include "Sub.h"
//-------------------------------------
#include <map>
#include <math.h>
//-------------------------------------
using namespace Raple;
//-------------------------------------


namespace RapleLibraries
{
	class StaticMathLibrary : public StaticLibraryBase
	{
	public:

		StaticMathLibrary()
		{
			registerSub("sin", 1, StaticMathLibrary::math_sin);
			registerSub("fib", 1, StaticMathLibrary::math_fib);
		}

		~StaticMathLibrary()
		{
		}

		virtual rstring GetName() const
		{
			return "math";
		}

	private:

		static int math_sin(IVirtualMachine *vm)
		{
			Var *v = vm->Pop();
			if (Var::IsNumericType(v->GetDataType()))
				vm->PushFloat(sin(v->Float()));
			else
				vm->PushNull();

			return 0;
		}

		static int math_fib(IVirtualMachine *vm)
		{
			Var *v = vm->Pop();
			if (Var::IsNumericType(v->GetDataType()))
				vm->PushInt(_math_fib(v->Int()));
			else
				vm->PushNull();

			return 0;
		}

		static int _math_fib(int n)
		{
			if (n == 0 || n == 1)
				return n;

			return _math_fib(n-1) + _math_fib(n-2);
		}
	};
};


#endif //__STATIC_MATH_LIBRARY_H_INCLUDED__