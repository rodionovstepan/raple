//-------------------------------------
#include "../Headers/SourceCode.h"
//-------------------------------------
#include <string.h>
//-------------------------------------


namespace Raple
{
	SourceCode::SourceCode()
	{
		_lines = 0;
	}

	SourceCode::SourceCode(const char *source)
	{
		_lines = 0;

		SetCode(source);
	}

	SourceCode::~SourceCode()
	{
		Clear();
	}

	void SourceCode::Clear()
	{
		_source.Clear();

		if (_lines != 0)
			delete [] _lines;

		_lines = 0;
	}

	const char *SourceCode::GetCode() const
	{
		return(_source.GetBuffer());
	}

	int SourceCode::GetCodeLength() const
	{
		return(_source.Length());
	}

	void SourceCode::SetCode(const char *sourceCode)
	{
		Clear();

		_source = sourceCode;

		// Для каждой позиции заполним номерами строк
		_lines = new int [_source.Length()+1];

		int n = 0;
		for(int i = 0; i <= _source.Length(); ++i)
			_lines[i] = _source[i] == '\n' ? n++ : n;
	}

	void SourceCode::ConvertPositionToRowColumn(int position, int *row, int *column) const
	{
		if(row != 0)
		{
			if (_source.Length() >= position)
				*row = _lines[position]+1;
			else
				*row = 0;
		}

		if(column != 0)
		{
			if (_source.Length() >= position)
			{
				int n = 0;
				for(int i = position-1; i >= 0; --i, ++n)
					if(_lines[position] != _lines[i])
						break;

				*column = n+1;
			}
			else
				*column = 0;
		}
	}

	rstring SourceCode::TakePart(unsigned int start, unsigned int length) const
	{
		return _source.Substring(start, length);
	}
}