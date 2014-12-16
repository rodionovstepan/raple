#ifndef __HASH_ARRAY_TESTS_H_INCLUDED__
#define __HASH_ARRAY_TESTS_H_INCLUDED__


//-------------------------------------
#include "../CppTest/cpptest.h"
#include "../Raple/Headers/HashArray.h"
#include "../Raple/Sources/HashArray.cpp"
//-------------------------------------
#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif
//-------------------------------------
using namespace Raple;
//-------------------------------------


namespace RapleTests
{
	class HashArrayTests : public Test::Suite
	{
	public:

		HashArrayTests()
		{
			addTest(HashArrayTests::Test1);
			addTest(HashArrayTests::Test2);
			addTest(HashArrayTests::Test3);
			addTest(HashArrayTests::ForEachTest1);
		}

	private:

		void Test1()
		{
			Var key1;
			key1.Int(0);
			HashArray array;
			
			Var *nullresult = array.Get(&key1);
			assertTrue(nullresult == 0);
		}

		void Test2()
		{
			Var key1;
			key1.Int(0);
			Var key2;
			key2.Int(1);

			Var val1;
			val1.Int(21);
			Var val2;
			val2.Int(22);
			
			HashArray array;
			
			array.Set(&key1, &val1);
			array.Set(&key2, &val2);

			Var *ret = array.Get(&key2);
			assertEquals(val2.GetDataType(), ret->GetDataType());
			assertEquals(val2.Int(), ret->Int());

			ret = array.Get(&key1);
			assertEquals(val1.GetDataType(), ret->GetDataType());
			assertEquals(val1.Int(), ret->Int());
		}

		void Test3()
		{
			Var key1;
			key1.Int(0);
			Var val1;
			val1.Int(2);

			HashArray array;
			array.Set(&key1, &val1);

			assertEquals(1, array.ItemCount());

			Var val2;
			val2.Int(3);
			array.Set(&key1, &val2);

			assertEquals(1, array.ItemCount());
		}

		void ForEachTest1()
		{
			Var k1, v1, k2, v2;
			k1.Int(0);
			v1.Int(1);
			k2.Int(1);
			v2.Int(2);

			HashArray array;
			array.Set(&k1, &v1);
			array.Set(&k2, &v2);

			size_t size = array.Size(),
				count = array.ItemCount(),
				i = 0, j = 0;

			for (; i < size && j < count; ++i) 
			{
				if (array.GetByIndex(i)->GetKey() != 0) ++j;
			}

			assertEquals(2, j);
		}
	};
}



#endif //__HASH_ARRAY_TESTS_H_INCLUDED__