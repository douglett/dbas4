// ----------------------------------------
// Tesst for Parsing Method 2
// ----------------------------------------
#pragma once
#include "testframe.hpp"


struct TestMethod2 : TestFrame {
	virtual string getname()   { return "TestMethod2"; }

	virtual int run_tests() {
		int errc = 0;
		errc += tt_struct();
		errc += tt_struct2();
		errc += tt_globals();
		errc += tt_function();
		errc += tt_function2();
		errc += tt_function3();
		errc += tt_expression();
		errc += tt_expression2();
		errc += tt_if();
		errc += tt_if2();
		errc += tt_spacing();
		errc += tt_variables();
		errc += tt_block_large();
		return errc;
	}


	int tt_struct() {
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
		return 0;
	}
	int tt_struct2() {
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
		return 0;
	}
	int tt_globals() {
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
		return 0;
	}
	int tt_function() {
		tt_start("tt_function");
		outp.clear();
		inp.load({
			"function foo()",
			"",
			"end function",
		});
		meth.ps_function();
		tt_ok();
		return 0;
	}
	int tt_function2() {
		tt_start("tt_function2");
		outp.clear();
		inp.load({
			"function bar(a, float b[])",
			"",
			"end function",
		});
		meth.ps_function();
		tt_ok();
		return 0;
	}
	int tt_function3() {
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
		return 0;
	}
	int tt_expression() {
		tt_start("tt_expression");
		outp.clear();
		inp.load({
			"5 + 3 * 2",
		});
		meth.ps_expression();
		tt_ok();
		return 0;
	}
	int tt_expression2() {
		tt_start("tt_expression2");
		outp.clear();
		inp.load({
			"1 == 2  "
			"1==2  "
			"1<2  "
		});
		meth.ps_expression();
		meth.ps_expression();
		meth.ps_expression();
		tt_ok();
		return 0;
	}
	int tt_if() {
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
		return 0;
	}
	int tt_if2() {
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
		return 0;
	}
	int tt_spacing() {
		tt_start("tt_spacing");
		outp.clear();
		inp.load({
			"printa", // expect failure
			""
		});
		try {
			meth.ps_print();
			tt_err();  // no error = failure
			return 1;
		}
		catch (ParseError e) {
			tt_ok();   // error = success
			return 0;
		}
	}
	int tt_variables() {
		tt_start("tt_variables");
		outp.clear();
		inp.load({
			"foo.bar[10].foo"
		});
		meth.ps_varpath();
		tt_ok();
		return 0;
	}

	int tt_block_large() {
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
		return 0;
	}
};