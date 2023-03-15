////
//// Created by vane4ka on 02.06.2022.
////
#include "LN.h"
#include "return_codes.h"

#include <fstream>
#include <iostream>
#include <stack>
#include <stdexcept>
#include <vector>

int main(int argc, char* argv[])
{
	std::vector< std::string_view > args(argv + 1, argv + argc);

	if (args.size() != 2)
	{
		std::cerr << "Wrong number of args\n";
		return ERROR_INVALID_PARAMETER;
	}

	std::ifstream fin(args[0].data());
	if (!fin.is_open())
	{
		std::cerr << "Input file doesn't exist: " << args[0] << "\n";
		return ERROR_FILE_NOT_FOUND;
	}

	std::stack< LN > st;
	while (true)
	{
		if (fin.eof())
		{
			break;
		}
		std::string x;
		fin >> x;
		if (x.empty())
		{
			break;
		}
		if (x == "+")
		{
			LN a = st.top();
			st.pop();
			LN b = st.top();
			st.pop();
			LN c = b + a;
			st.push(a + b);
			continue;
		}
		if (x == "-")
		{
			LN a = st.top();
			st.pop();
			LN b = st.top();
			st.pop();
			st.push(b - a);
			continue;
		}
		if (x == "*")
		{
			LN a = st.top();
			st.pop();
			LN b = st.top();
			st.pop();
			st.push(a * b);
			continue;
		}
		if (x == "/")
		{
			LN a = st.top();
			st.pop();
			LN b = st.top();
			st.pop();
			st.push(b / a);
			continue;
		}
		if (x == "~")
		{
			LN a = st.top();
			st.pop();
			st.push(~a);
			continue;
		}
		if (x == "_")
		{
			LN a = st.top();
			st.pop();
			st.push(-a);
			continue;
		}
		if (x == "%")
		{
			LN a = st.top();
			st.pop();
			LN b = st.top();
			st.pop();
			st.push(b % a);
			continue;
		}
		if (x == "<")
		{
			LN a = st.top();
			st.pop();
			LN b = st.top();
			st.pop();
			st.push(b < a);
			continue;
		}
		if (x == "<=")
		{
			LN a = st.top();
			st.pop();
			LN b = st.top();
			st.pop();
			st.push(b <= a);
			continue;
		}
		if (x == ">")
		{
			LN a = st.top();
			st.pop();
			LN b = st.top();
			st.pop();
			st.push(b > a);
			continue;
		}
		if (x == ">=")
		{
			LN a = st.top();
			st.pop();
			LN b = st.top();
			st.pop();
			st.push(b >= a);
			continue;
		}
		if (x == "==")
		{
			LN a = st.top();
			st.pop();
			LN b = st.top();
			st.pop();
			st.push(a == b);
			continue;
		}
		if (x == "!=")
		{
			LN a = st.top();
			st.pop();
			LN b = st.top();
			st.pop();
			st.push(a != b);
			continue;
		}
		try
		{
			LN a((std::string_view(x)));
			st.push(a);
		} catch (std::invalid_argument& e)
		{
			std::cerr << "Incorrect input - probably, there are non-numeral symbols in number";
		} catch (...)
		{
			std::cerr << "Errors occurs - bad alloked";
		}
	}
	fin.close();
	std::ofstream fout(args[1].data());
	if (!fout.is_open() || !fout.good())
	{
		std::cerr << "Can't write to output file: " << args[1] << "\n";
		return ERROR_UNKNOWN;
	}
	while (!st.empty())
	{
		fout << (std::string)st.top() << "\n";
		st.pop();
	}
	fout.close();
	return ERROR_SUCCESS;
}