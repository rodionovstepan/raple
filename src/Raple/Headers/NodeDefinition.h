#ifndef __NODE_DEFINITION_H_INCLUDED__
#define __NODE_DEFINITION_H_INCLUDED__


//-------------------------------------
//-------------------------------------


namespace Raple
{
	enum NodeType
	{
		ntUnknown,
		ntCode,
		ntSub,
		ntAnonymousSub,
		ntIdentifier,
		ntFunctionDefinitionArgList,
		ntFunctionCallArgList,
		ntStatementsBlock,
		ntAssignment,
		ntVarDeclaration,
		ntExpression,
		ntValue,
		ntBinaryOperator,
		ntPreOperator,
		ntPostOperator,
		ntExpressionValue,
		ntFunctionCall,
		ntConstant,
		ntExpressionOperator,
		ntReturn,
		ntIfElse,
		ntIndexing,
		ntArrayInitList,
		ntWhile,
		ntImport,
		ntForEach,
		ntRange
	};

	const char *GetTreeNodeDefinition(NodeType nodeType);
}


#endif //__NODE_DEFINITION_H_INCLUDED__