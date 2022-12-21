#include "CodeGen.h"
#include <stack>
#include <fstream>

#define GET_ASM_ID(asmText, i) asmText += idtable.table[lextable.table[i].idxTI].scope;\
asmText += "@";\
asmText += idtable.table[lextable.table[i].idxTI].id;
#define GET_ASM_LITERAL(asmText, i) asmText += idtable.table[lextable.table[i].idxTI].id;

namespace CodeGen
{
	std::string GetFunctionASMCode(LT::LexTable lextable, IT::IdTable idtable, int start, int end);

	bool Contains(std::vector<int> v, int item)
	{
		for (int i = 0; i < v.size(); i++) if (v[i] == item) return true;
		return false;
	}

	std::string GetExpressionASMCode(LT::LexTable lextable, IT::IdTable idtable, int start, int end, 
		IT::IDDATATYPE type)
	{
		std::string asmText = ";-------------";
		for (int i = start; i < end && lextable.table[i].lexema != LEX_PLACEHOLDER && 
			lextable.table[i].lexema != LEX_SEMICOLON && lextable.table[i].lexema != LEX_LEFTBRACE; i++) 
			asmText += lextable.table[i].lexema;
		asmText += "-------------\n";

		int numberOfDivByZeroChecks = 0;

		for (int i = start; i < end; i++)
		{
			switch (lextable.table[i].lexema)
			{
				case LEX_LITERAL:
				case LEX_ID:
				{
					IT::IDDATATYPE varType = idtable.table[lextable.table[i].idxTI].iddatatype;
					asmText += "\tPUSH\t";
					if (varType == IT::STR || varType == IT::SYM) asmText += "OFFSET\t";
					if (lextable.table[i].lexema == LEX_ID)
					{
						asmText += idtable.table[lextable.table[i].idxTI].scope;
						asmText += "@";
						asmText += idtable.table[lextable.table[i].idxTI].id;
					}
					else
					{
						asmText += idtable.table[lextable.table[i].idxTI].id;
					}
					asmText += "\n";
					break;
				}
				case LEX_CALLFUNC:
				{
					asmText += "\tCALL\t";
					asmText += idtable.table[lextable.table[++i].idxTI].id;
					asmText += "\n\tPUSH\tEAX\n";
					break;
				}
				case LEX_ARITHMETICALSIGN:
				{
					switch (lextable.table[i].signtype)
					{
						case LT::PLUS:
						{
							switch (type)
							{
								case IT::INT:
								{
									asmText += "\tPOP \tEDX\n\tPOP \tEAX\n";
									asmText += "\tADD \tEAX, EDX\n";
									break;
								}
								case IT::STR:
								{
									asmText += "\tPOP \tECX\n\tPOP \tEBX\n\tINVOKE\tstrconcat, EBX, ECX\n";
									break;
								}
								case IT::SYM:
								{
									asmText += "\tPOP \tECX\n\tPOP \tEBX\n\tINVOKE\tsumsymbol, EBX, ECX\n";
									break;
								}
							}
							asmText += "\tPUSH\tEAX\n";
							break;
						}
						case LT::MINUS:
						{
							asmText += "\tPOP \tEDX\n\tPOP \tEBX\n";
							asmText += "\tSUB \tEBX, EDX\n";
							asmText += "\tPUSH\tEBX\n";
							break;
						}
						case LT::MULTIPLY:
						{
							asmText += "\tPOP \tEDX\n\tPOP \tEBX\n";
							asmText += "\tIMUL\tEBX, EDX\n";
							asmText += "\tPUSH\tEBX\n";
							break;
						}
						case LT::DIVIDE:
						{
							asmText += "\tPOP \tEBX\n\tPOP \tEAX\n\tCMP \tEBX, 0\n\tJNE \tdivByZeroCheckPassed_";
							asmText += std::to_string(start);
							asmText += "_";
							asmText += std::to_string(numberOfDivByZeroChecks);
							asmText += "\n\tPUSH\t0\n\tCALL\tprinterror\n";
							asmText += "\tPUSH\t-1\n\tCALL\tExitProcess\n\tdivByZeroCheckPassed_";
							asmText += std::to_string(start);
							asmText += "_";
							asmText += std::to_string(numberOfDivByZeroChecks);
							numberOfDivByZeroChecks++;
							asmText += ":\n\tIDIV\tEBX\n\tPUSH\tEAX\n";
							break;
						}
					}
					break;
				}
				case LEX_COMPARISONSIGN:
				{
					switch (type)
					{
						case IT::INT:
						{
							asmText += "\tPOP \tEBX\n\tPOP \tEAX\n\t.IF \t(EAX ";
							switch (lextable.table[i].signtype)
							{
								case LT::EQUALS:
								{
									asmText += "== ";
									break;
								}
								case LT::NOTEQUAL:
								{
									asmText += "!= ";
									break;
								}
								case LT::MORE:
								{
									asmText += "> ";
									break;
								}
								case LT::LESS:
								{
									asmText += "< ";
									break;
								}
								case LT::MOREOREQUALS:
								{
									asmText += ">= ";
									break;
								}
								case LT::LESSOREQUALS:
								{
									asmText += "<= ";
									break;
								}
							}
							asmText += "EBX)\n";
							break;
						}
						case IT::STR:
						case IT::SYM:
						{
							asmText += "\tPOP \tEBX\n\tPOP \tEAX\n\tPUSH\tEBX\n\tPUSH\tEAX\n\
								\tCALL\tstringcompare\n\tMOV \tEBX, 0\n\t.IF \t(EAX ";
							switch (lextable.table[i].signtype)
							{
							case LT::EQUALS:
							{
								asmText += "== ";
								break;
							}
							case LT::NOTEQUAL:
							{
								asmText += "!= ";
								break;
							}
							case LT::MORE:
							{
								asmText += "> ";
								break;
							}
							case LT::LESS:
							{
								asmText += "< ";
								break;
							}
							case LT::MOREOREQUALS:
							{
								asmText += ">= ";
								break;
							}
							case LT::LESSOREQUALS:
							{
								asmText += "<= ";
								break;
							}
							}
							asmText += "EBX)\n";
							break;
						}
					}
					return asmText;
				}
			}
		}

		return asmText;
	}

