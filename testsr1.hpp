// ----------------------------------------
// Tests for Runtime 1
// ----------------------------------------
#pragma once


extern Input    inp;
extern OutputB  outp;
extern Method2  meth;

Runtime1 run;


void ttr_function() {
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
	run.r_func("foo");
}