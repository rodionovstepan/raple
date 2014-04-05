//-------------------------------------
#include "TokenizerTests.h"
#include "ParserTests.h"
#include "SourceCodeTests.h"
#include "BytecodeTests.h"
#include "CompilerTests.h"
#include "RuntimeEnvironmentTests.h"
#include "VirtualMachineTests.h"
#include "StackTests.h"
#include "HashArrayTests.h"
//-------------------------------------
#include "../Raple/Sources/InternalFunctionsLibrary.cpp"
//-------------------------------------
#include <memory>
//-------------------------------------
#pragma comment(lib, "cppTest/CppTest.lib")
//-------------------------------------
#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif
//-------------------------------------
using std::auto_ptr;
using std::cin;
//-------------------------------------

int main()
{
	using Test::Suite;
	Suite testSuite;

	testSuite.add(auto_ptr<Suite>(new RapleTests::TokenizerTests()));
	testSuite.add(auto_ptr<Suite>(new RapleTests::ParserTests()));
	testSuite.add(auto_ptr<Suite>(new RapleTests::SourceCodeTests()));
	testSuite.add(auto_ptr<Suite>(new RapleTests::BytecodeTests()));
	testSuite.add(auto_ptr<Suite>(new RapleTests::CompilerTests()));
	testSuite.add(auto_ptr<Suite>(new RapleTests::RuntimeEnvironmentTests()));
	testSuite.add(auto_ptr<Suite>(new RapleTests::VirtualMachineTests()));
	testSuite.add(auto_ptr<Suite>(new RapleTests::StackTests()));
	testSuite.add(auto_ptr<Suite>(new RapleTests::HashArrayTests()));

	auto_ptr<Test::Output> output(new Test::TextOutput(Test::TextOutput::Verbose));
	testSuite.run(*output, true);

	cin.get();

	return 0;
}