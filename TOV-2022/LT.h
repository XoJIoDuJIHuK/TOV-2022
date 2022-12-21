#pragma once

#include <iostream>
#include <vector>
#include "Error.h"
#include "In.h"
#include "Out.h"
#include "Log.h"
#include "IT.h"

#define LEXEMA_FIXSIZE			1
#define LT_MAXSIZE				4096
#define LT_TI_NULLIDX			0xffffffff
#define LEX_ID					'i'
#define LEX_LITERAL				'l'
#define LEX_FUNCTION			'f'
#define LEX_TYPE				't'
#define LEX_VAR					'n'
#define LEX_RETURN				'r'
#define LEX_PRINT				'p'
#define LEX_SEMICOLON			';'
#define LEX_COMMA				','
#define LEX_LEFTBRACE			'{'
#define LEX_BRACELET			'}'
#define LEX_LEFTHESIS			'('
#define LEX_RIGHTHESIS			')'
#define LEX_COMPARISONSIGN		'u'
#define LEX_ARITHMETICALSIGN	'v'
#define LEX_EQUALS				'w'
#define LEX_MAIN				'm'
#define LEX_ADD					'a'
#define LEX_IF					'c'
#define LEX_ELSE				'e'
#define LEX_CALLFUNC			'@'
#define LEX_PLACEHOLDER			'#'

namespace LT
{
	enum SIGNTYPE { DEFAULT = 0, PLUS, MINUS, MULTIPLY, DIVIDE, LESS, LESSOREQUALS, MORE, MOREOREQUALS,
		EQUALS, NOTEQUAL, ASSIGNMENT };
	struct Entry
	{
		char lexema;
		int sn;
		int idxTI;
		int priority = -1;
		std::vector<IT::IDDATATYPE> params;
		SIGNTYPE signtype = DEFAULT;
		IT::IDDATATYPE datatype = IT::INT;
	};
	struct LexTable
	{
		int maxsize;
		int size = 0;
		Entry* table;
	};
	LexTable Create(int size);
	void Add(LexTable& lextable, Entry entry);
	Entry GetEntry(LexTable& lextable, int n);
	void Delete(LexTable& lextable);
	void Print(LexTable, Log::LOG log);
	void PrintConsole(LexTable table, Log::LOG log);
	void LexAnalyze(In::IN& in, Log::LOG log, LT::LexTable& lextable, IT::IdTable& idtable);
}