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
	};
};


#endif //__STATIC_MATH_LIBRARY_H_INCLUDED__