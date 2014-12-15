//-------------------------------------
#include "../Headers/RapleEngine.h"
//-------------------------------------
#include "../Headers/ConsoleLogger.h"
#include "../Headers/Tokenizer.h"
#include "../Headers/Parser.h"
#include "../Headers/VirtualMachine.h"
#include "../Sources/VirtualMachine.cpp"
#include "../Headers/RuntimeEnvironment.h"
#include "../Headers/Compiler.h"
#include "../Headers/SourceCode.h"
//-------------------------------------


namespace Raple
{
	void Shift(int level)
	{
		for(int i = 0; i < level; ++i)
			printf("|  ");
	}

	void PrintNodeData(TreeNode *item, int level, const rstring &code)
	{
		Shift(level);

		printf("%s: %s: %d %d ", GetTreeNodeDefinition(item->GetType()), 
								  GetTokenDefinition(item->GetToken()->Type),
								  item->GetToken()->Position,
								  item->GetToken()->Length);

		if (item->GetToken()->Length <= 4)
			printf("'%s'\n", code.Substring(item->GetToken()->Position, item->GetToken()->Length).GetBuffer());
		else
			printf("\n");
	}

	void PrintTree(TreeNode *item, int level = 0, const rstring &code = "")
	{
		TreeNode *node = item;

		while(node)
		{
			PrintNodeData(node, level, code);
			PrintTree(node->GetChild(0), level+1, code);

			node = node->Next;
		}
	}
	
	RapleEngine::RapleEngine()
	{
		_logger = new ConsoleLogger();
		_tokenizer = new Tokenizer();
		_parser = new Parser(_tokenizer, _logger);

		_environment = new RuntimeEnvironment();
		_virtualMachine = new VirtualMachine(_environment, _logger);
		_compiler = new Compiler(_logger);

		_internalsLibrary = InternalFunctionsLibrary::Library();
		_internalsLibrary->RegisterAll();
	}

	RapleEngine::~RapleEngine()
	{
		delete _compiler;
		delete _virtualMachine;
		delete _environment;
		delete _parser;
		delete _tokenizer;
		delete _logger;
		delete _internalsLibrary;
	}

	void RapleEngine::ExecuteScript(const ISourceCodeProvider *provider)
	{
		SourceCode *code = provider->GetSourceCode();
		if (code == 0)
		{
			clearResourcesAndLog(0, 0, "Null source code");
			return;
		}

		TreeNode *root = _parser->ParseScript(code);
		if (root == 0 || _parser->HasErrorsWhileParsing())
		{
			clearResourcesAndLog(code, 0, "Parsing source code failed.");
			return;
		}

		_environment->ImportLibrary("io");

		CompileResult compileResult = _compiler->CompileTree(root, code, _virtualMachine);
		if (compileResult != crSuccess)
		{
			clearResourcesAndLog(code, root, "Compilation failed.");
			return;
		}

		Sub *main = _environment->GetSub(_environment->FindSub("main"));
		if (main == 0)
		{
			clearResourcesAndLog(code, root, "Entry point (sub 'main') not found in source code.");
			return;
		}

		VirtualMachineResult result = _virtualMachine->Execute(main);
		if (result != vmrSuccess)
		{
			clearResourcesAndLog(code, root, "Virtual machine executing failed.");
			return;
		}
		
		clearResourcesAndLog(code, root, "");
	}

	void RapleEngine::RegisterLibrary(const ILibrary *registrator)
	{
		registrator->Register(_environment);
	}

	void RapleEngine::RegisterStaticLibrary(const IStaticLibrary *library)
	{
		_environment->RegisterStaticLibrary(library);
	}

	void RapleEngine::clearResourcesAndLog(SourceCode *code, TreeNode *node, const rstring &msg)
	{
		if (code != 0)
			delete code;

		if (node != 0)
			delete node;

		if (msg != "")
			_logger->Error(Constants::LogTitleRapleEngine, msg);
	}
}