	std::string GetConditionASMCode(LT::LexTable lextable, IT::IdTable idtable, int start, int end, 
		int markIndex)
	{
		std::string asmText = "";

		bool isElse = false;
		for (int i = start; i < end && !isElse; i++) if (lextable.table[i].lexema == LEX_ELSE) isElse = true;
		int lastIdOrLiteralIndex = start;
		while (lextable.table[lastIdOrLiteralIndex].lexema != LEX_ARITHMETICALSIGN &&
			lextable.table[lastIdOrLiteralIndex].lexema != LEX_COMPARISONSIGN)
		{
			lastIdOrLiteralIndex++;
		}
		int startOfCondBlock = start;
		int endOfCondBlock = start;
		while (lextable.table[startOfCondBlock].lexema != LEX_LEFTBRACE) startOfCondBlock++;
		while (lextable.table[endOfCondBlock].lexema != LEX_BRACELET) endOfCondBlock++;
		IT::IDDATATYPE type = idtable.table[lextable.table[lastIdOrLiteralIndex - 1].idxTI].iddatatype;
		asmText += GetExpressionASMCode(lextable, idtable, start, startOfCondBlock - 1, type);
		asmText += GetFunctionASMCode(lextable, idtable, startOfCondBlock + 1, endOfCondBlock);

		if (isElse)
		{
			asmText += "\t.ELSE\n";
			while (lextable.table[endOfCondBlock + 1].lexema == LEX_ELSE)
			{
				if (lextable.table[endOfCondBlock + 2].lexema == LEX_IF)
				{
					startOfCondBlock = endOfCondBlock + 4;
					endOfCondBlock = startOfCondBlock;
					while (lextable.table[endOfCondBlock].lexema != LEX_BRACELET) endOfCondBlock++;
					asmText += GetConditionASMCode(lextable, idtable, startOfCondBlock, endOfCondBlock, start);
				}
				else
				{
					startOfCondBlock = endOfCondBlock + 3;
					endOfCondBlock = startOfCondBlock;
					while (lextable.table[endOfCondBlock].lexema != LEX_BRACELET) endOfCondBlock++;
					asmText += GetFunctionASMCode(lextable, idtable, startOfCondBlock, endOfCondBlock);
				}
			}
		}
		if (markIndex == -1)
		{
			asmText += "endIfMarker@";
			asmText += std::to_string(start);
			asmText += ":";
		}
		else
		{
			asmText += "\tJMP \tendIfMarker@";
			asmText += std::to_string(markIndex);
		}
		asmText += "\n\t.ENDIF\n";

		return asmText;
	}

