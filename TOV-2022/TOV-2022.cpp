#include "stdafx.h"
#include "MFST.h"
#include "Polization.h"
#include <time.h>

int _tmain(int argc, _TCHAR* argv[])
{
	setlocale(LC_ALL, "rus");

	clock_t start = clock();

	Log::LOG log = Log::INITLOG;
	LT::LexTable lextable;
	IT::IdTable idtable;
	try
	{
		Parm::PARM parm = Parm::getparm(argc, argv);
		log = Log::getlog(parm.log);
		Log::WriteLog(log);
		Log::WriteParm(log, parm);
		In::IN in = In::getin(parm);
		Log::WriteIn(log, in);

		idtable = IT::Create(4096);
		lextable = LT::Create(4096);

		LT::LexAnalyze(in, log, lextable, idtable);

		MFST::SyntAnalize(lextable, log);
		SemAnalyze(idtable, lextable);
		if (startPolishNotation(lextable, idtable, log, true)) *log.stream << "polish successfull\n";
		else *log.stream << "polish unsuccessful\n";

		LT::Print(lextable, log);
		IT::Print(idtable, log);
		CodeGen::StartCodeGeneration(lextable, idtable);
	}
	catch (Error::ERROR e)
	{
		Log::WriteError(log, e);
		Error::ErrorDisplay(e);
		LT::PrintConsole(lextable, log);
		IT::PrintConsole(idtable, log);
	}

	clock_t end = clock();

	cout << "Seconds elapsed: " << (float)(end - start) / CLOCKS_PER_SEC << endl;

	return 0;
}