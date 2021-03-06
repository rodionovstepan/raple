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
	return "sub main() {var x;}";
}

class ConstSourceCodeProvider : public ISourceCodeProvider
{
public:
		
	SourceCode *GetSourceCode() const
	{
		return new SourceCode(getSourceCode());
	}
};

int main(int argc, char **argv)
{
	RapleEngine engine;
	
	StdLibrary stdlib;
	StringsLibrary strlib;

	StaticMathLibrary mathlib;
	StaticIoLibrary iolib;

	//ConstSourceCodeProvider provider;
	FileSourceCodeProvider provider("d:\\tmp\\raple.r");

	engine.RegisterLibrary(&stdlib);
	engine.RegisterLibrary(&strlib);
	engine.RegisterStaticLibrary(&mathlib);
	engine.RegisterStaticLibrary(&iolib);

	engine.ExecuteScript(&provider);

	return 0;
}