	std::string GetFunctionASMCode(LT::LexTable lextable, IT::IdTable idtable, int start, int end)
	{
		std::string asmText = "";

		for (int i = start; i < end; i++)
		{
			switch (lextable.table[i].lexema)
			{
				case LEX_EQUALS:
				{
					int startOfExpression = i + 1;
					int endOfExpression = i + 2;
					while (lextable.table[endOfExpression].lexema != LEX_SEMICOLON) endOfExpression++;
					IT::IDDATATYPE type = idtable.table[lextable.table[i - 1].idxTI].iddatatype;
					asmText += GetExpressionASMCode(lextable, idtable, startOfExpression, endOfExpression, type);
					switch (type)
					{
						case IT::INT:
						{
							asmText += "\tPOP \tEAX\n\tMOV \t";
							GET_ASM_ID(asmText, i - 1);
							asmText += ", EAX\n";
							break;
						}
						case IT::STR:
						case IT::SYM:
						{
							asmText += "\tPUSH\tOFFSET\t";
							GET_ASM_ID(asmText, i - 1);
							asmText += "\n\tCALL\tstringcopy\n";
							break;
						}
					}
					break;
				}
				case LEX_RETURN:
				{
					int startOfExpression = i + 1;
					int endOfExpression = i + 2;
					while (lextable.table[endOfExpression].lexema != LEX_SEMICOLON) endOfExpression++;
					IT::IDDATATYPE type = lextable.table[i].datatype;
					asmText += GetExpressionASMCode(lextable, idtable, startOfExpression, endOfExpression, type);
					asmText += "\tPOP \tEAX\n\tRET\n";
					break;
				}
				case LEX_PRINT:
				{
					int startOfExpression = i + 1;
					int endOfExpression = i + 2;
					while (lextable.table[endOfExpression].lexema != LEX_SEMICOLON) endOfExpression++;
					IT::IDDATATYPE type;
					for (int j = endOfExpression - 1; j >= startOfExpression; j--)
					{
						if (lextable.table[j].lexema == LEX_ID || lextable.table[j].lexema == LEX_LITERAL)
						{
							type = idtable.table[lextable.table[j].idxTI].iddatatype;
							break;
						}
					}
					asmText += GetExpressionASMCode(lextable, idtable, startOfExpression, endOfExpression, type);
					switch (type)
					{
						case IT::INT:
						{
							asmText += "\tCALL\tprintnum\n";
							break;
						}
						case IT::STR:
						case IT::SYM:
						{
							asmText += "\tCALL\tprintstr\n";
							break;
						}
					}
					break;
				}
				case LEX_IF:
				{
					int j = i;
					int endOfCondBlock = i;
					bool condBlockFound = true;
					while (condBlockFound)
					{
						while (lextable.table[endOfCondBlock].lexema != LEX_BRACELET)
						{
							endOfCondBlock++;
						}
						if (lextable.table[endOfCondBlock + 1].lexema != LEX_ELSE) condBlockFound = false;
						else endOfCondBlock++;
					}
					while (lextable.table[j].lexema != LEX_BRACELET) j++;
					asmText += GetConditionASMCode(lextable, idtable, i + 2, endOfCondBlock, -1);
					i = endOfCondBlock;
					break;
				}
			}
		}

		return asmText;
	}

