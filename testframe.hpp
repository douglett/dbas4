// ----------------------------------------
// Testing Framework
// ----------------------------------------
#pragma once

struct TestFrame {
	const char 
		*TERM_DEF = "\033[0m",
		*TERM_RED = "\033[31m",
		*TERM_GREEN = "\033[32m";
	
	Input    inp;
	OutputB  outp;
	Method2  meth = { inp, outp };

	void tt_start(const string& name) { printf("    Running test [%s] ... ", name.c_str());  inp.close();  outp.clear(); }
	int  tt_ok   () { printf("%sOK%s.\n", TERM_GREEN, TERM_DEF);  return 0; }
	int  tt_err  () { printf("%sError%s.\n", TERM_RED, TERM_DEF);  return 1; }

	int runall() {
		outp.warn_flag = 0;  // switch off state warnings
		printf(">>> running %s tests...\n", getname().c_str());
		int errc = run_tests();
		if   (errc == 0)  { printf("    %s:All tests passed.%s\n", TERM_GREEN, TERM_DEF);  return 0; }
		else              { printf("    %s:Found %d errors.%s\n", TERM_RED, errc, TERM_DEF);  return 1; }
	}

	// override
	virtual string getname()   { return "TestFrame"; }
	virtual int    run_tests() { return 0; }
};