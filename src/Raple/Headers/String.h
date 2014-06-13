#ifndef __R_STRING_H_INCLUDED__
#define __R_STRING_H_INCLUDED__


namespace Raple
{
	class String
	{
		char *_str;
		int _len;
		int _allocated;

		mutable bool _hashComputed;
		mutable unsigned int _hash;

	public:
		String();
		String(char c);
		String(const char *str);
		String(const String &str);
		~String();

		//!
		String &operator = (const String &right);
		String &operator += (const String &right);

		//!
		void Clear();
		const char *GetBuffer() const;
		int Length() const;
		char &operator [](int index);
		const char &operator [](int index) const;

		//!
		int Format(const char *format, ...);
		String Substring(int start, int length) const;

		int ToInt() const;
		float ToFloat() const;

		unsigned int Hash() const;
		String *Clone() const;

		int LastIndexOf(char c) const;
		int IndexOf(char c) const;
		int IndexOf(unsigned int begin, char c) const;

	private:
		inline void alloc(int size);
	};

	//! Helper Operators
	String operator + (const String &left, const String &right);
	bool operator == (const String &left, const String &right);
	bool operator != (const String &left, const String &right);

	typedef String rstring;
}


#endif //__R_STRING_H_INCLUDED__