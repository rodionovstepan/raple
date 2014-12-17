#ifndef __OPCODE_H_INCLUDED__
#define __OPCODE_H_INCLUDED__


//-------------------------------------
//-------------------------------------


namespace Raple
{
	enum Opcode
	{
		opNop,

		/* use constants */
		opPushInt,
		opPushFloat,
		opPushString,

		opAdd,
		opSub,
		opProd,
		opDiv,
		opExp,
		opNeg,
		opRet,
		opEqual,
		opNotEqual,
		opGreater,
		opLess,
		opGreaterOrEqual,
		opLessOrEqual,
		opLogicAnd,
		opLogicOr,
		opPostInc,
		opPostDec,
		opPreInc,
		opPreDec,

		opNewArray,
		opGetElement,
		opSetElement,
		opArraySize,
		opGetNextElement,

		/* use argument */
		opSetLocal,
		opGetLocal,
		opCall,
		opInternalCall,
		opStaticCall,
		opDynamicCall,
		opGoto,
		opJumpZero,
		opJumpNotZero,
		opGetSub
	};

	static const char *GetOpcodeDefinition(Opcode op)
	{
		switch (op)
		{
		case opNop:
			return "<nop>";
		case opPushInt:
			return "<pushi>";
		case opPushFloat:
			return "<pushf>";
		case opPushString:
			return "<pushs>";
		case opAdd:
			return "<add>";
		case opSub:
			return "<sub>";
		case opDiv:
			return "<div>";
		case opProd:
			return "<prod>";
		case opExp:
			return "<exp>";
		case opNeg:
			return "<neg>";
		case opRet:
			return "<ret>";
		case opEqual:
			return "<eq>";
		case opNotEqual:
			return "<neq>";
		case opGreater:
			return "<grtr>";
		case opLess:
			return "<lss>";
		case opGreaterOrEqual:
			return "<grtreq>";
		case opLessOrEqual:
			return "<lsseq>";
		case opSetLocal:
			return "<sloc>";
		case opGetLocal:
			return "<gloc>";
		case opCall:
			return "<call>";
		case opGoto:
			return "<goto>";
		case opJumpNotZero:
			return "<jmpnz>";
		case opJumpZero:
			return "<jmpz>";
		case opArraySize:
			return "<arrsize>";
		case opNewArray:
			return "<newarr>";
		case opGetElement:
			return "<getelem>";
		case opSetElement:
			return "<setelem>";
		case opGetNextElement:
			return "<getnextelem>";
		case opInternalCall:
			return "<intcall>";
		case opStaticCall:
			return "<statcall>";
		case opDynamicCall:
			return "<dyncall>";
		case opPostInc:
			return "<postinc>";
		case opPostDec:
			return "<postdec>";
		}

		return "<???>";
	}
}


#endif //__OPCODE_H_INCLUDED__