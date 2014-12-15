//-------------------------------------
#include "../Headers/HashArray.h"
//-------------------------------------
//-------------------------------------


namespace Raple
{
	HashArray::HashArray()
	{
		_size = 0;
		_itemCount = 0;
		_items = 0;
		_firstFreeItem = 0;
	}

	HashArray::~HashArray()
	{
		if (_items == 0)
			return;

		delete [] _items;
	}

	void HashArray::growSize()
	{
		HashArrayEntry *oldItems = _items;
		unsigned int oldItemCount = _itemCount;

		_size = _size == 0 ? 4 : _size * 2;
		_itemCount = 0;

		_items = new HashArrayEntry [_size];
		_firstFreeItem = _items + _size - 1;

		if (oldItemCount != 0)
		{
			for (unsigned int i = 0; i < oldItemCount; ++i)
				Set(oldItems[i].GetKey(), oldItems[i].GetValue());

			delete []oldItems;
		}
	}

	Var *HashArray::Get(Var *key)
	{
		if (_size == 0)
			return 0;

		HashArrayEntry *item = getItem(key);
		if (item->GetKey() == 0)
			return 0;

		do
		{
			if (item->GetKey()->GetHash() == key->GetHash())
				return item->GetValue();

			item = item->GetNext();
		}
		while (item != 0);

		return 0;
	}

	void HashArray::Set(Var *key, Var *value)
	{
		if (_size == _itemCount)
			growSize();

		HashArrayEntry *node0 = getItem(key);
		HashArrayEntry *node = node0;

		do
		{
			Var *nodekey = node->GetKey();
			if (nodekey != 0 && nodekey->GetHash() == key->GetHash())
			{
				node->SetValue(value);
				return;
			}

			node = node->GetNext();
		}
		while (node != 0);

		if (node0->GetKey() != 0)
		{
			HashArrayEntry *otherEntry = getItem(node0->GetKey());
			if (otherEntry != node0)
			{
				while (otherEntry->GetNext() != node0)
					otherEntry = otherEntry->GetNext();

				otherEntry->SetNext(_firstFreeItem);
				*_firstFreeItem = *node0;
				node0->SetNext(0);
			}
			else
			{
				_firstFreeItem->SetNext(node0->GetNext());
				node0->SetNext(_firstFreeItem);
				node0 = _firstFreeItem;
			}
		}

		node0->SetKey(key);
		node0->SetValue(value);
		++_itemCount;

		for (;;)
		{
			if (_firstFreeItem->GetKey() == 0)
				return;
			else if (_firstFreeItem == _items)
				return;
			
			--_firstFreeItem;
		}
	}

	void HashArray::Add(Var *value)
	{
		Set(Var::CreateInt(_itemCount), value);
	}

	inline HashArrayEntry *HashArray::getItem(Var *key) const
	{
		return _items + (key->GetHash() % _size);
	}
}