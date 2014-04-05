//-------------------------------------
#include "../Headers/RuntimeEnvironment.h"
//-------------------------------------


namespace Raple
{
	RuntimeEnvironment::~RuntimeEnvironment()
	{
		for (std::vector<Sub *>::iterator I = _subs.begin();
			I != _subs.end();
			++I)
			delete *I;

		_subs.clear();
	}

	int RuntimeEnvironment::AddSub(const rstring &name)
	{
		int idx = FindSub(name);

		if (idx != -1)
			return idx;

		Sub *sub = new Sub(name);
		_subs.push_back(sub);

		return _subs.size()-1;
	}

	int RuntimeEnvironment::AddAnonymousSub()
	{
		rstring name;
		name.Format("__anon_sub_%d", _subs.size());

		return AddSub(name);
	}

	Sub *RuntimeEnvironment::GetSub(int index)
	{
		if (index >= 0 && index < _subs.size())
			return _subs[index];

		return 0;
	}

	int RuntimeEnvironment::FindSub(const rstring &name)
	{
		unsigned int size = _subs.size();

		for (unsigned int i = 0; i < size; ++i)
			if (_subs[i]->GetName() == name)
				return i;

		return -1;
	}

	void RuntimeEnvironment::RegisterNativeFunction(const rstring &name, int argumentCount, NativeFunction nf)
	{
		if (FindSub(name) != -1)
			return;

		int idx = AddSub(name);

		Sub *sub = GetSub(idx);
		sub->SetArgumentsCount(argumentCount);
		sub->SetNativeFunction(nf);
	}

	bool RuntimeEnvironment::IsLibraryExists(const rstring &name)
	{
		return _knownLibs[name.Hash()] != 0;
	}

	bool RuntimeEnvironment::IsLibraryImported(const rstring &name)
	{
		return _importedLibs[name.Hash()] != 0;
	}

	void RuntimeEnvironment::ImportLibrary(const rstring &name)
	{
		unsigned int hash = name.Hash();
		
		IStaticLibrary *lib = _knownLibs[hash];
		_importedLibs[hash] = lib;
	}

	void RuntimeEnvironment::RegisterStaticLibrary(const IStaticLibrary *library)
	{
		_knownLibs[library->GetName().Hash()] = const_cast<IStaticLibrary *>(library);
	}

	Sub *RuntimeEnvironment::GetLibrarySub(unsigned int library, unsigned int sub)
	{
		return _importedLibs[library]->GetSub(sub);
	}
}