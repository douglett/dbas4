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



int main() {
	printf("hello world\n");
	// run all tests
	TestMethod2 tm2;
	tm2.runall();
	TestOutputB tob;
	tob.runall();
	TestRuntime1 tr1;
	tr1.runall();
}