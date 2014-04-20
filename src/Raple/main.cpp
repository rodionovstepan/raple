//-------------------------------------
#include "Headers/RapleEngine.h"
#include "Headers/System.h"
#include "Headers/StdLibrary.h"
#include "Headers/StringsLibrary.h"
#include "Headers/FileSourceCodeProvider.h"
#include "Headers/StaticMathLibrary.h"
#include "Headers/StaticIoLibrary.h"
//#include "vld.h"
//-------------------------------------
#include <stdio.h>
//-------------------------------------
//#pragma comment(lib, "vld.lib")
//-------------------------------------
using namespace Raple;
using namespace RapleLibraries;
//-------------------------------------

static const char *getSourceCode()
{
	return "sub main() { var x=true; if(x == false) io.println('yes'); }";
}

int main(int argc, char **argv)
{
	class ConstSourceCodeProvider : public ISourceCodeProvider
	{
	public:
		
		SourceCode *GetSourceCode() const
		{
			return new SourceCode(getSourceCode());
		}
	};

	RapleEngine engine;
	
	StdLibrary stdlib;
	StringsLibrary strlib;

	StaticMathLibrary mathlib;
	StaticIoLibrary iolib;

	//FileSourceCodeProvider provider(argc, argv);
	ConstSourceCodeProvider provider;

	engine.RegisterLibrary(&stdlib);
	engine.RegisterLibrary(&strlib);
	engine.RegisterStaticLibrary(&mathlib);
	engine.RegisterStaticLibrary(&iolib);

	engine.ExecuteScript(&provider);

	return 0;
}