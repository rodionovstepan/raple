#ifndef __SCRIPT_CODE_H_INCLUDED__
#define __SCRIPT_CODE_H_INCLUDED__


//-------------------------------------
#include "String.h"
//-------------------------------------


namespace Raple
{
	class SourceCode
	{
		int *_lines;
		rstring _source;

	public:

		SourceCode();
		SourceCode(const char *source);
		~SourceCode();

		void SetCode(const char *sourceCode);
		void ConvertPositionToRowColumn(int position, int *row, int *column) const;

		const char *GetCode() const;
		int GetCodeLength() const;

		rstring TakePart(unsigned int start, unsigned int length) const;

	private:

		void Clear();
	};
}


#endif //__SCRIPT_CODE_H_INCLUDED__