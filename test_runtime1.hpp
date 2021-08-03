// ----------------------------------------
// Tests for Runtime 1
// ----------------------------------------
#pragma once
#include "testframe.hpp"


struct TestRuntime1 : TestFrame {
	Runtime1 run1 = { outp };

	virtual string getname()   { return "TestRuntime1"; }

	virtual int run_tests() {
		int errc = 0;
		// errc += ttr_function();
		return errc;
	}


	int ttr_function() {
		tt_start("ttr_function");
		outp.clear();
		inp.load({
			"function foo()",
			"	dim a = 101",
			"	print \"result: \", a",
			"	let a = 11*2+3",
			"	print \"maths result: \", a",
			"end function",
			"",
		});
		meth.ps_function();
		// outp.show();
		printf("-----\n");
		printf("<running>\n");
		run1.r_func("foo");
		return tt_ok();
	}

	// void ttr_text() {
	// 	outp.clear();
	// 	inp.load({
	// 		"function foo()",
	// 		"	prints a, \"hello world \", 101",
	// 		// "	let b = 1",
	// 		"end function",
	// 		"",
	// 	});
	// 	meth.ps_function();
	// 	outp.show();
	// 	printf("-----\n");
	// 	printf("<running>\n");
	// 	// run.r_func("foo");
	// }
};