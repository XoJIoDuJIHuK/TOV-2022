#include "LT.h"
#include "IT.h"
#include "Log.h"
#include "Polization.h"
#include <stack>
#include <queue>

using namespace std;

void PolishNotationalize(int start, int end, LT::LexTable& lextable, IT::IdTable& idtable, Log::LOG log, bool writeToLog)
{
	LT::Entry placeholder;
	placeholder.lexema = LEX_PLACEHOLDER;
	placeholder.idxTI = -1;
	int length = end - start;
	stack<LT::Entry> stackPolish;
	LT::Entry* sourceArray = new LT::Entry[length];
	LT::Entry* destinationArray = new LT::Entry[length];
	for (int i = 0; i < length; i++) destinationArray[i] = placeholder;
	for (int i = start; i < end; i++)
		sourceArray[i - start] = lextable.table[i];
	int destArrSize = 0;

	for (int i = 0; i < length; i++)
	{
		switch (sourceArray[i].lexema)
		{
		case LEX_LITERAL:
		{
			destinationArray[destArrSize] = sourceArray[i];
			if (idtable.table[sourceArray[i].idxTI].idxfirstLE == start + i)//если лексема
				//ссылаетс€ на тот же идентификатор, который ссылаетс€ на неЄ
				idtable.table[sourceArray[i].idxTI].idxfirstLE = destArrSize + start;
			destArrSize++;
			break;
		}
		case LEX_ID:
		{
			if (idtable.table[lextable.table[start + i].idxTI].idtype == IT::F)
			{
				int j = i + 2;
				int amountOfParams = 0;
				stack<LT::Entry> stackOfParams;
				stack<int> stackOfPositions;
				for (; lextable.table[start + j].lexema != LEX_RIGHTHESIS; j++)
				{
					if (lextable.table[start + j].lexema == LEX_COMMA) continue;
					stackOfPositions.push(start + j);
					stackOfParams.push(lextable.table[start + j]);
					amountOfParams++;
				}
				for (int k = 0; k < amountOfParams; k++)
				{
					destinationArray[destArrSize] = stackOfParams.top();
					if (idtable.table[lextable.table[stackOfPositions.top()].idxTI].idxfirstLE == 
						stackOfPositions.top())
						//если лексема ссылаетс€ на тот же идентификатор, который ссылаетс€ на неЄ
						idtable.table[lextable.table[stackOfPositions.top()].idxTI].idxfirstLE = 
						destArrSize + start;
					stackOfPositions.pop();
					stackOfParams.pop();
					destArrSize++;
				}
				LT::Entry e;
				e.lexema = LEX_CALLFUNC;
				e.idxTI = -1;
				e.priority = amountOfParams;
				destinationArray[destArrSize++] = e;
				destinationArray[destArrSize] = lextable.table[start + i];
				if (idtable.table[lextable.table[i + start].idxTI].idxfirstLE == start + i)//если лексема
					//ссылаетс€ на тот же идентификатор, который ссылаетс€ на неЄ
					idtable.table[sourceArray[i].idxTI].idxfirstLE = destArrSize + start;
				destArrSize++;
				i = j;
			}
			else
			{
				destinationArray[destArrSize] = sourceArray[i];
				destArrSize++;
			}
			break;
		}
		case 'u':
		case 'v':
		{
			while (stackPolish.size() > 0 && stackPolish.top().priority >= lextable.table[i + start].priority)
			{
				destinationArray[destArrSize] = stackPolish.top();
				destArrSize++;
				stackPolish.pop();
			}
			if (stackPolish.size() == 0 || lextable.table[i + start].priority >
				stackPolish.top().priority)
			{
				stackPolish.push(lextable.table[i + start]);
			}
			break;
		}
		case LEX_LEFTHESIS:
		{
			stackPolish.push(lextable.table[start + i]);
			break;
		}
		case LEX_RIGHTHESIS:
		{
			while (stackPolish.size() > 0 && stackPolish.top().lexema != LEX_LEFTHESIS)
			{
				destinationArray[destArrSize] = stackPolish.top();
				destArrSize++;
				stackPolish.pop();
			}
			stackPolish.pop();
			break;
		}
		default:
		{
			break;
		}
		}
	}
	while (stackPolish.size() > 0)
	{
		destinationArray[destArrSize] = stackPolish.top();
		destArrSize++;
		stackPolish.pop();
	}
	if (writeToLog)
	{

		for (int i = 0; i < length; i++) *log.stream << sourceArray[i].lexema;
		*log.stream << "->";
		for (int i = 0; i < length; i++) *log.stream << destinationArray[i].lexema;
		*log.stream << endl;
	}
	for (int i = start; i < end; i++)
	{
		lextable.table[i] = destinationArray[i - start];
	}
	delete[] sourceArray;
	delete[] destinationArray;
}

bool startPolishNotation(LT::LexTable& lextable, IT::IdTable& idtable, Log::LOG log, bool writeToLog)
{
	bool rc = true;
	for (int i = 0; i < lextable.size; i++)
	{
		if (lextable.table[i].lexema == LEX_EQUALS ||
			lextable.table[i].lexema == LEX_RETURN ||//поиск выражений, которые необходимо преобразовать
			lextable.table[i].lexema == LEX_PRINT)
		{
			int start = i + 1;
			while (lextable.table[i].lexema != LEX_SEMICOLON) i++;//обнаружение конца выражени€
			PolishNotationalize(start, i, lextable, idtable, log, writeToLog);
		}
		if (lextable.table[i].lexema == LEX_IF)//поиск выражений, которые необходимо преобразовать
		{
			int start = i += 2;
			int parenthesesOpened = 0;
			while (lextable.table[i].lexema != LEX_LEFTBRACE) i++;//обнаружение конца выражени€
			PolishNotationalize(start, i - 1, lextable, idtable, log, writeToLog);
		}
	}
	return rc;
}