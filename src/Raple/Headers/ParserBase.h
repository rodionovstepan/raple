#ifndef __PARSER_BASE_H_INCLUDED__
#define __PARSER_BASE_H_INCLUDED__


//-------------------------------------
#include "ITokenizer.h"
#include "ILogger.h"
#include "IParser.h"
//-------------------------------------


namespace Raple
{
	class ParserBase : public IParser
	{
	protected:

		ITokenizer *const _tokenizer;
		const ILogger *_logger;
		bool _errorsWhileParsing;

	public:

		ParserBase(ITokenizer *const tokenizer, const ILogger *logger)
			: _tokenizer(tokenizer), _logger(logger), _errorsWhileParsing(false)
		{
		}

		virtual ~ParserBase()
		{
		}

		bool HasErrorsWhileParsing() const
		{
			return _errorsWhileParsing;
		}
	};
}


#endif //__I_PARSER_H_INCLUDED__