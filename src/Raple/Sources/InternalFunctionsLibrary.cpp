//-------------------------------------
#include "../Headers/InternalFunctionsLibrary.h"
//-------------------------------------

namespace Raple
{
	InternalFunctionsLibrary::~InternalFunctionsLibrary()
	{
		for (auto I = _lib.begin(); I != _lib.end(); ++I)
			for (auto J = (*I).second.begin(); J != (*I).second.end(); ++J)
				delete (*J).second;

		_lib.clear();
	}

	InternalFunctionsLibrary *InternalFunctionsLibrary::Library()
	{
		if (_instance == 0)
			_instance = new InternalFunctionsLibrary();

		return _instance;
	}

	Sub *InternalFunctionsLibrary::GetInternalFunction(DataType dataType, unsigned int hash)
	{
		return _lib[dataType][hash];
	}

	void InternalFunctionsLibrary::RegisterAll()
	{
		registerFunction(dtString, "length", 0, InternalFunctionsLibrary::string_length);
		registerFunction(dtString, "indexOf", 1, InternalFunctionsLibrary::string_indexOf);
		registerFunction(dtString, "type", 0, InternalFunctionsLibrary::string_type);
		registerFunction(dtString, "toint", 0, InternalFunctionsLibrary::string_toint);
		registerFunction(dtString, "tostr", 0, InternalFunctionsLibrary::string_tostring);

		registerFunction(dtArray, "size", 0, InternalFunctionsLibrary::array_size);
		registerFunction(dtArray, "type", 0, InternalFunctionsLibrary::array_type);

		registerFunction(dtInteger, "type", 0, InternalFunctionsLibrary::int_type);
		registerFunction(dtInteger, "tostr", 0, InternalFunctionsLibrary::int_tostring);

		registerFunction(dtFloat, "type", 0, InternalFunctionsLibrary::float_type);
		registerFunction(dtFloat, "tostr", 0, InternalFunctionsLibrary::float_tostring);

		registerFunction(dtNull, "type", 0, InternalFunctionsLibrary::null_type);
		registerFunction(dtNull, "tostr", 0, InternalFunctionsLibrary::null_tostring);
	}

	InternalFunctionsLibrary *InternalFunctionsLibrary::_instance = 0;
}