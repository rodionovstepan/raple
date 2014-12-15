#ifndef __ARRAY_H_INCLUDED__
#define __ARRAY_H_INCLUDED__


//-------------------------------------
#include "HashArrayEntry.h"
#include "System.h"
//-------------------------------------


namespace Raple
{
	class HashArray
	{
		unsigned int _size;
		unsigned int _itemCount;

		HashArrayEntry *_items;
		HashArrayEntry *_firstFreeItem;

	public:

		HashArray();
		~HashArray();

		Var *Get(Var *key);
		void Set(Var *key, Var *value);
		void Add(Var *value);

		unsigned int ItemCount() const { return _itemCount; }
		unsigned int Size() const { return _size; }

		const HashArrayEntry *GetByIndex(unsigned int index)
		{
			return &_items[index];
		}

	private:

		void growSize();
		inline HashArrayEntry *getItem(Var *key) const;
	};
}



#endif //__ARRAY_H_INCLUDED__