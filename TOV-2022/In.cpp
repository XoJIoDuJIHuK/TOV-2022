#include "In.h"
#include "Error.h"
#include <fstream>
#include <string>
#include <iostream>

using namespace std;

namespace In
{
	IN getin(Parm::PARM parm)
	{
		try
		{
			IN in{ 0, 0, 0, nullptr, nullptr, IN_CODE_TABLE };
			int col = 0;

			unsigned char* text = new unsigned char[IN_MAX_LEN_TEXT];
			unsigned char* textOut = new unsigned char[IN_MAX_LEN_TEXT];

			ifstream fin(parm.in);
			if (fin.fail()) throw ERROR_THROW(110);
			bool SingleParenthesesClosed = true, DoubleParenthesesClosed = true;
			bool skipGet = false;
			unsigned char x;
			while (in.size < IN_MAX_LEN_TEXT)
			{
				if (!skipGet)
				{
					x = (unsigned char)fin.get();
				}
				else
				{
					skipGet = false;
				}

				if (fin.eof())
				{
					text[in.size] = '\0';
					in.lines++;
					break;
				}
				if (in.code[x] == in.T || in.code[x] == in.S)
				{
					if (x == '\'') SingleParenthesesClosed = !SingleParenthesesClosed;
					if (x == '\"') DoubleParenthesesClosed = !DoubleParenthesesClosed;
					if (x == ' ' && SingleParenthesesClosed && DoubleParenthesesClosed)
					{
						text[in.size] = x;
						in.size++;
						col++;
						while (x == ' ') x = (unsigned char)fin.get();
						skipGet = true;
						continue;
					}
					if (x == '\n' && SingleParenthesesClosed && DoubleParenthesesClosed)
					{
						text[in.size] = x;
						in.size++;
						col++;
						while (x == '\n') x = (unsigned char)fin.get();
						skipGet = true;
						continue;
					}
					if (x == '\t' && SingleParenthesesClosed && DoubleParenthesesClosed)
					{
						in.ignor++;
						continue;
					}
					text[in.size] = x;
					in.size++;
					col++;
				}
				else if (in.code[x] == in.I)
				{
					in.ignor++;
				}
				else if (in.code[x] == in.F)
				{
					cout << "Запрещённый символ в строке " << in.lines << " в столбце " << col << 
						" код символа: " << (int)x << endl;
					in.noErrors = false;
					throw ERROR_THROW_IN(111, in.lines + 1, col);
				}
				else
				{
					text[in.size] = in.code[x];
					in.size++;
					col++;
				}
				if (x == IN_CODE_ENDL)
				{
					in.lines++;
					col = 0;
				}
			}
			in.text = text;
			fin.close();/*
			fout << text;
			fout.close();*/
			return in;
		}
		catch (Error::ERROR e)
		{
			Error::ErrorDisplay(e);
		}
	}
}