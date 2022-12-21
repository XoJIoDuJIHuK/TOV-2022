#include "Greibach.h"
#include <string.h>

namespace GRB
{
	Rule::Chain::Chain(short psize, GRBALPHABET s, ...)
	{
		nt = new GRBALPHABET[size = psize];
		int* p = (int*)&s;
		for (short i = 0; i < psize; ++i) nt[i] = (GRBALPHABET)p[i];
	}
	Rule::Rule(GRBALPHABET pnn, int piderror, short psize, Chain c, ...)
	{
		nn = pnn;
		iderror = piderror;
		chains = new Chain[size = psize];
		Chain* p = &c;
		for (int i = 0; i < size; i++) chains[i] = p[i];
	}
	Greibach::Greibach(GRBALPHABET pstartN, GRBALPHABET pstbottomT, short psize, GRB::Rule r, ...)
	{
		startN = pstartN;
		stbottomT = pstbottomT;
		rules = new GRB::Rule[size = psize];
		GRB::Rule* p = &r;
		for (int i = 0; i < size; i++) rules[i] = p[i];
	}
	Greibach greibachOriginal(NS('S'), TS('$'), 12,
		//     F HIJKL  O QR TUV XY 
		// b d   gh jk   o q s    xyz

		//S -> m{N}Z | m{N} | tfi(P)BZ | tfi(P)B | atfi(P);Z | atfi(P); | tfi()BZ | tfi()B | atfi();Z | 
		//atfi();
		Rule(NS('S'), GRB_ERRORS_START, 10,						// Неверная структура программы	
			Rule::Chain(5, TS('m'), TS('{'), NS('N'), TS('}'), NS('Z')),
			Rule::Chain(4, TS('m'), TS('{'), NS('N'), TS('}')),
			Rule::Chain(8, TS('t'), TS('f'), TS('i'), TS('('), NS('P'), TS(')'), NS('B'), NS('Z')),
			Rule::Chain(7, TS('t'), TS('f'), TS('i'), TS('('), NS('P'), TS(')'), NS('B')),
			Rule::Chain(7, TS('t'), TS('f'), TS('i'), TS('('), TS(')'), NS('B'), NS('Z')),
			Rule::Chain(6, TS('t'), TS('f'), TS('i'), TS('('), TS(')'), NS('B')),
			Rule::Chain(9, TS('a'), TS('t'), TS('f'), TS('i'), TS('('), NS('P'), TS(')'), TS(';'), NS('Z')),
			Rule::Chain(8, TS('a'), TS('t'), TS('f'), TS('i'), TS('('), NS('P'), TS(')'), TS(';')),
			Rule::Chain(8, TS('a'), TS('t'), TS('f'), TS('i'), TS('('), TS(')'), TS(';'), NS('Z')),
			Rule::Chain(7, TS('a'), TS('t'), TS('f'), TS('i'), TS('('), TS(')'), TS(';'))
		),
		//Z -> mBZ | mB | tfi(P)BZ | tfi(P)B | atfi(P);Z | atfi(P);
		Rule(NS('Z'), GRB_ERRORS_START, 10,						// Неверная структура программы	
			Rule::Chain(5, TS('m'), TS('{'), NS('N'), TS('}'), NS('Z')),
			Rule::Chain(4, TS('m'), TS('{'), NS('N'), TS('}')),
			Rule::Chain(8, TS('t'), TS('f'), TS('i'), TS('('), NS('P'), TS(')'), NS('B'), NS('Z')),
			Rule::Chain(7, TS('t'), TS('f'), TS('i'), TS('('), NS('P'), TS(')'), NS('B')),
			Rule::Chain(7, TS('t'), TS('f'), TS('i'), TS('('), TS(')'), NS('B'), NS('Z')),
			Rule::Chain(6, TS('t'), TS('f'), TS('i'), TS('('), TS(')'), NS('B')),
			Rule::Chain(9, TS('a'), TS('t'), TS('f'), TS('i'), TS('('), NS('P'), TS(')'), TS(';'), NS('Z')),
			Rule::Chain(8, TS('a'), TS('t'), TS('f'), TS('i'), TS('('), NS('P'), TS(')'), TS(';')),
			Rule::Chain(8, TS('a'), TS('t'), TS('f'), TS('i'), TS('('), TS(')'), TS(';'), NS('Z')),
			Rule::Chain(7, TS('a'), TS('t'), TS('f'), TS('i'), TS('('), TS(')'), TS(';'))
		),
		//B -> NrE; | rE;
		Rule(NS('B'), GRB_ERRORS_START + 2, 2,						// Функция обязана возвращать значение
			Rule::Chain(6, TS('{'), NS('N'), TS('r'), NS('E'), TS(';'), TS('}')),
			Rule::Chain(5, TS('{'), TS('r'), NS('E'), TS(';'), TS('}'))
		),
		//N -> nti; | nti;N | iwE;N | iwE; | pE; | pE;N | cCAN | cCA
		Rule(NS('N'), GRB_ERRORS_START + 3, 8,					// Недопустимая инструкция в теле функции	
			Rule::Chain(5, TS('n'), TS('t'), TS('i'), TS(';'), NS('N')),
			Rule::Chain(4, TS('n'), TS('t'), TS('i'), TS(';')),
			Rule::Chain(5, TS('i'), TS('w'), NS('E'), TS(';'), NS('N')),
			Rule::Chain(4, TS('i'), TS('w'), NS('E'), TS(';')),
			Rule::Chain(4, TS('p'), NS('E'), TS(';'), NS('N')),
			Rule::Chain(3, TS('p'), NS('E'), TS(';')),
			Rule::Chain(4, TS('c'), NS('C'), NS('A'), NS('N')),
			Rule::Chain(3, TS('c'), NS('C'), NS('A'))
		),
		//E-> i | l | (E) | i(W) | iM | lM | (E)M | i(W)M		Неверно составленное выражение
		Rule(NS('E'), GRB_ERRORS_START + 4, 10,
			Rule::Chain(2, TS('i'), NS('M')),
			Rule::Chain(1, TS('i')),
			Rule::Chain(2, TS('l'), NS('M')),
			Rule::Chain(1, TS('l')),
			Rule::Chain(4, TS('('), NS('E'), TS(')'), NS('M')),
			Rule::Chain(3, TS('('), NS('E'), TS(')')),
			Rule::Chain(5, TS('i'), TS('('), NS('W'), TS(')'), NS('M')),
			Rule::Chain(4, TS('i'), TS('('), NS('W'), TS(')')),
			Rule::Chain(4, TS('i'), TS('('), TS(')'), NS('M')),
			Rule::Chain(3, TS('i'), TS('('), TS(')'))
		),
		//M-> vE | vEM		Неверное арифметическое выражение
		Rule(NS('M'), GRB_ERRORS_START + 5, 2,
			Rule::Chain(3, TS('v'), NS('E'), NS('M')),
			Rule::Chain(2, TS('v'), NS('E'))
		),
		//P-> ti | ti,P		Неверный список параметров
		Rule(NS('P'), GRB_ERRORS_START + 6, 2,
			Rule::Chain(4, TS('t'), TS('i'), TS(','), NS('P')),
			Rule::Chain(2, TS('t'), TS('i'))
		),
		//W-> i | l | i,W | l,W		Неверный список аргументов
		Rule(NS('W'), GRB_ERRORS_START + 7, 4,
			Rule::Chain(3, TS('i'), TS(','), NS('W')),
			Rule::Chain(1, TS('i')),
			Rule::Chain(3, TS('l'), TS(','), NS('W')),
			Rule::Chain(1, TS('l'))
		),
		//A-> {D}Ge{D} | {D}G | {D}e{D} | {D}		Неверно составленное условное выражение
		Rule(NS('A'), GRB_ERRORS_START + 8, 4,
			Rule::Chain(8, TS('{'), NS('D'), TS('}'), NS('G'), TS('e'), TS('{'), NS('D'), TS('}')),
			Rule::Chain(4, TS('{'), NS('D'), TS('}'), NS('G')),
			Rule::Chain(7, TS('{'), NS('D'), TS('}'), TS('e'), TS('{'), NS('D'), TS('}')),
			Rule::Chain(3, TS('{'), NS('D'), TS('}'))
		),
		//D-> iwE;D	| iwE; | pE;D | pE;		В условном блке могут содержаться только операции присваивания и вывода
		Rule(NS('D'), GRB_ERRORS_START + 9, 4,
			Rule::Chain(5, TS('i'), TS('w'), NS('E'), TS(';'), NS('D')),
			Rule::Chain(4, TS('i'), TS('w'), NS('E'), TS(';')),
			Rule::Chain(4, TS('p'), NS('E'), TS(';'), NS('D')),
			Rule::Chain(3, TS('p'), NS('E'), TS(';'))
		),
		//G-> ecC{D} | ecC{D}G		Неверно составленные блоки else if
		Rule(NS('G'), GRB_ERRORS_START + 10, 2,
			Rule::Chain(7, TS('e'), TS('c'), NS('C'), TS('{'), NS('D'), TS('}'), NS('G')),
			Rule::Chain(6, TS('e'), TS('c'), NS('C'), TS('{'), NS('D'), TS('}'))
		),
		//C -> (EuE) Условием может являться только сравнение двух выражений
		Rule(NS('C'), GRB_ERRORS_START + 11, 1,
			Rule::Chain(5, TS('('), NS('E'), TS('u'), NS('E'), TS(')'))
		)
	);
	Greibach getGreibach() { return greibachOriginal; }
	short Greibach::getRule(GRBALPHABET pnn, GRB::Rule& prule)
	{
		short rc = -1, k = 0;
		while (k < size && rules[k].nn != pnn) k++;
		if (k < size) prule = rules[rc = k];
		return rc;
	}
	GRB::Rule Greibach::getRule(short n)
	{
		GRB::Rule rc;
		if (n < size) rc = rules[n];
		return rc;
	}
	char* Rule::getCRule(char* b, short nchain)
	{
		char bchain[200];
		b[0] = Chain::alphabet_to_char(nn);
		b[1] = '-';
		b[2] = '>';
		b[3] = 0;
		chains[nchain].getCChain(bchain);
		strcat_s(b, sizeof(bchain) + 5, bchain);
		return b;
	}
	short Rule::getNextChain(GRBALPHABET t, Rule::Chain& pchain, short j)
	{
		short rc = -1;
		while (j < size && chains[j].nt[0] != t) ++j;
		rc = (j < size ? j : -1);
		if (rc >= 0) pchain = chains[rc];
		return rc;
	}
	char* Rule::Chain::getCChain(char* b)
	{
		for (int i = 0; i < size; i++) b[i] = Chain::alphabet_to_char(nt[i]);
		b[size] = 0;
		return b;
	}
}