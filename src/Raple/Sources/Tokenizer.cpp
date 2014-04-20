//-------------------------------------
#include "../Headers/Tokenizer.h"
//-------------------------------------
#include <string.h>
//-------------------------------------

namespace Raple
{
	TokenType Tokenizer::GetTokenType(const char *source, int length, int *tokenLength)
	{
		if (source == 0)
			return ttUnknown;

		_source = source;
		_length = length;

		parseToken();

		if (tokenLength != NULL)
			*tokenLength = _tokenLength;

		return _tokenType;
	}

	void Tokenizer::parseToken()
	{
		if (isWhiteSpace())
			return;

		if (isComment())
			return;

		if (isConstant())
			return;

		if (isIdentifier())
			return;

		if (isKeyWord())
			return;

		_tokenType = ttUnknown;
		_tokenLength = 0;
	}

	bool Tokenizer::isWhiteSpace()
	{
		if (_source[0] == '\0')
		{
			_tokenType = ttEndOfFile;
			_tokenLength = 0;

			return(true);
		}

		int n;
		for (n = 0; n < _length; ++n)
		{
			bool isWs = false;
			for (int i = 0; i < WhiteSpaceLength; ++i)
				if (_source[n] == WhiteSpace[i])
				{
					isWs = true;
					break;
				}

			if (!isWs)
				break;
		}

		if (n != 0)
		{
			_tokenType = ttWhitespace;
			_tokenLength = n;

			return(true);
		}

		return(false);
	}

	bool Tokenizer::isComment()
	{
		if (!(_source[0] == '/' && _length >= 2) && _source[0] != '#')
			return false;

		if (_source[1] == '/' || _source[0] == '#')
		{
			int n = _source[1] == '/' ? 2 : 1;

			for (; n < _length; )
			{
				if (_source[n++] == '\n')
					break;
			}

			_tokenType = ttComment;
			_tokenLength = n;

			return true;
		}

		return(false);
	}

	bool Tokenizer::isConstant()
	{
		if (_source[0] >= '0' && _source[0] <= '9')
		{
			if (_source[0] == '0')
			{
				//! Шестнадцатеричное число
				if (_source[1] == 'h')
				{
					int n;
					for (n = 2; n < _length; ++n)
						if (!(_source[n] >= '0' && _source[n] <= '9') &&
						    !(_source[n] >= 'A' && _source[n] <= 'F'))
						   break;

					_tokenType = ttHexConst;
					_tokenLength = n;
					return(true);
				}
				//! Бинарное число
				else if (_source[1] == 'b')
				{
					int n;
					for (n = 2; n < _length; ++n)
						if (_source[n] < '0' || _source[n] > '1')
							break;

					_tokenType = ttBinConst;
					_tokenLength = n;
					return(true);
				}
			}

			int n;
			for (n = 0; n < _length; ++n)
				if (_source[n] < '0' || _source[n] > '9')
					break;
			
			if (_source[n] == '.')
			{
				++n;
				for (; n < _length; ++n)
					if (_source[n] < '0' || _source[n] > '9')
						break;
				
				_tokenType = ttFloatConst;
				_tokenLength = n;
				return(true);
			}

			_tokenType = ttDecConst;
			_tokenLength = n;
			return(true);
		}
		else if (_source[0] == '"' || _source[0] == '\'')
		{
			char cend = _source[0];
			int n;
			for (n = 1; n < _length; ++n)
				if (_source[n] == cend)
				{
					_tokenType = ttString;
					_tokenLength = n+1;

					return(true);
				}

			_tokenLength = n;
			_tokenType = ttNonTerminatedString;
			return(true);
		}

		return(false);
	}

	bool Tokenizer::isIdentifier()
	{
		if (_source[0] == '_' || 
		   (_source[0] >= 'a' && _source[0] <= 'z') ||
		   (_source[0] >= 'A' && _source[0] <= 'Z'))
		{
			int n;
			for (n = 1; n < _length; ++n)
			{
				if ((_source[n] < '0' || _source[n] > '9') &&
				    (_source[n] < 'a' || _source[n] > 'z') &&
				    (_source[n] < 'A' || _source[n] > 'Z'))
				   break;
			}

			_tokenType = ttIdentifier;
			_tokenLength = n;

			if (_tokenLength > 50)
				return(true);

			char temp[51];
			memcpy(temp, _source, _tokenLength);
			temp[_tokenLength] = 0;

			for (n = 0; n < NumberOfTokenWords; ++n)
			{
				if (!strcmp(temp, TokenWords[n].word))
					return(false);
			}

			return(true);
		}

		return(false);
	}

