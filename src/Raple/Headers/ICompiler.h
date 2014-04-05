#ifndef __I_COMPILER_H_INCLUDED__
#define __I_COMPILER_H_INCLUDED__


//-------------------------------------
#include "IRuntimeEnvironment.h"
#include "IVirtualMachine.h"
#include "CompileResult.h"
#include "SourceCode.h"
#include "TreeNode.h"
//-------------------------------------


namespace Raple
{
	class ICompiler
	{
	public:

		virtual ~ICompiler()
		{
		}

		virtual CompileResult CompileTree(TreeNode *root,
										  SourceCode *sourceCode,
										  IVirtualMachine *vm) = 0;
	};
}



#endif //__I_COMPILER_H_INCLUDED__