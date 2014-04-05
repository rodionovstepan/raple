//-------------------------------------
#include "../Headers/StaticCgiLibrary.h"
#include "../Headers/IVirtualMachine.h"
#include "../Headers/HashArray.h"
//-------------------------------------
#include <iostream>
#include <vector>
//-------------------------------------


namespace RapleLibraries
{
	StaticCgiLibrary::StaticCgiLibrary()
	{
		registerSub("host", 0, StaticCgiLibrary::cgi_host);
		registerSub("html", 0, StaticCgiLibrary::cgi_html);
		registerSub("beginform", 2, StaticCgiLibrary::cgi_beginForm);
		registerSub("endform", 0, StaticCgiLibrary::cgi_endForm);
		registerSub("submit", 1, StaticCgiLibrary::cgi_submit);
		registerSub("input", 2, StaticCgiLibrary::cgi_input);
		registerSub("hidden", 2, StaticCgiLibrary::cgi_hidden);
		registerSub("get", 0, StaticCgiLibrary::cgi_get);
	}

	StaticCgiLibrary::~StaticCgiLibrary()
	{
	}

	int StaticCgiLibrary::cgi_host(IVirtualMachine *vm)
	{
		char *host;
		size_t length;

		if (_dupenv_s(&host, &length, "HTTP_HOST"))
		{
			vm->PushNull();
			return 1;
		}

		vm->PushString(host);
		delete [length+1] host;

		return 0;
	}

	int StaticCgiLibrary::cgi_html(IVirtualMachine *vm)
	{
		std::cout << "Content-type: text/html\n\n";
		return 0;
	}

	int StaticCgiLibrary::cgi_beginForm(IVirtualMachine *vm)
	{
		Var *method = vm->Pop();
		Var *action = vm->Pop();
		
		RapleAssert(action->GetDataType() == dtString && method->GetDataType() == dtString);

		std::cout << "<form action=\"" << action->String()->GetBuffer() << "\" method=\"" << method->String()->GetBuffer() << "\">\n";
		return 0;
	}

	int StaticCgiLibrary::cgi_endForm(IVirtualMachine *vm)
	{
		std::cout << "</form>";
		return 0;
	}

	int StaticCgiLibrary::cgi_submit(IVirtualMachine *vm)
	{
		Var *value = vm->Pop();
		RapleAssert(value->GetDataType() == dtString);

		std::cout << "\t<input type=\"submit\" value=\"" << value->String()->GetBuffer() << "\" />\n";
		return 0;
	}

	int StaticCgiLibrary::cgi_input(IVirtualMachine *vm)
	{
		Var *name = vm->Pop();
		Var *id = vm->Pop();

		RapleAssert(id->GetDataType() == dtString && name->GetDataType() == dtString);

		std::cout << "\t<input type=\"text\" id=\"" << id->String()->GetBuffer() << "\" name=\"" << name->String()->GetBuffer() << "\" />\n";
		return 0;
	}

	int StaticCgiLibrary::cgi_hidden(IVirtualMachine *vm)
	{
		Var *value = vm->Pop();
		Var *name = vm->Pop();

		RapleAssert(value->GetDataType() == dtString && name->GetDataType() == dtString);

		std::cout << "\t<input type=\"hidden\" name=\"" << name->String()->GetBuffer() << "\" value=\"" << value->String()->GetBuffer() << "\" />\n";
		return 0;
	}

	int StaticCgiLibrary::cgi_href(IVirtualMachine *vm)
	{
		Var *href = vm->Pop();
		Var *title = vm->Pop();

		RapleAssert(href->GetDataType() == dtString && title->GetDataType() == dtString);

		std::cout << "<a href=\"" << href->String()->GetBuffer() << "\">" << title->String()->GetBuffer() << "</a>";
		return 0;
	}

	std::vector<rstring> split(const rstring &s, char delim)
	{
		std::vector<rstring> v;

		int begin = 0;
		int end;
		bool shouldBreak = false;

		for (;;)
		{
			end = s.IndexOf(begin, delim);
			if (end == -1)
			{
				end = s.Length();
				shouldBreak = true;
			}

			v.push_back(s.Substring(begin, end-begin));
			if (shouldBreak)
				break;

			begin = end+1;
		}

		return v;
	}

	HashArray *processQueryString(const rstring &qs)
	{
		HashArray *a = new HashArray();
		if (qs.Length() == 0)
			return a;

		std::vector<rstring> params = split(qs, '&');

		for (auto i = params.begin(); i != params.end(); ++i)
		{
			std::vector<rstring> v1 = split(*i, '=');
			
			if (v1.size() == 1)
				a->Set(Var::CreateString(v1[0]), Var::CreateNull());
			else if (v1.size() == 2)
				a->Set(Var::CreateString(v1[0]), Var::CreateString(v1[1]));
		}

		return a;
	}

	int StaticCgiLibrary::cgi_get(IVirtualMachine *vm)
	{
		char *querystring;
		size_t length;

		if (_dupenv_s(&querystring, &length, "QUERY_STRING") || querystring == 0)
			vm->PushArrayPointer(new HashArray());
		else
			vm->PushArrayPointer(processQueryString(querystring));

		delete [length] querystring;

		return 0;
	}
}
