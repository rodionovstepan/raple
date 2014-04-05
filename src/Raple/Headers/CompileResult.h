#ifndef __COMPILE_RESULT_H_INCLUDED__
#define __COMPILE_RESULT_H_INCLUDED__


namespace Raple
{
	enum CompileResult
	{
		crSuccess,
		crNullTree,
		crNullNode,
		crUnknownNode,
		crNullEnvironment,
		crNullSourceCode,
		crNullVm,
		crUnknownToken,
		crFailed
	};

}


#endif //__COMPILE_RESULT_H_INCLUDED__