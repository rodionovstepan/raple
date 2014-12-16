#ifndef __HASH_ARRAY_ENTRY_H_INCLUDED__
#define __HASH_ARRAY_ENTRY_H_INCLUDED__


//-------------------------------------
#include "../Headers/System.h"
#include "../Headers/Var.h"
//-------------------------------------

namespace Raple
{
	class Var;

	class HashArrayEntry
	{
		Var *_key;
		Var *_value;
		HashArrayEntry *_next;

	public:

		HashArrayEntry()
			: _key(0), _value(0), _next(0)
		{
		}

		HashArrayEntry(Var *key, Var *value, HashArrayEntry *next = 0)
			: _next(next)
		{
			SetKey(key);
			SetValue(value);
		}

		~HashArrayEntry()
		{
			delete _key;
			delete _value;
		}

		Var *GetKey() const
		{
			return _key;
		}

		Var *GetValue() const
		{
			return _value;
		}

		HashArrayEntry *GetNext() const
		{
			return _next;
		}

		void SetKey(Var *key)
		{
			if (_key != 0)
				delete _key;

			_key = new Var();
			_key->CopyFrom(key);
		}

		void SetValue(Var *value)
		{
			if (_value != 0)
				delete _value;

			_value = new Var();
			_value->CopyFrom(value);
		}

		void SetNext(HashArrayEntry *next)
		{
			_next = next;
		}
	};
}



#endif //__HASH_ARRAY_ENTRY_H_INCLUDED__