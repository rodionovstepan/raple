#ifndef __CONSTANTS_H_INCLUDED__
#define __CONSTANTS_H_INCLUDED__


//-------------------------------------
#include "String.h"
//-------------------------------------


namespace Raple
{
	namespace Constants
	{
		const int TreeIndexCode = 0;

		const int TreeIndexSubName = 0;
		const int TreeIndexSubArguments = 1;
		const int TreeIndexSubBody = 2;

		const int TreeIndexAnonymousSubArguments = 0;
		const int TreeIndexAnonymousSubBody = 1;

		const int TreeIndexFunctionCallName = 0;
		const int TreeIndexFunctionCallInternalCall = 1;
		const int TreeIndexFunctionCallArguments = 1;

		const int TreeIndexAssignmentAssignee = 0;
		const int TreeIndexAssignmentValue = 1;

		const int TreeIndexIfElseCondition = 0;
		const int TreeIndexIfElseIfStatements = 1;
		const int TreeIndexIfElseElseStatements = 2;

		const int TreeIndexWhileCondition = 0;
		const int TreeIndexWhileStatements = 1;

		const int TreeIndexArrayIndexingArray = 0;
		const int TreeIndexArrayIndexingKey = 1;

		const int TreeIndexVarDeclarationIdentifier = 0;
		const int TreeIndexVarDeclarationValue = 1;
		const int TreeIndexVarDeclarationAdditionalNode = 2;

		const int TreeIndexImportLibName = 0;

		const rstring LogTitleParserError = "Parsing error";
		const rstring LogTitleParserWarning = "Parsing warning";
		const rstring LogTitleCompilerError = "Compilation error";
		const rstring LogTitleCompilerWarning = "Compilation warning";
		const rstring LogTitleValidationError = "Validation";
		const rstring LogTitleVirtualMachineError = "VM";
		const rstring LogTitleRapleEngine = "Raple Engine";

		const int EnvironmentUnknownSubId = -1;
		const rstring EnvironmentLocalsPrefix = "lcl`_";
		const rstring EnvironmentMainSubName = "main";

		const int SubUnknownLocalId = -1;

		const unsigned int StackSize = 1024;
	}
}


#endif //__CONSTANTS_H_INCLUDED__