#ifndef __CONVERTER_H_INCLUDED__
#define __CONVERTER_H_INCLUDED__


//-------------------------------------
#include "String.h"
//-------------------------------------
#include <stdlib.h>
//-------------------------------------


namespace Raple
{
	class Convert
	{
	public:

		static rstring IntToString(int i)
		{ 
			char buf[32];
			_itoa_s(i, buf, 32, 10);

			return rstring(buf);
		}
	};
}



#endif //__CONVERTER_H_INCLUDED__