	std::string GetFunctionPrototypeASMCode(LT::LexTable lextable, IT::IdTable idtable, int start)
	{
		std::string asmText = "";
		asmText += idtable.table[lextable.table[start].idxTI].id;
		asmText += "\t";
		asmText += "PROTO";
		if (lextable.table[idtable.table[lextable.table[start].idxTI].idxfirstLE].params.size() > 0)
		{
			switch (lextable.table[idtable.table[lextable.table[start].idxTI].idxfirstLE].params[0])
			{
				case IT::INT:
				{
					asmText += " : SDWORD";
					break;
				}
				case IT::STR:
				{
					asmText += " : DWORD";
					break;
				}
				case IT::SYM:
				{
					asmText += " : DWORD";
					break;
				}
			}
			for (int j = 1; j < lextable.table[idtable.table[lextable.table[start].idxTI].idxfirstLE].
				params.size(); j++)
			{
				asmText += ", : ";
				switch (lextable.table[idtable.table[lextable.table[start].idxTI].idxfirstLE].params[j])
				{
					case IT::INT:
					{
						asmText += "SDWORD";
						break;
					}
					case IT::STR:
					{
						asmText += "DWORD";
						break;
					}
					case IT::SYM:
					{
						asmText += "DWORD";
						break;
					}
				}
			}
		}
		asmText += '\n';

		return asmText;
	}

