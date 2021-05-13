#include <iostream>
#include <string>
#include <vector>
#include <cassert>

#include "helpers.hpp"
#include "input.hpp"
#include "outputb.hpp"
#include "method2.hpp"
#include "testsm2.hpp"
#include "runtime1.hpp"

using namespace std;


// global state
Input    inp;
OutputB  outp;
Method2  meth = { inp, outp };



int main() {
	printf("hello world\n");
	
	// tt_struct();
	// tt_struct2();
	// tt_globals();
	// tt_function();
	// tt_function2();
	// tt_function3();
	// tt_expression();
	// tt_if();
	// tt_if2();
	// tt_variables();
	// tt_block_large();
	// outp.show();

	ttr_function();
}