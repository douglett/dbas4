// ----------------------------------------
// Tetst for Parsing Output B
// ----------------------------------------
#pragma once

int tt_redim() {
	tt_start("tt_redim");
	outp.clear();
	inp.load({
		"dim a",
		"dim a",
	});
	try {
		meth.ps_segment("dim");
		tt_err();  // no error = failure
		return 1;
	}
	catch (ParseError e) {
		tt_ok();   // error = success
		return 0;
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


void testsob_runall() {
	printf("-----\n");
	printf("> running OutputB tests...\n");
	
	int errc = 0;
	
	errc += tt_redim();
	errc += tt_undimmed();

	if   (errc == 0)  printf("> %sAll tests passed.%s\n", TERM_GREEN, TERM_DEF);
	else              printf("> %sFound %d errors.%s\n", TERM_RED, errc, TERM_DEF);
	printf("-----\n");
}