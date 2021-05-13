#pragma once

extern Input    inp;
extern OutputB  outp;
extern Method2  meth;


void tt_start(const string& name) { printf("Running test [%s] ... ", name.c_str()); }
void tt_ok   () { printf("OK.\n"); }
void tt_err  () { printf("Error.\n"); }


void tt_struct() {
	tt_start("tt_struct");
	outp.clear();
	inp.load({
		"struct test",
		"	a1",
		"	b2",
		"	c3",
		"end struct",
		// "",
	});
	meth.ps_struct();
	tt_ok();
}
void tt_struct2() {
	tt_start("tt_struct2");
	outp.clear();
	inp.load({
		"struct test1",
		"	a",
		"	b[]",
		"end struct",
		"",
		"struct test1",
		"	float a",
		"	float b[]",
		"end struct",
	});
	meth.ps_segment("struct");
	tt_ok();
}
void tt_globals() {
	tt_start("tt_globals");
	outp.clear();
	inp.load({
		"dim a",
		"dim b[]",
		"",
		"dim float c[]",
	});
	meth.ps_segment("dim");
	tt_ok();
}
void tt_function() {
	tt_start("tt_function");
	outp.clear();
	inp.load({
		"function foo()",
		"",
		"end function",
	});
	meth.ps_function();
	tt_ok();
}
void tt_function2() {
	tt_start("tt_function2");
	outp.clear();
	inp.load({
		"function bar(a, float b[])",
		"",
		"end function",
	});
	meth.ps_function();
	tt_ok();
}
void tt_function3() {
	tt_start("tt_function3");
	outp.clear();
	inp.load({
		"function foo()",
		"	print \"hello world\"",
		"	let a = 1 + 2 * 3",
		"end function",
	});
	meth.ps_function();
	tt_ok();
}
void tt_expression() {
	tt_start("tt_expression");
	outp.clear();
	inp.load({
		"5 + 3 * 2",
	});
	meth.ps_expression();
	tt_ok();
}
void tt_if() {
	tt_start("tt_if");
	outp.clear();
	inp.load({
		"if a",
		"	let a = 100 * b",
		"end if",
		"",
	});
	meth.ps_if();
	tt_ok();
}
void tt_if2() {
	tt_start("tt_if2");
	outp.clear();
	inp.load({
		"if a",
		"	print \"hello world\", a",
		"	let a = 100 * b",
		"	if b",
		"		let b[0] = 10",
		"	end if",
		"end if",
		"",
	});
	meth.ps_if();
	tt_ok();
}
void tt_variables() {
	tt_start("tt_variables");
	outp.clear();
	inp.load({
		"foo.bar[10].foo"
	});
	meth.ps_varpath();
	tt_ok();
}

void tt_block_large() {
	tt_start("tt_block_large");
	outp.clear();
	inp.load({
		"print \"hello world\", a, \"ass\"",
		"input a",
		"input \"assy \", a",
		"",
		"if a",
		"end if",
		"",
		"while a",
		"end while",
		"",
		"return",
		"return 10",
		"",
		"call a()",
		"call b(a, 10)",
		"",
		"set a = b",
		"let a = 10",
		"let a = 10",
		"",
		"end"
	});
	meth.ps_block();
	tt_ok();
}



// run all tests above
void tt_run_all() {
	printf("-----\n");
	printf("> running tests...\n");

	// vector<pair<string, void(*)()>> tests = {
	// 	{ "tt_struct", &tt_struct },
	// };
	
	tt_struct();
	tt_struct2();
	tt_globals();
	tt_function();
	tt_function2();
	tt_function3();
	tt_expression();
	tt_if();
	tt_if2();
	tt_variables();
	tt_block_large();
	// outp.show();

	printf("> tests OK.\n");
	printf("-----\n");
}