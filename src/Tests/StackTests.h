#ifndef __STACK_TESTS_H_INCLUDED__
#define __STACK_TESTS_H_INCLUDED__


//-------------------------------------
#include "../CppTest/cpptest.h"
#include "../Raple/Headers/Stack.h"
//-------------------------------------
//-------------------------------------
#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif
//-------------------------------------
using Raple::Stack;
using Raple::StackOverflowException;
using Raple::StackUnderflowException;
//-------------------------------------


namespace RapleTests
{
	class StackTests : public Test::Suite
	{
	public:

		StackTests()
		{
			addTest(StackTests::SizeTest);
			addTest(StackTests::PushAndPopTest);
			addTest(StackTests::OverflowExceptionTest);
			addTest(StackTests::UnderflowExceptionTest);
		}

	private:

		void SizeTest()
		{
			Stack<int> s;

			assertEquals(0, s.Size());
			
			s.Push(1);
			s.Push(2);
			assertEquals(2, s.Size());
			
			s.Pop();
			assertEquals(1, s.Size());
			
			s.Pop();
			assertEquals(0, s.Size());
		}

		void PushAndPopTest()
		{
			Stack<int> s;

			s.Push(1);
			s.Push(2);
			s.Push(3);

			assertEquals(3, s.Pop());
			assertEquals(2, s.Pop());
			assertEquals(1, s.Pop());
		}

		void OverflowExceptionTest()
		{
			Stack<int> s(2);

			s.Push(1);
			s.Push(2);

			try
			{
				s.Push(3);
			}
			catch (StackOverflowException ex)
			{
				assert(true);
				return;
			}

			assert(false);
		}

		void UnderflowExceptionTest()
		{
			Stack<int> s;
			
			try
			{
				s.Pop();
			}
			catch (StackUnderflowException ex)
			{
				assert(true);
				return;
			}

			assert(false);
		}
	};
}


#endif //__STACK_TESTS_H_INCLUDED__