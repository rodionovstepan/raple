#ifndef __SCRIPT_CODE_TESTS_H_INCLUDED__
#define __SCRIPT_CODE_TESTS_H_INCLUDED__


//-------------------------------------
#include "../CppTest/cpptest.h"
#include "../Raple/Headers/SourceCode.h"
//-------------------------------------
//-------------------------------------
#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif
//-------------------------------------
using Raple::SourceCode;
//-------------------------------------


namespace RapleTests
{
	class SourceCodeTests : public Test::Suite
	{
	public:

		SourceCodeTests()
		{
			addTest(SourceCodeTests::ReturnsZeroIndexesIfTextIsEmptyOrPositionIsOverLength);
			addTest(SourceCodeTests::ReturnsCorrectlyRowAndColumnAtPosition);
			addTest(SourceCodeTests::ReturnsCorrectPartOfCode);
		}

	private:

		void ReturnsZeroIndexesIfTextIsEmptyOrPositionIsOverLength()
		{
			const char *src = "";

			_sourceCode.SetCode(src);

			int row, col;
			_sourceCode.ConvertPositionToRowColumn(12, &row, &col);

			assertEquals(0, row);
			assertEquals(0, col);

			src = "shorttext";

			_sourceCode.SetCode(src);

			_sourceCode.ConvertPositionToRowColumn(12, &row, &col);

			assertEquals(0, row);
			assertEquals(0, col);
		}

		void ReturnsCorrectlyRowAndColumnAtPosition()
		{
			const char *src = "sub main()\n{\n\treturn 21;\n}";

			_sourceCode.SetCode(src);

			int row, col;
			_sourceCode.ConvertPositionToRowColumn(4, &row, &col);
			printf("----%d %d---", row, col);
			assertEquals(1, row);
			assertEquals(5, col);

			_sourceCode.ConvertPositionToRowColumn(14, &row, &col);

			assertEquals(3, row);
			assertEquals(2, col);

			src = "sub main() {";

			_sourceCode.SetCode(src);
			_sourceCode.ConvertPositionToRowColumn(12, &row, &col);

			assertEquals(1, row);
			assertEquals(13, col);
		}

		void ReturnsCorrectPartOfCode()
		{
			const char *src = "abbra caddabra";

			_sourceCode.SetCode(src);

			Raple::String part1 = _sourceCode.TakePart(6, 5);
			Raple::String part2 = _sourceCode.TakePart(10, 4);

			assertEquals(Raple::String("cadda"), part1.GetBuffer());
			assertEquals(Raple::String("abra"), part2.GetBuffer());
		}

		SourceCode _sourceCode;
	};
}


#endif //__SCRIPT_CODE_TESTS_H_INCLUDED__