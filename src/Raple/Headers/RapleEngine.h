#ifndef __RAPLE_ENGINE_H_INCLUDED__
#define __RAPLE_ENGINE_H_INCLUDED__


//-------------------------------------
#include "IRapleEngine.h"
#include "ILogger.h"
#include "IParser.h"
#include "ITokenizer.h"
#include "IRuntimeEnvironment.h"
#include "IVirtualMachine.h"
#include "ICompiler.h"
#include "ILibrary.h"
#include "ISourceCodeProvider.h"
#include "InternalFunctionsLibrary.h"
//-------------------------------------


namespace Raple
{
	class RapleEngine : public IRapleEngine
	{
		ILogger *_logger;
		ITokenizer *_tokenizer;
		IParser *_parser;

		IRuntimeEnvironment *_environment;
		IVirtualMachine *_virtualMachine;
		ICompiler *_compiler;

		InternalFunctionsLibrary *_internalsLibrary;

	public:

		RapleEngine();
		~RapleEngine();

		virtual void ExecuteScript(const ISourceCodeProvider *provider);
		virtual void RegisterLibrary(const ILibrary *registrator);
		virtual void RegisterStaticLibrary(const IStaticLibrary *library);

	private:

		void clearResourcesAndLog(SourceCode *code, TreeNode *node, const rstring &msg);
	};
}



#endif //__RAPLE_ENGINE_H_INCLUDED__