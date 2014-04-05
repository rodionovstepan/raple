#ifndef __RUNTIME_ENVIRONMENT_TESTS_H_INCLUDED__
#define __RUNTIME_ENVIRONMENT_TESTS_H_INCLUDED__



//-------------------------------------
#include "../CppTest/cpptest.h"
#include "../Raple/Headers/RuntimeEnvironment.h"
#include "../Raple/Sources/RuntimeEnvironment.cpp"
//-------------------------------------
//-------------------------------------
#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif
//-------------------------------------
using Raple::RuntimeEnvironment;
//-------------------------------------



namespace RapleTests
{
	class RuntimeEnvironmentTests : public Test::Suite
	{
	public:

		RuntimeEnvironmentTests()
		{
			addTest(RuntimeEnvironmentTests::TestForAddingAndFindingSubs);
		}

		void TestForAddingAndFindingSubs()
		{
			RuntimeEnvironment env;

			int main_idx = env.AddSub("main");
			int bar_idx = env.AddSub("bar");

			assertEquals(rstring("main"), env.GetSub(main_idx)->GetName());
			assertEquals(main_idx, env.FindSub("main"));

			assertEquals(rstring("bar"), env.GetSub(bar_idx)->GetName());
			assertEquals(bar_idx, env.FindSub("bar"));

			assertEquals(-1, env.FindSub("radiogaga"));
		}
	};
}




#endif //__RUNTIME_ENVIRONMENT_TESTS_H_INCLUDED__