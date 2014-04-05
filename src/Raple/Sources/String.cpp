//-------------------------------------
#include "../Headers/String.h"
//-------------------------------------
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
//-------------------------------------


namespace Raple
{
	String::String()
	{
		_str = 0;
		_len = 0;
		_hashComputed = false;
		_hash = 0;
		_allocated = 0;
	}

	String::String(char c)
	{
		alloc(1);
		_str[_len] = 0;
		_str[0] = c;
	}

	String::String(const char *str)
	{
		int len = strlen(str);

		if(len >= 0)
		{
			alloc(len);
			strcpy_s(_str, _allocated, str);
			_str[_len] = 0;
		}
	}
	
	String::String(const String &str)
	{
		int len = str._len;

		if(len >= 0)
		{
			alloc(len);
			strcpy_s(_str, _allocated, str._str);
			_str[_len] = 0;
		}
	}

	String::~String()
	{
		Clear();
	}

	void String::Clear()
	{
		if (_str != 0 && _allocated > 0)
		{
			delete [_allocated] _str;
			_len = 0;
			_allocated = 0;
		}
	}

	const char *String::GetBuffer() const
	{
		return _str;
	}

	int String::Length() const
	{
		return _len;
	}

	int String::Format(const char *format, ...)
	{
		va_list args;
		va_start(args, format);

		char tmp[256];
		int size = _vsnprintf_s(tmp, 256, format, args);

		if (size > 0)
		{
			Clear();
			*this = tmp;
		}
		else
		{
			int n = 512;
			char *buffer = new char [n];

			while ((size = _vsnprintf_s(buffer, n, n-1, format, args)) < 0)
			{
				n *= 2;

				delete [](buffer);
				buffer = new char [n];
			}

			Clear();
			*this = buffer;
			delete [] buffer;
		}

		return size;
	}

	String String::Substring(int start, int length) const
	{
		if (length-start <= _len-start)
		{
			String s;
			s.alloc(length);

			strncpy_s(s._str, length+1, _str+start, length);
			return s;
		}

		return String("");
	}

	int String::ToInt() const
	{
		bool isValid = true;
		int i = 0;

		if (_str[i] == '-')
			++i;

		for (; i < _len; ++i)
			if (_str[i] > '9' || _str[i] < '0')
			{
				isValid = false;
				break;
			}

		if (!isValid || _len > 8)
			return 0;
			
		return atoi(_str);
	}

	float String::ToFloat() const
	{
		bool isValid = true;
		int dotCount = 0;

		for (int i = 0; i < _len; ++i)
			if (_str[i] > '9' || _str[i] < '0')
			{
				if (_str[i] == '.')
					++dotCount;
				else
				{
					isValid = false;
					break;
				}
			}

		if (!isValid || dotCount > 1 || _len > 10)
			return 0.f;

		return atof(_str);
	}

	unsigned int String::Hash() const
	{
		if (_len == 0)
			return 0;

		if (_hashComputed)
			return _hash;

		_hash = _len;
		unsigned int step = (_len >> 5) + 1;

		for (unsigned int i = _len; i >= step; i -= step)
			_hash ^= ((_hash << 5) | (_hash >> 2) + _str[i-1]);

		_hashComputed = true;

		return _hash;
	}

	char &String::operator [](int index)
	{
		return _str[index];
	}

	const char &String::operator [](int index) const
	{
		return _str[index];
	}

	String &String::operator = (const String &right)
	{
		if (this != &right)
		{
			Clear();
			alloc(right._len);

			strcpy_s(_str, _len+1, right._str);
			_str[_len] = 0;
		}

		return *this;
	}

	String &String::operator += (const String &right)
	{
		int oldLength = _len;
		char *tmp = new char [oldLength+1];
		tmp[oldLength] = 0;
		strcpy_s(tmp, oldLength+1, _str);

		Clear();
		alloc(oldLength + right._len);
		
		strcpy_s(_str, _len+1, tmp);
		strcat_s(_str, _len+1, right._str);
		_str[_len] = 0;
		delete []tmp;

		return *this;
	}

	inline void String::alloc(int size)
	{
		_len = size;
		_str = new char [_len+1];
		_hashComputed = false;
		_hash = 0;
		_allocated = _len+1;
	}

	String *String::Clone() const
	{
		return new String(*this);
	}

	int String::IndexOf(char c) const
	{
		for (int i = 0; i < _len; ++i)
			if (_str[i] == c)
				return i;

		return -1;
	}

	int String::IndexOf(unsigned int begin, char c) const
	{
		for (unsigned int i = begin; i < _len; ++i)
			if (_str[i] == c)
				return i;

		return -1;
	}

	//----------------------------------
	// Helper Operators
	//----------------------------------
	String operator + (const String &left, const String &right)
	{
		String tmp = left;
		tmp += right;
		return tmp;
	}

	bool operator == (const String &left, const String &right)
	{
		return strcmp(left.GetBuffer(), right.GetBuffer()) == 0;
	}

	bool operator != (const String &left, const String &right)
	{
		return strcmp(left.GetBuffer(), right.GetBuffer()) != 0;
	}
}