	bool Tokenizer::isKeyWord()
	{
		int words[NumberOfTokenWords];
	
		int n;
		for (n = 0; n < NumberOfTokenWords; ++n)
			words[n] = n;

		int numWords = NumberOfTokenWords;
		int lastPossible = -1;

		n = 0;
		while (n < _length && numWords >= 0)
		{
			for (int i = 0; i < numWords; ++i)
			{
				if (TokenWords[words[i]].word[n] == '\0')
				{
					if (((TokenWords[words[i]].word[n-1] >= 'a' && TokenWords[words[i]].word[n-1] <= 'z') ||
						 (TokenWords[words[i]].word[n-1] >= 'A' && TokenWords[words[i]].word[n-1] <= 'Z')) &&
						((_source[n] >= 'a' && _source[n] <= 'z') ||
						 (_source[n] >= 'A' && _source[n] <= 'Z') ||
						 (_source[n] >= '0' && _source[n] <= '9') ||
						 (_source[n] == '_')) )
					{
						words[i--] = words[--numWords];
					}
					else if (numWords > 1)
					{
						lastPossible = words[i];
						words[i--] = words[--numWords];
						continue;
					}
					else
					{
						_tokenType = TokenWords[words[i]].type;
						_tokenLength = n;

						return(true);
					}
				}
				else if (TokenWords[words[i]].word[n] != _source[n])
					words[i--] = words[--numWords];
			}
			++n;
		}

		if (numWords)
		{
			for (int i = 0; i < numWords; ++i)
			{
				if (TokenWords[words[i]].word[n] == '\0')
				{
					_tokenType = TokenWords[words[i]].type;
					_tokenLength = n;

					return(true);
				}
			}
		}

		if (lastPossible > -1)
		{
			_tokenType = TokenWords[lastPossible].type;
			_tokenLength = strlen(TokenWords[lastPossible].word);

			return(true);
		}

		return(false);
	}

	const char *GetTokenDefinition(TokenType tokenType)
	{
		switch (tokenType)
		{
		case ttComment:
			return "<comment>";

		case ttSub:
			return "<sub>";

		case ttVar:
			return "<var>";

		case ttReturn:
			return "<return>";

		case ttIdentifier:
			return "<identifier>";

		case ttDecConst:
			return "<dec const>";

		case ttHexConst:
			return "<hex const>";

		case ttBinConst:
			return "<bin const>";

		case ttFloatConst:
			return "<float const>";

		case ttString:
			return "<string>";

		case ttNonTerminatedString:
			return "<non terminated string>";

		case ttEndOfFile:
			return "<end of file>";
			
		case ttEndOfStatement:
			return "<end of statement>";

		case ttComma:
			return "<comma (,)>";

		case ttOpenStatementBlock:
			return "<open statement block>";

		case ttCloseStatementBlock:
			return "<close statement block>";

		case ttOpenParanthesis:
			return "<open paranthesis>";

		case ttCloseParanthesis:
			return "<close paranthesis>";

		case ttAssignment:
			return "<assignment (=)>";

		case ttStar:
			return "<star (*)>";

		case ttExponentiation:
			return "<exponentiation (**)>";

		case ttSlash:
			return "<slash (/)>";

		case ttPlus:
			return "<plus (+)>";

		case ttMinus:
			return "<minus (-)>";

		case ttAnd:
			return "<and>";

		case ttOr:
			return "<or>";

		case ttLower:
			return "< < >";

		case ttGreater:
			return "< > >";

		case ttEqual:
			return "<==>";

		case ttNotEqual:
			return "<!=>";

		case ttIncrement:
			return "<++>";

		case ttDecrement:
			return "<-->";

		case ttOpenBracket:
			return "<[]>";

		case ttCloseBracket:
			return "<[]>";

		case ttTrue:
			return "<true>";

		case ttFalse:
			return "<false>";
		}

		return("<???>");
	}
}