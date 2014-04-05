#ifndef __STACK_H_INCLUDED__
#define __STACK_H_INCLUDED__


//-------------------------------------
#include "Constants.h"
#include "String.h"
//-------------------------------------


namespace Raple
{
	template <class T>
	class Stack
	{
		T *_base;
		T *_ptr;

		int _size;
		unsigned int _maxsize;

	public:

		Stack<T>(unsigned int maxsize = Constants::StackSize)
		{
			_maxsize = maxsize;
			_base = new T [_maxsize];
			_ptr = _base;
			_size = 0;
		}
		
		~Stack<T>()
		{
			delete [_maxsize] _base;
		}

		void Push(T t)
		{
			if (_size == _maxsize)
				throw StackOverflowException("OverflowException");

			*_ptr = t;
			++_ptr;
			++_size;
		}

		T Pop()
		{
			if (_size == 0)
				throw StackUnderflowException("UnderflowException");

			--_ptr;
			--_size;
			return *_ptr;
		}

		T Top()
		{
			if (_size == 0)
				throw StackUnderflowException("UnderflowException");

			return *(_ptr-1);
		}

		int Size() const 
		{
			return _size;
		}
	};

	class StackException
	{
		rstring _message;
	protected:

		StackException(const rstring &message)
			: _message(message)
		{
		}

	public:
		const rstring &Message()
		{
			return _message;
		}
	};

	class StackUnderflowException : public StackException
	{
	public:

		StackUnderflowException(const rstring &message)
			: StackException(message)
		{
		}
	};

	class StackOverflowException : public StackException
	{
	public:

		StackOverflowException(const rstring &message)
			: StackException(message)
		{
		}
	};
}



#endif //__STACK_H_INCLUDED__