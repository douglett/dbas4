#include <iostream>
#include <string>
#include <vector>
#include <cassert>

#include "helpers.hpp"
#include "input.hpp"
#include "outputb.hpp"
#include "method2.hpp"
#include "runtime1.hpp"
#include "testsm2.hpp"
#include "testsob.hpp"
#include "testsr1.hpp"

using namespace std;


// global state
Input    inp;
OutputB  outp;
Method2  meth = { inp, outp };



int main() {
	printf("hello world\n");
	// run all tests
	// testsm2_runall();
	// testsob_runall();
	// outp.show();

	ttr_function();
	// ttr_text();
}