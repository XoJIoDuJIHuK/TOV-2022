#include "LT.h"
#include "IT.h"
#include "In.h"
#include "FST.h"
#include <vector>
#include <stack>

using namespace std;

namespace LT
{

	vector<string> LibraryFunctions = {
		"tonumber",
		"indexof",
		"stringlength"
		"userstringcompare"
	};
	vector<IT::IDDATATYPE> LibFuncsTypes = {
		IT::INT,
		IT::INT,
		IT::INT,
		IT::INT
	};
	vector<vector<IT::IDDATATYPE>> LibFuncsParams = {
		{ IT::STR },
		{ IT::STR, IT::STR },
		{ IT::STR },
		{ IT::STR, IT::STR }
	};
	int Contains(vector<string> v, string s)
	{
		for (int i = 0; i < v.size(); i++)
		{
			if (s == v[i]) return i;
		}
		return -1;
	}

	LexTable Create(int size)
	{
		LexTable lextable;
		lextable.maxsize = size;
		lextable.table = new Entry[size];
		return lextable;
	}
	void Add(LexTable& lextable, Entry entry)
	{
		lextable.table[lextable.size++] = entry;
	}
	
	Entry GetEntry(LexTable& lextable, int n)
	{
		if (n < 0 || n > lextable.size - 1)
		{
			throw ERROR_THROW(10);
			std::cout << "Index n in GetEntry (lextable) is out of range\n";
			Entry e;
			e.idxTI = -1;
			e.lexema = 0;
			e.sn = -1;
			return e;
		}
		return lextable.table[n];
	}
	void Delete(LexTable& lextable)
	{
		delete& lextable;
	}
	void Print(LexTable table, Log::LOG log)
	{
		*log.stream << "Table of lexems | size:" << table.size << " | maxsize: " << table.maxsize << "\n";
		for (int i = 0; i < table.size; i++)
		{
			Entry e = table.table[i];
			*log.stream << i << ") " << e.lexema << " | idxTI: " << e.idxTI << " | string number: " << e.sn;
			*log.stream << " | params: ";
			for (int j = 0; j < e.params.size(); j++)
			{
				switch (e.params[j])
				{
					case IT::INT:
					{
						*log.stream << "int ";
						break;
					}
					case IT::STR:
					{
						*log.stream << "str ";
						break;
					}
					case IT::SYM:
					{
						*log.stream << "sym ";
						break;
					}
				}
			}
			if (e.lexema == LEX_CALLFUNC) *log.stream << " | amount of parameters: " << e.priority;
			*log.stream << '\n';
		}
		*log.stream << "End of lextable\n";
		for (int i = 0; i < table.size; i++)
		{
			*log.stream << table.table[i].lexema;
		}
	}
	void PrintConsole(LexTable table, Log::LOG log)
	{
		std::cout << "Table of lexems | size:" << table.size << " | maxsize: " << table.maxsize << "\n";
		for (int i = 0; i < table.size; i++)
		{
			Entry e = table.table[i];
			std::cout << i << ") " << e.lexema << " | idxTI: " << e.idxTI << " | string number: " << e.sn;
			std::cout << " | params: ";
			for (int j = 0; j < e.params.size(); j++)
			{
				switch (e.params[j])
				{
				case IT::INT:
				{
					std::cout << "int ";
					break;
				}
				case IT::STR:
				{
					std::cout << "str ";
					break;
				}
				case IT::SYM:
				{
					std::cout << "sym ";
					break;
				}
				}
			}
			if (e.lexema == LEX_CALLFUNC) std::cout << " | amount of parameters: " << e.priority;
			std::cout << '\n';
		}
		std::cout << "End of lextable\n";
		for (int i = 0; i < table.size; i++)
		{
			std::cout << table.table[i].lexema;
		}
		std::cout << '\n';
	}
	bool isKeyWord(string str)
	{
		if (str.length() != 32) return false;
		string arr[] = {"printsum", "printstr", "strconcat", "printerror", "stringcopy", "stringcompare"};
		int arrLength = 5;
		for (int i = 0; i < arrLength; i++)
		{
			bool wordFound = true;
			for (int j = 0; j < str.length() && wordFound; j++)
			{
				if (str[j] != arr[i][j]) wordFound = false;
			}
			if (wordFound) return true;
		}
		return false;
	}

