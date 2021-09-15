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
		errc += tt_redim_func();
		// errc += tt_undimmed();
		return errc;
	}


	int tt_redim() {
		tt_start("tt_redim");
		inp.load({
			"dim a",
			"dim a",
		});
		try {
			meth.ps_segment("dim");
			return tt_err();  // no error = failure
		}
		catch (WizParseError e) {
			if    (e.error_code == WIZERR_REDIM)  return tt_ok();   // redim error = success
			else  return tt_err();  // incorrect error
		}
	}

	int tt_redim_func() {
		tt_start("tt_redim_func");
		inp.load({
			"function a(a, a)",
			"end function",
			"",
		});
		try {
			meth.ps_segment("function");
			return tt_err();  // no error = failure
		}
		catch (WizParseError e) {
			if    (e.error_code == WIZERR_REDIM)  return tt_ok();   // redim error = success
			else  return tt_err();  // incorrect error
		}
	}

	int tt_undimmed() {
		tt_start("tt_undimmed");
		inp.load({
			"let a = 1",
			"",
		});
		try {
			// meth.ps_block();
			meth.ps_let();
			return tt_err();  // no error = failure
		}
		catch (WizParseError e) {
			return tt_ok();   // error = success
		}
	}
};