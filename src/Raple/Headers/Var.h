#ifndef __VAR_H_INCLUDED__
#define __VAR_H_INCLUDED__


//-------------------------------------
#include "String.h"
#include "TokenDefinition.h"
//-------------------------------------


namespace Raple
{
	class HashArray;
	class Sub;

	enum DataType
	{
		dtNull,
		dtInteger,
		dtFloat,
		dtString,
		dtCallArgument,
		dtArray,
		dtFunction
	};

	static const char *GetDataTypeDefinition(DataType dt)
	{
		switch (dt)
		{
		case dtNull:
			return "<Null>";
		case dtInteger:
			return "<Integer>";
		case dtFloat:
			return "<Float>";
		case dtString:
			return "<String>";
		case dtCallArgument:
			return "<Argument>";
		case dtArray:
			return "<Array>";
		}

		return "<???>";
	}

	class Var
	{
	public:

		Var();
		Var(const rstring &name, DataType dataType = dtNull);
		~Var();

		void Int(int value);
		void Float(float value);
		void String(const rstring &value);
		void Array(const HashArray *array);
		void CallArgument(int id);
		void Function(int id);

		int Int() const 
		{
			if (_dataType == dtFloat)
				return (int)_float;

			return _int;
		}

		float Float() const 
		{
			if (_dataType == dtInteger)
				return (float) _int;

			return _float; 
		}

		rstring *String() const { return _string; }
		HashArray *Array() const { return _array; }
		void CreateArray();
		void Arg();
		bool IsArg() const { return _isArg; }

		const rstring &GetName() const { return _name; }
		DataType GetDataType() const { return _dataType; }

		void Null();
		void CopyFrom(Var *otherVar);

		Sub *GetInternalFunction(unsigned int hash);

		inline unsigned int GetHash() const
		{
			switch (_dataType)
			{
			case dtInteger:
				return (unsigned int)_int;
			case dtFloat:
				return (unsigned int)_float;
			case dtString:
				return _string->Hash();
			}
		
			return 0;
		}

		static bool IsNumericType(DataType dataType)
		{
			return dataType == dtInteger || dataType == dtFloat;
		}

		static bool IsStringType(DataType dataType)
		{
			return dataType == dtString;
		}

	private:

		union
		{
			int _int;
			float _float;
			rstring *_string;
			HashArray *_array;
		};

		rstring _name;
		DataType _dataType;
		bool _isReference;
		bool _isArg;

		inline void clearDynamicData();

	public:

		static Var *CreateInt(int i)
		{
			Var *var = new Var();
			var->Int(i);
			return var;
		}

		static Var *CreateFloat(float f)
		{
			Var *var = new Var();
			var->Float(f);
			return var;
		}

		static Var *CreateString(const rstring &s)
		{
			Var *var = new Var();
			var->String(s);
			return var;
		}

		static Var *CreateNull()
		{
			Var *var = new Var();
			var->Null();
			return var;
		}

		static Var *CreateArray(const HashArray *array)
		{
			Var *var = new Var();
			var->Array(array);
			return var;
		}

		static Var *From(Var *otherVar)
		{
			Var *var = new Var();
			var->CopyFrom(otherVar);
			return var;
		}
	};
}



#endif //__VAR_H_INCLUDED__