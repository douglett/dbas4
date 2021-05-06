#pragma once


const int tt_type = 1;


void tt_struct() {
	inp.load({
		"struct test",
		"	a1",
		"	b2",
		"	c3",
		"end struct",
		// "",
	});
	if      (tt_type == 1)  state.push(PState::PS_STRUCT),  ploop();
	// else if (tt_type == 2)  ps_struct();
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
	if      (tt_type == 1)  state.push(PState::PS_STRUCT_BLOCK),  ploop();
	// else if (tt_type == 2)  ps_segment("struct");
}
void tt_globals() {
	inp.load({
		"dim a",
		"dim b[]",
		"",
		"dim float c[]",
	});
	if      (tt_type == 1)  state.push(PState::PS_GLOBAL_BLOCK), ploop();
	// else if (tt_type == 2)  ps_segment("dim");
}
void tt_function() {
	inp.load({
		// "function foo()",
		"function bar(a, float b[])",
		// "",
		"end function",
	});
	if      (tt_type == 1)  state.push(PState::PS_FUNCTION), ploop();
	// else if (tt_type == 2)  ps_function();
}