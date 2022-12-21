#pragma once
#include <stack>
#include <iomanip>
#include "Greibach.h"
#include "LT.h"

using namespace LT;
using namespace std;

#define MFST_DIAGN_MAXSIZE 2 * ERROR_MAXSIZE_MESSAGE
#define MFST_DIAGN_NUMBER 3
#define MFST_TRACE_START *log.stream << setw(4) << left << "Шаг" << ":"\
						<< setw(20) << left << " Правило"\
						<< setw(30) << left << " Входная лента"\
						<< setw(20) << left << " Стек"\
						<< '\n';

#define MFST_TRACE1 *log.stream<<setw(4)<<left<<++FST_TRACE_n<<": "\
	<<setw(20)<<left<<rule.getCRule(rbuf,nrulechain)\
	<<setw(30)<<left<<getCLenta(lbuf,lenta_position)\
	<<setw(20)<<left<<getCSt(sbuf)<<'\n';

#define MFST_TRACE2 *log.stream<<setw(4)<<left<<FST_TRACE_n<<": "\
	<<setw(20)<<left<<" "<<setw(30)<<left<<getCLenta\
	(lbuf, lenta_position)<<setw(20)<<left<<getCSt(sbuf)<<'\n';

#define MFST_TRACE3 *log.stream<<setw(4)<<left<<++FST_TRACE_n<<": "\
	<<setw(20)<<left<<" "\
	<<setw(30)<<left<<getCLenta(lbuf,lenta_position)\
	<<setw(20)<<left<<getCSt(sbuf)\
	<<'\n';

#define MFST_TRACE4(c) *log.stream<<setw(4)<<left<<++FST_TRACE_n<<": "<<setw(20)<<left<<c<<'\n';

#define MFST_TRACE5(c) *log.stream<<setw(4)<<left<<FST_TRACE_n<<": "<<setw(20)<<left<<c<<'\n';

#define MFST_TRACE6(c,k) *log.stream<<setw(4)<<left<<FST_TRACE_n<<": "<<setw(20)<<left<<c<<k<<'\n';

#define MFST_TRACE7 *log.stream<<setw(4)<<left<<state.lenta_position<<": "\
	<<setw(20)<<left<<rule.getCRule(rbuf, state.nrulechain)\
	<<'\n';

typedef stack<short> MFSTSTACK;

namespace MFST
{
	struct MfstState
	{
		short lenta_position;
		short nrule;
		short nrulechain;
		MFSTSTACK st;
		MfstState();
		MfstState(short pposition, MFSTSTACK pst, short pnrulechain);
		MfstState(short pposition, MFSTSTACK pst, short pnrule, short pnrulechin);
	};
	struct Mfst
	{
		enum RC_STEP
		{
			NS_OK,
			NS_NORULE,
			NS_NORULECHAIN,
			NS_ERROR,
			TS_OK,
			TS_NOK,
			LENTA_END,
			SURPRISE
		};
		struct MfstDiagnosis
		{
			short lenta_position;
			RC_STEP rc_step;
			short nrule, nrulechain;
			MfstDiagnosis();
			MfstDiagnosis(short plenta_position, RC_STEP prc_step, short pnrule, short pnrule_chain);
		} diagnosis[MFST_DIAGN_NUMBER];
		GRBALPHABET* lenta;
		short lenta_position;
		short nrule;
		short nrulechain;
		short lenta_size;
		GRB::Greibach greibach;
		LexTable lex;
		MFSTSTACK st;
		stack<MfstState> storestate;
		Mfst();
		Mfst(LexTable plex, GRB::Greibach pgreibach);
		char* getCSt(char* buf);
		char* getCLenta(char* buf, short pos, short n = 25);
		char* getDiagnosis(short n, char* buf);
		bool savestate(Log::LOG log);
		bool reststate(Log::LOG log);
		bool push_chain(GRB::Rule::Chain chain);
		RC_STEP step(Log::LOG log);
		bool start(Log::LOG log);
		bool savediagnosis(RC_STEP pprc_step);
		void printrules(Log::LOG log);
		struct Deducation
		{
			short size;
			short* nrules;
			short* nrulechains;
			Deducation() { size = 0; nrules = 0; nrulechains = 0; }
		} deducation;
		bool savededucation();
	};
	void SyntAnalize(LT::LexTable lextable, Log::LOG log);
}