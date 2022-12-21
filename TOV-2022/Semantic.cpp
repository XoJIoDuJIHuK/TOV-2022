#include "Semantic.h"
#include "LT.h"
#include "IT.h"
#include <iostream>
#include <vector>

void TypeCheck(IT::IDDATATYPE type, IT::IdTable idtable, LT::LexTable lextable, int start, int end)
{
	for (int i = start; i < end; i++)
	{
		if (lextable.table[i].idxTI == -1) continue;
		if (type != idtable.table[lextable.table[i].idxTI].iddatatype)
		{
			if (lextable.table[start - 2].lexema == LEX_IF) throw ERROR_THROW_IN(164, lextable.table[i].sn, -1);
			throw ERROR_THROW_IN(152, lextable.table[i].sn, -1);
		}
		switch (idtable.table[lextable.table[i].idxTI].idtype)
		{
			case IT::L:
			case IT::V:
			{
				if (lextable.table[i + 1].lexema == LEX_LEFTHESIS)
				{
					throw ERROR_THROW_IN(156, lextable.table[i].sn, -1);
				}
				break;
			}
			case IT::F:
			{
				if (lextable.table[i + 1].lexema != LEX_LEFTHESIS) throw 
					ERROR_THROW_IN(157, lextable.table[i].sn, -1);
				int funcIndex = i;
				i += 2;
				int paramCount = 0;
				std::vector<IT::IDDATATYPE> params = lextable.table[idtable.table[lextable.
					table[funcIndex].idxTI].idxfirstLE].params;
				for (; lextable.table[i].lexema != LEX_RIGHTHESIS; i++)
				{
					if (paramCount >= params.size())
						throw ERROR_THROW_IN(154, lextable.table[i].sn, -1);
					if (lextable.table[i].lexema == LEX_COMMA) continue;
					if (params[paramCount] != idtable.table[lextable.table[i].idxTI].iddatatype)
						throw ERROR_THROW_IN(153, lextable.table[i].sn, -1);
					paramCount++;
				}
				if (params.size() != paramCount) throw ERROR_THROW_IN(155, lextable.table[i].sn, -1);
				break;
			}
		}
	}
}

void SemAnalyze(IT::IdTable idtable, LT::LexTable lextable)
{
	for (int i = 0; i < lextable.size; i++)
	{
		if (lextable.table[i].lexema == LEX_EQUALS || lextable.table[i].lexema == 'r')
		{
			int j = i;
			IT::IDDATATYPE type;
			if (lextable.table[j].lexema == LEX_EQUALS)
			{
				if (lextable.table[i - 1].lexema != LEX_ID) throw ERROR_THROW_IN(162, lextable.table[i].sn, -1);
				if ((lextable.table[i - 2].lexema != LEX_LEFTBRACE) &&
					(lextable.table[i - 2].lexema != LEX_BRACELET) &&
					(lextable.table[i - 2].lexema != LEX_SEMICOLON))
					throw ERROR_THROW_IN(163, lextable.table[i].sn, -1);
				type = idtable.table[lextable.table[j - 1].idxTI].iddatatype;
			}
			else if (lextable.table[j].lexema == 'r')
			{
				type = lextable.table[j].datatype;
			}
			int start = ++j;
			for (; lextable.table[j].lexema != LEX_SEMICOLON;) j++;
			TypeCheck(type, idtable, lextable, start, j);
		}
		if (lextable.table[i].lexema == LEX_IF)
		{
			int j = i + 2;
			IT::IDDATATYPE type = IT::INT;
			while (true)
			{
				if (lextable.table[j].lexema == LEX_ID || lextable.table[j].lexema == LEX_LITERAL)
				{
					type = idtable.table[lextable.table[j].idxTI].iddatatype;
					break;
				}
				j++;
			}
			j = i;
			int start = i + 2;
			int indexOfComparisonSign = 0;
			int amountOfComparisonSigns = 0;
			while (lextable.table[j].lexema != LEX_LEFTBRACE)
			{
				if (lextable.table[j].signtype >= LT::LESS && lextable.table[j].signtype <= LT::NOTEQUAL)
				{
					amountOfComparisonSigns++;
					indexOfComparisonSign = j;
					if (amountOfComparisonSigns > 1) throw ERROR_THROW_IN(158, lextable.table[j].sn, -1);
				}
				j++;
			}
			if (amountOfComparisonSigns == 0) throw ERROR_THROW_IN(159, lextable.table[j].sn, -1);
			int end = --j;
			TypeCheck(type, idtable, lextable, start, indexOfComparisonSign);
			TypeCheck(type, idtable, lextable, indexOfComparisonSign + 1, end);
		}
		if (lextable.table[i].signtype >= LT::MINUS && lextable.table[i].signtype <= LT::DIVIDE)
		{
			int j = i;
			while (lextable.table[j].lexema != LEX_ID && lextable.table[j].lexema != LEX_LITERAL) j++;
			if (idtable.table[lextable.table[j].idxTI].iddatatype == IT::STR || 
				idtable.table[lextable.table[j].idxTI].iddatatype == IT::SYM) throw ERROR_THROW_IN(161,
				lextable.table[j].sn, -1);
		}
		if (lextable.table[i].idxTI != -1 && idtable.table[lextable.table[i].idxTI].idtype == IT::F && 
			idtable.table[lextable.table[i].idxTI].idxfirstLE != i)
		{
			for (int j = i + 2; lextable.table[j].lexema != LEX_RIGHTHESIS; j++)
			{
				int numOfParam = 0;
				if (lextable.table[j].lexema == LEX_COMMA) continue;
				if (lextable.table[idtable.table[lextable.table[i].idxTI].idxfirstLE].params.size() <=
					numOfParam)
					throw ERROR_THROW_IN(154, lextable.table[i].sn, -1);
				if (idtable.table[lextable.table[j].idxTI].iddatatype !=
					lextable.table[idtable.table[lextable.table[i].idxTI].idxfirstLE].params[numOfParam])
				{
					throw ERROR_THROW_IN(153, lextable.table[j].sn, -1);
				}
				numOfParam++;
			}
		}
		if (lextable.table[i].idxTI != -1 &&
			idtable.table[lextable.table[i].idxTI].idtype == IT::F && idtable.table[lextable.table[i].idxTI].
			idxfirstLE != i)
		{
			int numOfParams = 0;
			for (int j = i + 2; lextable.table[j].lexema != LEX_RIGHTHESIS; j++)
			{
				if (lextable.table[j].lexema == LEX_COMMA) continue;
				if (numOfParams >= lextable.table[idtable.table[lextable.table[i].idxTI].idxfirstLE].
					params.size())
				{
					throw ERROR_THROW_IN(154, lextable.table[i].sn, -1);
				}
				if (lextable.table[idtable.table[lextable.table[i].idxTI].idxfirstLE].params[numOfParams] !=
					idtable.table[lextable.table[j].idxTI].iddatatype)
				{
					throw ERROR_THROW_IN(153, lextable.table[j].sn, -1);
				}
				numOfParams++;
			}
			if (numOfParams != lextable.table[idtable.table[lextable.table[i].idxTI].idxfirstLE].params.size())
			{
				throw ERROR_THROW_IN(155, lextable.table[i].sn, -1);
			}
		}
	}
}