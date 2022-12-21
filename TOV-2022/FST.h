#pragma once
#include "In.h"
#include <iostream>
#include <string>
#include <vector>
namespace FSTN
{
	struct RELATION
	{
		char symbol;
		short nnode;
		RELATION(char, short);
	};

	struct NODE
	{
		short n_relation;
		RELATION* relations;
		NODE();
		NODE(short, RELATION, ...);
	};

	struct FSTsmall
	{
		std::string word;//разбираемое слово
		short position;//текущая позиция в слове
		short nstates;//количество состояний автомата
		NODE* nodes;//массив всевозможных состояний автомата
		short* rstates;//массив возможных состояний графа в данный момент
		FSTsmall(short, NODE, ...);
	};


	bool step(FSTsmall&, short*);
	bool execute(In::IN, FSTsmall, std::string);
}