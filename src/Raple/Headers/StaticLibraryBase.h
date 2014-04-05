#ifndef __STATIC_LIBRARY_BASE_H_INCLUDED__
#define __STATIC_LIBRARY_BASE_H_INCLUDED__


//-------------------------------------
#include "IStaticLibrary.h"
#include "Sub.h"
//-------------------------------------
#include <map>
//-------------------------------------


namespace Raple
{
	class StaticLibraryBase : public IStaticLibrary
	{
	protected:
		std::map<rhash, Sub *> _subs;

		void registerSub(const rstring &name, unsigned int argCount, NativeFunction native)
		{
			_subs[name.Hash()] = new Sub(name, argCount, native);
		}

	public:

		virtual ~StaticLibraryBase()
		{
			for (auto i = _subs.begin(); i != _subs.end(); ++i)
				delete (*i).second;
		}

		virtual rstring GetName() const = 0;

		virtual Sub *GetSub(rhash hash)
		{
			return _subs[hash];
		}
	};
};



#endif //__STATIC_LIBRARY_BASE_H_INCLUDED__