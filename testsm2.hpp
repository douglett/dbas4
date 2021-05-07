#pragma once


void tt_struct() {
	inp.load({
		"struct test",
		"	a1",
		"	b2",
		"	c3",
		"end struct",
		// "",
	});
	ps_struct();
}
void tt_struct2() {
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
	ps_segment("struct");
}
void tt_globals() {
	inp.load({
		"dim a",
		"dim b[]",
		"",
		"dim float c[]",
	});
	ps_segment("dim");
}
void tt_function() {
	inp.load({
		"function foo()",
		"",
		"end function",
	});
	ps_function();
}
void tt_function2() {
	inp.load({
		"function bar(a, float b[])",
		"",
		"end function",
	});
	ps_function();
}
void tt_function3() {
	inp.load({
		"function foo()",
		"	print \"hello world\"",
		"	let a = 1 + 2 * 3",
		"end function",
	});
	ps_function();
}
void tt_expression() {
	inp.load({
		"5 + 3 * 2",
	});
	ps_expression();
}
void tt_if() {
	inp.load({
		"if a",
		"	let a = 100 * b",
		"end if",
		"",
	});
	ps_if();
}
void tt_if2() {
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
	ps_if();
}
void tt_variables() {
	inp.load({
		"foo.bar[10].foo"
	});
	ps_varpath();
}

void tt_block_large() {
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
	ps_block();
}