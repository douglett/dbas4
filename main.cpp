#include <iostream>
#include <string>
#include <vector>
#include <cassert>

#include "helpers.hpp"
#include "input.hpp"
#include "outputb.hpp"
#include "method2.hpp"
#include "runtime1.hpp"
#include "test_method2.hpp"
#include "test_outputb.hpp"
#include "test_runtime1.hpp"

using namespace std;


// global state
Input    inp;
OutputB  outp;
Method2  meth = { inp, outp };


int dotests() {
	// run all tests
	TestMethod2 tm2;
	tm2.runall();
	TestOutputB tob;
	tob.runall();
	TestRuntime1 tr1;
	tr1.runall();
	return 0;
}


int main() {
	printf("hello world\n");
	// return dotests();


	// test adventure code
	inp.open("adventure1.bas");
	meth.ps_program();
	outp.show();
	Runtime1 run1 = { outp };
	run1.init();
	// run1.r_func("main");
}