	bool isSpecSymbol(char c)
	{
		string str = "(),{};\n";
		for (int i = 0; i < str.length(); i++) if (str[i] == c) return true;
		return false;
	}

	void ToId(char* id, string word)
	{
		for (int j = 0; j < ID_MAXSIZE; j++)
		{
			if (j < word.length()) id[j] = word[j];
			else id[j] = 0;
		}
	}

	void ToCharPtr(char** ptr, string str)
	{
		short length = str.length();
		*ptr = new char[length + 1];
		for (short i = 0; i < length; i++) (*ptr)[i] = str[i];
		(*ptr)[length] = 0;
	}

	void strcpy(char** dest, char* src)
	{
		short length = 0;
		while (src[length] != 0) length++;
		(*dest) = new char[length + 1];
		for (short i = 0; i < length; i++) (*dest)[i] = src[i];
		(*dest)[length] = 0;
	}

	int ToDec(string str, int base)
	{
		int ret = 0;
		for (int i = 0; i < str.length() - 1; i++)
		{
			ret *= base;
			ret += (str[i] - '0');
		}
		return ret;
	}

	void LexAnalyze(In::IN& in, Log::LOG log, LT::LexTable& lextable, IT::IdTable& idtable)
	{
		vector<pair<char, FSTN::FSTsmall>> lexems =
		{
			{
				{
					'v', FSTN::FSTsmall
					{
						2,
						FSTN::NODE(4, FSTN::RELATION('+', 1), FSTN::RELATION('-', 1), FSTN::RELATION('*', 1), FSTN::RELATION('/', 1)),
						FSTN::NODE()
					}
				},
				{
					't', FSTN::FSTsmall
					{
						17,
						FSTN::NODE(3, FSTN::RELATION('n', 1), FSTN::RELATION('s', 6), FSTN::RELATION('s', 11)),
						FSTN::NODE(1, FSTN::RELATION('u', 2)),
						FSTN::NODE(1, FSTN::RELATION('m', 3)),
						FSTN::NODE(1, FSTN::RELATION('b', 4)),
						FSTN::NODE(1, FSTN::RELATION('e', 5)),
						FSTN::NODE(1, FSTN::RELATION('r', 16)),
						FSTN::NODE(1, FSTN::RELATION('y', 7)),
						FSTN::NODE(1, FSTN::RELATION('m', 8)),
						FSTN::NODE(1, FSTN::RELATION('b', 9)),
						FSTN::NODE(1, FSTN::RELATION('o', 10)),
						FSTN::NODE(1, FSTN::RELATION('l', 16)),
						FSTN::NODE(1, FSTN::RELATION('t', 12)),
						FSTN::NODE(1, FSTN::RELATION('r', 13)),
						FSTN::NODE(1, FSTN::RELATION('i', 14)),
						FSTN::NODE(1, FSTN::RELATION('n', 15)),
						FSTN::NODE(1, FSTN::RELATION('g', 16)),
						FSTN::NODE()
					}
				},
				{
					'f', FSTN::FSTsmall
					{
						9,
						FSTN::NODE(1, FSTN::RELATION('f', 1)),
						FSTN::NODE(1, FSTN::RELATION('u', 2)),
						FSTN::NODE(1, FSTN::RELATION('n', 3)),
						FSTN::NODE(1, FSTN::RELATION('c', 4)),
						FSTN::NODE(1, FSTN::RELATION('t', 5)),
						FSTN::NODE(1, FSTN::RELATION('i', 6)),
						FSTN::NODE(1, FSTN::RELATION('o', 7)),
						FSTN::NODE(1, FSTN::RELATION('n', 8)),
						FSTN::NODE()
					}
				},
				{
					'n', FSTN::FSTsmall
					{
						4,
						FSTN::NODE(1, FSTN::RELATION('v',  1)),
						FSTN::NODE(1, FSTN::RELATION('a',  2)),
						FSTN::NODE(1, FSTN::RELATION('r',  3)),
						FSTN::NODE()
					}
				},
				{
					'r', FSTN::FSTsmall
					{
						7,
						FSTN::NODE(1, FSTN::RELATION('r',  1)),
						FSTN::NODE(1, FSTN::RELATION('e',  2)),
						FSTN::NODE(1, FSTN::RELATION('t',  3)),
						FSTN::NODE(1, FSTN::RELATION('u',  4)),
						FSTN::NODE(1, FSTN::RELATION('r',  5)),
						FSTN::NODE(1, FSTN::RELATION('n',  6)),
						FSTN::NODE()
					}
				},
				{
					'c', FSTN::FSTsmall
					{
						3,
						FSTN::NODE(2, FSTN::RELATION('i', 1)),
						FSTN::NODE(1, FSTN::RELATION('f', 2)),
						FSTN::NODE()
					}
				},
				{
					'e', FSTN::FSTsmall
					{
						5,
						FSTN::NODE(2, FSTN::RELATION('e', 1)),
						FSTN::NODE(1, FSTN::RELATION('l', 2)),
						FSTN::NODE(2, FSTN::RELATION('s', 3)),
						FSTN::NODE(1, FSTN::RELATION('e', 4)),
						FSTN::NODE()
					}
				},
				{
					'p', FSTN::FSTsmall
					{
						6,
						FSTN::NODE(1, FSTN::RELATION('p',  1)),
						FSTN::NODE(1, FSTN::RELATION('r',  2)),
						FSTN::NODE(1, FSTN::RELATION('i',  3)),
						FSTN::NODE(1, FSTN::RELATION('n',  4)),
						FSTN::NODE(1, FSTN::RELATION('t',  5)),
						FSTN::NODE()
					}
				},
				{
					'm', FSTN::FSTsmall
					{
						5,
						FSTN::NODE(1, FSTN::RELATION('m',  1)),
						FSTN::NODE(1, FSTN::RELATION('a',  2)),
						FSTN::NODE(1, FSTN::RELATION('i',  3)),
						FSTN::NODE(1, FSTN::RELATION('n',  4)),
						FSTN::NODE()
					}
				},
				{
					'a', FSTN::FSTsmall
					{
						4,
						FSTN::NODE(1, FSTN::RELATION('a',  1)),
						FSTN::NODE(1, FSTN::RELATION('d',  2)),
						FSTN::NODE(1, FSTN::RELATION('d',  3)),
						FSTN::NODE()
					}
				},
				{
					'l', FSTN::FSTsmall
					{
						3,
						FSTN::NODE(1, FSTN::RELATION('\'', 1)),
						FSTN::NODE(40, FSTN::RELATION('\'', 2), FSTN::RELATION('H', 1), FSTN::RELATION('!', 1), FSTN::RELATION(' ', 1), FSTN::RELATION('a', 1), FSTN::RELATION('b', 1), FSTN::RELATION('c', 1), FSTN::RELATION('d', 1), FSTN::RELATION('e', 1), FSTN::RELATION('f', 1), FSTN::RELATION('g', 1), FSTN::RELATION('h', 1), FSTN::RELATION('i', 1), FSTN::RELATION('j', 1), FSTN::RELATION('k', 1), FSTN::RELATION('l', 1), FSTN::RELATION('m', 1), FSTN::RELATION('n', 1), FSTN::RELATION('o', 1), FSTN::RELATION('p', 1), FSTN::RELATION('q', 1), FSTN::RELATION('r', 1), FSTN::RELATION('s', 1), FSTN::RELATION('t', 1), FSTN::RELATION('u', 1), FSTN::RELATION('v', 1), FSTN::RELATION('w', 1), FSTN::RELATION('x', 1), FSTN::RELATION('y', 1), FSTN::RELATION('z', 1), FSTN::RELATION('0', 1), FSTN::RELATION('1', 1), FSTN::RELATION('2', 1), FSTN::RELATION('3', 1), FSTN::RELATION('4', 1), FSTN::RELATION('5', 1), FSTN::RELATION('6', 1), FSTN::RELATION('7', 1), FSTN::RELATION('8', 1), FSTN::RELATION('9', 1)),
						FSTN::NODE()
					}
				},
				{
					'l', FSTN::FSTsmall
					{
						1,
						FSTN::NODE(10, FSTN::RELATION('0', 0), FSTN::RELATION('1', 0), FSTN::RELATION('2', 0), FSTN::RELATION('3', 0), FSTN::RELATION('4', 0), FSTN::RELATION('5', 0), FSTN::RELATION('6', 0), FSTN::RELATION('7', 0), FSTN::RELATION('8', 0), FSTN::RELATION('9', 0)),
					}
				},
				{
					'l', FSTN::FSTsmall//binary
					{
						3,
						FSTN::NODE(2, FSTN::RELATION('0', 1), FSTN::RELATION('1', 1)),
						FSTN::NODE(3, FSTN::RELATION('0', 1), FSTN::RELATION('1', 1), FSTN::RELATION('b', 2)),
						FSTN::NODE()
					}
				},
				{
					'l', FSTN::FSTsmall//8
					{
						3,
						FSTN::NODE(8, FSTN::RELATION('0', 1), FSTN::RELATION('1', 1), FSTN::RELATION('2', 1), FSTN::RELATION('3', 1), FSTN::RELATION('4', 1), FSTN::RELATION('5', 1), FSTN::RELATION('6', 1), FSTN::RELATION('7', 1)),
						FSTN::NODE(9, FSTN::RELATION('0', 1), FSTN::RELATION('1', 1), FSTN::RELATION('2', 1), FSTN::RELATION('3', 1), FSTN::RELATION('4', 1), FSTN::RELATION('5', 1), FSTN::RELATION('6', 1), FSTN::RELATION('7', 1), FSTN::RELATION('e', 2)),
						FSTN::NODE()
					}
				},
				{
					'l', FSTN::FSTsmall
					{
						4,
						FSTN::NODE(1, FSTN::RELATION('"', 1)),
						FSTN::NODE(38, FSTN::RELATION('"', 3), FSTN::RELATION(' ', 2), FSTN::RELATION('a', 2), FSTN::RELATION('b', 2), FSTN::RELATION('c', 2), FSTN::RELATION('d', 2), FSTN::RELATION('e', 2), FSTN::RELATION('f', 2), FSTN::RELATION('g', 2), FSTN::RELATION('h', 2), FSTN::RELATION('i', 2), FSTN::RELATION('j', 2), FSTN::RELATION('k', 2), FSTN::RELATION('l', 2), FSTN::RELATION('m', 2), FSTN::RELATION('n', 2), FSTN::RELATION('o', 2), FSTN::RELATION('p', 2), FSTN::RELATION('q', 2), FSTN::RELATION('r', 2), FSTN::RELATION('s', 2), FSTN::RELATION('t', 2), FSTN::RELATION('u', 2), FSTN::RELATION('v', 2), FSTN::RELATION('w', 2), FSTN::RELATION('x', 2), FSTN::RELATION('y', 2), FSTN::RELATION('z', 2), FSTN::RELATION('0', 2), FSTN::RELATION('1', 2), FSTN::RELATION('2', 2), FSTN::RELATION('3', 2), FSTN::RELATION('4', 2), FSTN::RELATION('5', 2), FSTN::RELATION('6', 2), FSTN::RELATION('7', 2), FSTN::RELATION('8', 2), FSTN::RELATION('9', 2)),
						FSTN::NODE(1, FSTN::RELATION('"', 3)),
						FSTN::NODE()
					}
				},
				{
					'i', FSTN::FSTsmall
					{
						1,
						FSTN::NODE(26, FSTN::RELATION('a', 0), FSTN::RELATION('b', 0), FSTN::RELATION('c', 0), FSTN::RELATION('d', 0), FSTN::RELATION('e', 0), FSTN::RELATION('f', 0), FSTN::RELATION('g', 0), FSTN::RELATION('h', 0), FSTN::RELATION('i', 0), FSTN::RELATION('j', 0), FSTN::RELATION('k', 0), FSTN::RELATION('l', 0), FSTN::RELATION('m', 0), FSTN::RELATION('n', 0), FSTN::RELATION('o', 0), FSTN::RELATION('p', 0), FSTN::RELATION('q', 0), FSTN::RELATION('r', 0), FSTN::RELATION('s', 0), FSTN::RELATION('t', 0), FSTN::RELATION('u', 0), FSTN::RELATION('v', 0), FSTN::RELATION('w', 0), FSTN::RELATION('x', 0), FSTN::RELATION('y', 0), FSTN::RELATION('z', 0))
					}
				}
			}
		};
		int position = 0;
		int positionInLine = 0;
		int positionOut = 3;
		int lexemNumber = 0;
		int identifiersNumber = 0;
		short line = 1;
		int numOfMains = 0;
		bool functionRecentlyDeclared = false;//функци€ только что объ€влена, дл€ заполнени€ вектора параметров
		in.textOut = new unsigned char[IN_MAX_LEN_TEXT];
		std::memset(in.textOut, 0, IN_MAX_LEN_TEXT);
		in.textOut[0] = '0';
		in.textOut[1] = '1';
		in.textOut[2] = ' ';
		string pre0previousWord = "";
		string pre1previousWord = "";

		stack<string> scopeStack;
		bool zeroLiteralAlreadyAdded = false;
		scopeStack.push("");

		IT::IDDATATYPE currentReturnType = IT::INT;

		char* currentScope = new char[1];
		currentScope[0] = 0;
		short numberOfLiterals = 0;
		short parenthesesOpened = 0;
		bool waitingForParameters = false;
		bool includedFunctionScope = false;
		while (position < in.size)
		{
			string word = "";
			char* id = new char[ID_MAXSIZE + 1];
			memset(id, 0, ID_MAXSIZE + 1);
			bool wordFound = false;
			bool stringLiteral = false;
			for (int i = 0; position + i < in.size; i++)
			{
				if (!stringLiteral && (in.code[in.text[position + i]] == In::IN::S ||
					in.text[position + i] == '$')) break;
				if (in.text[position + i] == '\'' || in.text[position + i] == '"') 
					stringLiteral = !stringLiteral;
				wordFound = true;
				word += in.text[position + i];
			}
			if (!wordFound)
			{
				LT::Entry ltEntry;
				if (in.code[in.text[position]] == In::IN::S)
				{
					if (isSpecSymbol(in.text[position]))
					{
						LT::Entry e;
						e.idxTI = -1;
						e.lexema = in.text[position];
						e.sn = line;
						LT::Add(lextable, e);
						lexemNumber++;
					}
					switch (in.text[position])
					{
						case '{':
						{
							parenthesesOpened++;
							break;
						}
						case '}':
						{
							parenthesesOpened--;
							if (parenthesesOpened == 0)
							{
								delete[] currentScope;
								currentScope = new char[1];
								currentScope[0] = 0;
							}
							break;
						}
						case '(':
						{
							ltEntry.priority = 1;
							if (idtable.table[idtable.size - 1].idtype == IT::F) waitingForParameters = true;
							break;
						}
						case ')':
						{
							ltEntry.priority = 1;
							if (waitingForParameters) waitingForParameters = false;
							if (functionRecentlyDeclared) functionRecentlyDeclared = false;
							if (includedFunctionScope)
							{
								includedFunctionScope = false;
								scopeStack.pop();
								ToCharPtr(&currentScope, scopeStack.top());
							}
							break;
						}
					}
					if (in.text[position] == ' ')
					{
						position++;
						positionInLine++;
						continue;
					}
					if (in.text[position] == '+' ||
						in.text[position] == '-' ||
						in.text[position] == '*' ||
						in.text[position] == '/'
						)
					{
						switch (in.text[position])
						{
							case '+':
							{
								ltEntry.signtype = PLUS;
								ltEntry.priority = 3;
								break;
							}
							case '-':
							{
								ltEntry.signtype = MINUS;
								ltEntry.priority = 3;
								break;
							}
							case '*':
							{
								ltEntry.signtype = MULTIPLY;
								ltEntry.priority = 4;
								break;
							}
							case '/':
							{
								ltEntry.signtype = DIVIDE;
								ltEntry.priority = 4;
								break;
							}
						}
						in.textOut[positionOut] = 'v';
						ltEntry.idxTI = -1;
						ltEntry.lexema = 'v';
						ltEntry.sn = line;
						Add(lextable, ltEntry);
						lexemNumber++;
					}
					if (in.text[position] == '=')
					{
						if (in.text[position + 1] == '=')
						{
							position++;
							positionInLine++;
							in.textOut[positionOut] = 'u';
							ltEntry.signtype = EQUALS;
							ltEntry.lexema = 'u';
						}
						else
						{
							ltEntry.lexema = 'w';
						}
						ltEntry.idxTI = -1;
						ltEntry.sn = line;
						Add(lextable, ltEntry);
						lexemNumber++;
					}
					if (in.text[position] == '<' ||
						in.text[position] == '>' ||
						in.text[position] == '!'
						)
					{
						switch (in.text[position])
						{
							case '<':
							{
								if (in.text[position + 1] == '=')
								{
									position++;
									positionInLine++;
									ltEntry.signtype = LESSOREQUALS;
								}
								else ltEntry.signtype = LESS;
								ltEntry.priority = 2;
								break;
							}
							case '>':
							{
								if (in.text[position + 1] == '=')
								{
									position++;
									positionInLine++;
									ltEntry.signtype = MOREOREQUALS;
								}
								else ltEntry.signtype = MORE;
								ltEntry.priority = 2;
								break;
							}
							case '!':
							{
								if (in.text[position + 1] == '=')
								{
									ltEntry.signtype = NOTEQUAL;
									position++;
									positionInLine++;
								}
								else throw ERROR_THROW_IN(13, line, positionInLine);
								ltEntry.priority = 2;
								break;
							}
						}
						in.textOut[positionOut] = 'u';
						ltEntry.idxTI = -1;
						ltEntry.lexema = 'u';
						ltEntry.sn = line;
						Add(lextable, ltEntry);
						lexemNumber++;
					}
					else
					{
						in.textOut[positionOut] = in.text[position];
					}
					if (ltEntry.datatype == LT::ASSIGNMENT) ltEntry.lexema = '=';
					position++;
					positionOut++;
					positionInLine++;
					continue;
				}
				if (in.text[position] == '$')
				{
					line++;
					in.textOut[positionOut++] = '\n';
					in.textOut[positionOut++] = (char)(line / 10 + (int)'0');
					in.textOut[positionOut++] = (char)(line % 10 + (int)'0');
					in.textOut[positionOut++] = ' ';
					position++;
					positionInLine = 0;
					continue;
				}
			}
			else
			{
				ToId(id, word);
				position += word.length();
				positionInLine += word.length();
				bool lexemFound = false;
				for (int i = 0; i < lexems.size(); i++)
				{
					if (FSTN::execute(in, lexems[i].second, word))
					{
						LT::Entry ltEntry;
						IT::Entry itEntry;
						memset(itEntry.id, 0, ID_MAXSIZE + 1);
						if (lexems[i].first == LEX_RETURN)
						{
							ltEntry.datatype = currentReturnType;
						}
						if (lexems[i].first == LEX_MAIN)
						{
							numOfMains++;
							delete[] currentScope;
							currentScope = new char[5];
							currentScope[0] = 'm';
							currentScope[1] = 'a';
							currentScope[2] = 'i';
							currentScope[3] = 'n';
							currentScope[4] = '\0';
							scopeStack.push("main");
							currentReturnType = IT::INT;
						}
						strcpy(&itEntry.scope, currentScope);
						lexemFound = true;
						in.textOut[positionOut++] = lexems[i].first;
						ltEntry.lexema = lexems[i].first;
						ltEntry.sn = line;
						if (lexems[i].first == LEX_ID || lexems[i].first == LEX_LITERAL)
						{
							if (isKeyWord(word)) throw ERROR_THROW_IN(5, line, positionInLine); //#
							itEntry.idxfirstLE = lexemNumber;
							for (int j = 0; j < ID_MAXSIZE; j++) itEntry.id[j] = id[j];
							if (lexems[i].first == LEX_ID)
							{
								int indexOfId = IT::IsId(idtable, id, currentScope);
								if (indexOfId == -1)
								{
									ltEntry.idxTI = idtable.size;
									//переменна€
									if (lextable.table[lextable.size - 2].lexema == LEX_VAR ||
										lextable.table[lextable.size - 1].lexema == LEX_VAR)
									{
										if (Contains(LibraryFunctions, word) != -1)
											throw ERROR_THROW_IN(15, line, positionInLine);
										itEntry.idtype = IT::V;
										if (pre0previousWord == "number")
										{
											itEntry.iddatatype = IT::INT;
											itEntry.value.vint = 0;
										}
										else if (pre0previousWord == "string")
										{
											itEntry.iddatatype = IT::STR;
											itEntry.value.vstr.len = TI_STR_DEFAULT;
										}
										else if (pre0previousWord == "symbol")
										{
											itEntry.iddatatype = IT::SYM;
											itEntry.value.vstr.len = 1;
											itEntry.value.vstr.str[0] = 0;
										}
										else throw ERROR_THROW_IN(6, line, positionInLine);// #6
									}
									//функци€
									else if (lextable.table[lextable.size - 1].lexema == LEX_FUNCTION)
									{
										if (word.length() < 3) throw ERROR_THROW_IN(14, line, positionInLine);
										if (currentScope[0] == 0)
										{
											functionRecentlyDeclared = true;
											ToCharPtr(&currentScope, word);
											scopeStack.push(word);
										}
										if (lextable.table[lextable.size - 3].lexema == LEX_ADD)
										{
											includedFunctionScope = true;
										}
										itEntry.idtype = IT::F;
										if (pre1previousWord == "number")
										{
											itEntry.iddatatype = IT::INT;
											currentReturnType = IT::INT;
											itEntry.value.vint = 0;
										}
										else if (pre1previousWord == "string")
										{
											itEntry.iddatatype = IT::STR;
											currentReturnType = IT::STR;
											itEntry.value.vstr.len = TI_STR_DEFAULT;
											itEntry.value.vstr.str[0] = 0;
										}
										else if (pre1previousWord == "symbol")
										{
											itEntry.iddatatype = IT::SYM;
											currentReturnType = IT::SYM;
											itEntry.value.vstr.len = TI_STR_DEFAULT;
											itEntry.value.vstr.str[0] = 0;
										}
										else throw ERROR_THROW(6, line, positionInLine);// #6
										int indexOfLibFunc = Contains(LibraryFunctions, word);
										if (indexOfLibFunc != -1)
										{
											if (itEntry.iddatatype != LibFuncsTypes[indexOfLibFunc])
												throw ERROR_THROW_IN(166, line, positionInLine);
										}
									}
									//параметр
									else if (waitingForParameters)
									{
										if (Contains(LibraryFunctions, word) != -1)
											throw ERROR_THROW_IN(15, line, positionInLine);
										if (includedFunctionScope) ltEntry.idxTI = -1;
										itEntry.idtype = IT::P;
										if (pre0previousWord == "number")
										{
											itEntry.iddatatype = IT::INT;
											itEntry.value.vint = 0;
										}
										else if (pre0previousWord == "string")
										{
											itEntry.iddatatype = IT::STR;
											itEntry.value.vstr.len = TI_STR_DEFAULT;
											itEntry.value.vstr.str[0] = 0;
										}
										else if (pre0previousWord == "symbol")
										{
											itEntry.iddatatype = IT::SYM;
											itEntry.value.vstr.len = 1;
											itEntry.value.vstr.str[0] = 0;
										}
										else throw ERROR_THROW_IN(8, line, positionInLine);// #8
										if (functionRecentlyDeclared)
										{
											int funcIndex = lextable.size - 2;
											while (funcIndex > -1)
											{
												if (lextable.table[funcIndex].idxTI == -1)
												{
													funcIndex--;
												}
												else
												{
													if (idtable.table[lextable.table[funcIndex].idxTI].
														idtype == IT::F) break;
													funcIndex--;
												}
											}
											if (funcIndex == -1) cout << "funcIndex == -1\n";
											string func = "";
											func += idtable.table[lextable.table[funcIndex].idxTI].id;
											int indexOfLibFunc = Contains(LibraryFunctions, func);
											if (indexOfLibFunc != -1)
											{
												if (itEntry.iddatatype != LibFuncsParams[indexOfLibFunc][
													lextable.table[idtable.table[lextable.
														table[funcIndex].idxTI].idxfirstLE].params.size()
												]) throw ERROR_THROW_IN(165, line, positionInLine);
											}
											switch (itEntry.iddatatype)
											{
												case IT::INT:
												{
													lextable.table[idtable.table[lextable.
														table[funcIndex].idxTI].idxfirstLE].params.
														push_back(IT::INT);
													break;
												}
												case IT::STR:
												{
													lextable.table[idtable.table[lextable.
														table[funcIndex].idxTI].idxfirstLE].params.
														push_back(IT::STR);
													break;
												}
												case IT::SYM:
												{
													lextable.table[idtable.table[lextable.
														table[funcIndex].idxTI].idxfirstLE].params.
														push_back(IT::SYM);
													break;
												}
											}
										}
									}
									else
									{
										throw ERROR_THROW_IN(7, line, positionInLine);// #7
									}

									if (!(waitingForParameters && includedFunctionScope))
									{
										IT::Add(idtable, itEntry);
										identifiersNumber++;
									}
								}
								else
								{
									if (waitingForParameters)
									{

									}
									ltEntry.idxTI = indexOfId;
								}
							}
							else
							{
								int alreadyExists = -1;
								bool isString = word[0] == '\'';
								bool isSymbol = word[0] == '"';
								IT::IDDATATYPE type = IT::INT;
								if (isString) type = IT::STR;
								if (isSymbol) type = IT::SYM;
								for (int j = 0; j < idtable.size; j++)
								{
									IT::Entry entry = idtable.table[j];
									//if (entry.idtype == IT::L && entry.iddatatype == type)
									if (lextable.table[entry.idxfirstLE].lexema == LEX_LITERAL && 
										entry.iddatatype == type)
									{
										if (isString)
										{
											string strCmp = word.substr(1, word.length() - 2);
											bool stringsAreEqual = strCmp.length() == entry.value.vstr.len;
											for (int k = 0; k < strCmp.length() && stringsAreEqual; k++)
											{
												if (strCmp[k] != entry.value.vstr.str[k]) stringsAreEqual = false;
											}
											if (stringsAreEqual)
											{
												alreadyExists = j;
												break;
											}
										}
										else if (isSymbol)
										{
											if (entry.value.vstr.len == 0 && word.length() == 2) alreadyExists = j;
											else if (entry.value.vstr.len == 1 && word.length() == 3)
											{
												if (entry.value.vstr.str[0] == word[1]) alreadyExists = j;
												break;
											}
										}
										else
										{
											if (atoi(word.c_str()) == entry.value.vint)
											{
												alreadyExists = j;
												break;
											}
										}
									}
								}
								if (numberOfLiterals > 999) throw ERROR_THROW_IN(12, line, positionInLine);
								memset(itEntry.id, 0, ID_MAXSIZE + 1);
								itEntry.id[0] = 'L';
								itEntry.id[1] = 'i';
								if (isString) itEntry.id[1] = 's';
								else if (isSymbol) itEntry.id[1] = 'y';
								itEntry.id[2] = (char)(((numberOfLiterals / 100) % 10) + '0');
								itEntry.id[3] = (char)(((numberOfLiterals / 10) % 10) + '0');
								itEntry.id[4] = (char)(numberOfLiterals % 10 + '0');
								itEntry.idtype = IT::L;
								if (isString)
								{
									itEntry.iddatatype = IT::STR;
									int length = word.length() - 2;
									if (length > 255) throw ERROR_THROW_IN(16, line, positionInLine);
									for (int j = 0; j < length; j++) itEntry.value.vstr.str[j] = word[j + 1];
									itEntry.value.vstr.len = length;
								}
								else if (isSymbol)
								{
									itEntry.iddatatype = IT::SYM;
									int length = word.length() - 2;
									itEntry.value.vstr.len = length;
									if (length > 0)
									{
										itEntry.value.vstr.str[0] = word[1];
									}
								}
								else
								{
									itEntry.iddatatype = IT::INT;
									int length = word.length();
									char* a = new char[length];
									for (int j = 0; j < length; j++)
									{
										a[j] = word[j];
									}
									switch (i)
									{
										case 11:
										{
											itEntry.value.vint = atoi(a);
											break;
										}
										case 12:
										{
											itEntry.value.vint = ToDec(word, 2);
											break;
										}
										case 13:
										{
											itEntry.value.vint = ToDec(word, 8);
											break;
										}
									}
								}
								if (alreadyExists == -1 || (itEntry.iddatatype == IT::INT && 
									!zeroLiteralAlreadyAdded && itEntry.value.vint == 0))
								{
									if (!zeroLiteralAlreadyAdded) zeroLiteralAlreadyAdded = true;
									ltEntry.idxTI = identifiersNumber;
									IT::Add(idtable, itEntry);
									identifiersNumber++;
									numberOfLiterals++;
								}
								else
								{
									ltEntry.idxTI = alreadyExists;
								}
							}
						}
						else
						{
							ltEntry.idxTI = -1;
						}
						LT::Add(lextable, ltEntry);
						pre1previousWord = pre0previousWord;
						pre0previousWord = word;
						lexemNumber++;
						break;
					}
				}
				if (!lexemFound)
				{
					cout << "Unexpected lexem at position " << position << " word: " << word << endl;
					throw ERROR_THROW_IN(11, line, positionInLine);
				}
			}
			delete[] id;
		}
		if (numOfMains == 0) throw ERROR_THROW(150);
		if (numOfMains > 1) throw ERROR_THROW(151);
		LT::Entry le;
		le.idxTI = -1;
		le.lexema = '$';
		le.sn = -1;
		LT::Add(lextable, le);
		IT::Print(idtable, log);
		LT::Print(lextable, log);
	}
}