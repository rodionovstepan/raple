//-------------------------------------
#include "../Headers/Var.h"
#include "../Headers/HashArray.h"
#include "../Headers/Sub.h"
#include "../Headers/InternalFunctionsLibrary.h"
//-------------------------------------
#include <cstdlib>
//-------------------------------------


namespace Raple
{
	Var::Var()
	{
		_dataType = dtNull;
		_isReference = false;
	}

	Var::Var(const rstring &name, DataType dataType)
	{
		_name = name;
		_dataType = dataType;
		_isReference = false;

		switch (dataType)
		{
		case dtInteger:
			_int = 0;
			break;
		case dtFloat:
			_float = 0.f;
			break;
		}
	}

	Var::~Var()
	{
		clearDynamicData();
	}

	inline void Var::clearDynamicData()
	{
		if (_dataType == dtString)
			delete _string;
		else
			if (_dataType == dtArray && !_isReference)
				delete _array;

		_dataType = dtNull;
	}

	void Var::Null()
	{
		clearDynamicData();
	}

	void Var::Int(int value)
	{
		clearDynamicData();
		_dataType = dtInteger;
		_int = value;
		_isReference = false;
	}

	void Var::Float(float value)
	{
		clearDynamicData();
		_dataType = dtFloat;
		_float = value;
		_isReference = false;
	}

	void Var::String(const rstring &value)
	{
		clearDynamicData();
		_dataType = dtString;
		_string = new rstring(value);
		_isReference = false;
	}

	void Var::Array(const HashArray *array)
	{
		clearDynamicData();
		_dataType = dtArray;
		_array = const_cast<HashArray *>(array);
		_isReference = true;
	}

	void Var::CallArgument(int id)
	{
		clearDynamicData();
		_dataType = dtCallArgument;
		_int = id;
		_isReference = false;
	}

	void Var::Function(int id)
	{
		clearDynamicData();
		_dataType = dtFunction;
		_int = id;
		_isReference = true;
	}

	void Var::CreateArray()
	{
		clearDynamicData();
		
		_dataType = dtArray;
		_array = new HashArray();
		_isReference = false;
	}

	void Var::CopyFrom(Var *otherVar)
	{
		if (this == otherVar)
			return;

		clearDynamicData();
		_dataType = otherVar->_dataType;
		_isReference = otherVar->_isReference;
			
		switch (_dataType)
		{
		case dtInteger:
			_int = otherVar->_int;
			break;
		case dtFloat:
			_float = otherVar->_float;
			break;
		case dtString:
			_string = otherVar->_string->Clone();
			break;
		case dtArray:
			_array = otherVar->_array;
			break;
		case dtCallArgument:
			_int = otherVar->_int;
			break;
		}

		if (otherVar->_name.Length() > 0)
			_name = otherVar->_name;
		else
			_name.Clear();
	}

	Sub *Var::GetInternalFunction(unsigned int hash)
	{
		return InternalFunctionsLibrary::Library()->GetInternalFunction(_dataType, hash);
	}
}