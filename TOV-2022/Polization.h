#pragma once
#include "IT.h"
#include "LT.h"
#include "Log.h"

bool PolishNotation(int i, LT::LexTable& lextable, IT::IdTable& idtable, bool writeToLog = true);
bool startPolishNotation(LT::LexTable& lextable, IT::IdTable& idtable, Log::LOG log, bool writeToLog = true);