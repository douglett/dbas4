// ----------------------------------------
// Tetst for Parsing Output B
// ----------------------------------------
#pragma once

void tt_redim() {
	tt_start("tt_redim");
	outp.clear();
	inp.load({
		"dim a",
		"dim a",
	});
	try {
		meth.ps_segment("dim");
		tt_err();  // no error = failure
	}
	catch (ParseError e) {
		tt_ok();   // error = success
	}
}

void testsob_runall() {
	printf("-----\n");
	printf("> running OutputB tests...\n");
	
	tt_redim();

	printf("> tests OK.\n");
	printf("-----\n");
}