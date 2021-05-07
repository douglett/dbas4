#pragma once


void runtime_function(const string& fname) {

}


void ttr_function() {
	inp.load({
		"function foo()",
		// "	dim a",
		// "	let a = 1",
		"	print \"result: \", a",
		"end function",
		"",
	});
	ps_function();
	outp.show();
	runtime_function("foo");
}