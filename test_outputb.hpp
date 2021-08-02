// ----------------------------------------
// Tetst for Parsing Output B
// ----------------------------------------
#pragma once
#include "testframe.hpp"


struct TestOutputB : TestFrame {
	virtual string getname()   { return "TestOutputB"; }

	virtual int run_tests() {
		int errc = 0;
		errc += tt_redim();
		// errc += tt_undimmed();
		return errc;
	}


	int tt_redim() {
		tt_start("tt_redim");
		outp.clear();
		inp.load({
			"dim a",
			"dim a",
		});
		try {
			meth.ps_segment("dim");
			return tt_err();  // no error = failure
		}
		catch (ParseError e) {
			return tt_ok();   // error = success
		}
	}
	int tt_undimmed() {
		tt_start("tt_undimmed");
		outp.clear();
		inp.load({
			// "dim a",
			"let a = 1",
			// "",
		});
		try {
			// meth.ps_block();
			meth.ps_let();
			tt_err();  // no error = failure
			return 1;
		}
		catch (ParseError e) {
			tt_ok();   // error = success
			throw e;
			return 0;
		}
	}
};