	void StartCodeGeneration(LT::LexTable lextable, IT::IdTable idtable)
	{
		std::ofstream outStream("..\\Assembler\\Out.asm", std::ios_base::trunc);
		//std::ofstream outStream("out.asm", std::ios_base::trunc);
		if (!(outStream.is_open())) throw ERROR_THROW(113);

		std::string asmText = ".586\n\n.MODEL FLAT, STDCALL\n\nINCLUDELIB		 libucrt.lib\n\
INCLUDELIB		 kernel32.lib\nINCLUDELIB		 \"..\\Debug\\STDLIB.lib\"\n\
ExitProcess		 PROTO : DWORD\n\n.STACK 4096\n\nprinterror		PROTO : DWORD\nprintnum		PROTO : SDWORD\n\
stringcopy		 PROTO : DWORD, : DWORD\nprintstr	PROTO : DWORD\nstrconcat\tPROTO : DWORD, : DWORD\n\
stringcompare	 PROTO : DWORD, : DWORD\nsumsymbol	PROTO : DWORD, : DWORD\n";
		
		for (int i = 0; i < lextable.size; i++)//прототипы функций
		{
			if (lextable.table[i].lexema == LEX_FUNCTION)
			{
				asmText += GetFunctionPrototypeASMCode(lextable, idtable, i + 1);
			}
		}
		asmText += "\n.CONST\n";


		for (int i = 0; i < idtable.size; i++)//заполнение констант
		{
			if (lextable.table[idtable.table[i].idxfirstLE].lexema == LEX_LITERAL)
			{
				asmText += '\t';
				asmText += idtable.table[i].id;
				switch (idtable.table[i].iddatatype)
				{
					case IT::INT:
					{
						asmText += "\tSDWORD\t" + std::to_string(idtable.table[i].value.vint) + '\n';
						break;
					}
					case IT::STR:
					case IT::SYM:
					{
						int length = idtable.table[i].value.vstr.len;
						asmText += "\tBYTE\t";
						asmText += std::to_string(length);
						if (length > 0)
						{

							asmText += ", \"";
							for (int j = 0; j < length; j++) asmText += idtable.table[i].value.vstr.str[j];
							asmText += "\"\n";
						}
						else asmText += "\n";
						break;
					}
				}
			}
		}
		asmText += "\n.DATA\n";


		for (int i = 0; i < idtable.size; i++)//заполнение переменных
		{
			if (lextable.table[idtable.table[i].idxfirstLE].lexema == LEX_ID &&
				idtable.table[i].idtype != IT::F)
			{
				asmText += '\t';

				asmText += idtable.table[i].scope;
				asmText += "@";
				asmText += idtable.table[i].id;

				asmText += '\t';
				switch (idtable.table[i].iddatatype)
				{
					case IT::INT:
					{
						asmText += "SDWORD\t";
						asmText += std::to_string(idtable.table[i].value.vint);
						asmText += '\n';
						break;
					}
					case IT::STR:
					{
						asmText += "BYTE\t256 DUP(0)\n";
						break;
					}
					case IT::SYM:
					{
						asmText += "BYTE\t0, 0\n";
						break;
					}
				}
			}
		}
		asmText += "\n.CODE\n";

		for (int i = 0; i < lextable.size; i++)//main
		{
			if (lextable.table[i].lexema == LEX_MAIN)
			{
				asmText += "main PROC\n";
				int endOfMain = i + 2;
				int bracesOpened = 0;
				while (true)
				{
					if (lextable.table[endOfMain].lexema == LEX_BRACELET && bracesOpened == 0) break;
					switch (lextable.table[endOfMain].lexema)
					{
						case LEX_LEFTBRACE:
						{
							bracesOpened++;
							break;
						}
						case LEX_BRACELET:
						{
							bracesOpened--;
							break;
						}
					}
					endOfMain++;
				}
				asmText += GetFunctionASMCode(lextable, idtable, i + 2, endOfMain);
				asmText += "\tPUSH\t0\n\tCALL\tExitProcess\nmain ENDP";
				break;
			}
		}

		for (int i = 0; i < lextable.size; i++)//добавляем процедуры
		{
			if (lextable.table[i].idxTI != -1 && idtable.table[lextable.table[i].idxTI].idxfirstLE == i && 
				idtable.table[lextable.table[i].idxTI].idtype == IT::F && lextable.table[i - 3].lexema !=
				LEX_ADD)
			{
				asmText += "\n\n;----------FUNCTION ";
				asmText += idtable.table[lextable.table[i].idxTI].id;
				asmText += "---------\n\n";
				asmText += idtable.table[lextable.table[i].idxTI].id;
				asmText += " PROC uses EBX ECX EDX";
				std::string localVarsInit = "";
				int j = i + 2;
				int numOfInitLoops = 0;
				std::string currFuncName = idtable.table[lextable.table[i].idxTI].id;
				while (lextable.table[j].lexema != LEX_RIGHTHESIS)
				{
					switch (lextable.table[j].lexema)
					{
						case LEX_COMMA:
						case LEX_TYPE:
						{
							break;
						}
						case LEX_ID:
						{
							asmText += ", ";
							GET_ASM_ID(asmText, j);
							switch (idtable.table[lextable.table[j].idxTI].iddatatype)
							{
								case IT::INT:
								{
									asmText += "_PARAM : SDWORD";
									localVarsInit += "\tMOV \tEAX, ";
									GET_ASM_ID(localVarsInit, j);
									localVarsInit += "_PARAM\n\tMOV \t";
									GET_ASM_ID(localVarsInit, j);
									localVarsInit += ", EAX\n";
									break;
								}
								case IT::STR:
								case IT::SYM:
								{//потому что в функцию передаём 
									asmText += "_OFFSET : DWORD";
									localVarsInit += "\tPUSH\t";
									GET_ASM_ID(localVarsInit, j);
									localVarsInit += "_OFFSET\n\tPUSH\tOFFSET ";
									GET_ASM_ID(localVarsInit, j);
									localVarsInit += "\n\tCALL\tstringcopy\n";
									break;
								}
							}
							break;
						}
					}
					j++;
				}
				asmText += "\n";
				int startOfFunc = i;
				while (lextable.table[startOfFunc++].lexema != LEX_LEFTBRACE);
				int endOfFunc = i;
				int bracesOpened = 0;
				while (true)
				{
					if (lextable.table[endOfFunc].lexema == LEX_BRACELET && bracesOpened == 1) break;
					switch (lextable.table[endOfFunc].lexema)
					{
						case LEX_LEFTBRACE:
						{
							bracesOpened++;
							break;
						}
						case LEX_BRACELET:
						{
							bracesOpened--;
							break;
						}
					}
					endOfFunc++;
				}
				asmText += localVarsInit;
				asmText += GetFunctionASMCode(lextable, idtable, startOfFunc, endOfFunc);
				asmText += idtable.table[lextable.table[i].idxTI].id;
				asmText += " ENDP\n";
			}
		}

		asmText += "\n\nEND main";

		for (int i = 0; i < asmText.length(); i++) outStream << asmText[i];
	}
}