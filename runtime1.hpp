#pragma once
#include <map>


struct Runtime1 {
	map<string, i32>  dims;
	vector<i32>       stack;


	i32  pop()       { i32 r = stack.at(stack.size()-1);  stack.pop_back();  return r; }
	void push(i32 v) { stack.push_back(v); }


	void error(int err=1) {
		fprintf(stderr, "runtime error: %d\n", err);
		exit(1);
	}


	void r_func(const string& fname) {
		int i = 0;
		for (i = 0; i < outp.functions.size(); i++)
			if (outp.functions[i].id == fname)  break;
		const auto& fn = outp.functions.at(i);

		for (auto d : fn.dims)  r_dim(d);
		r_block(fn.block);
	}
	
	void r_dim(i32 idx) {
		const auto& dim = outp.dims.at(idx);
		if    (dim.expression == -1)  dims[dim.id] = 0;
		else  r_expr(dim.expression),  dims[dim.id] = pop();
	}

	void r_block(i32 idx) {
		// printf("here123\n");
		const auto& block = outp.blocks.at(idx);
		for (auto& stmt : block.stmts)
			// 
			if (stmt.type == "print") {
				const auto& print = outp.prints.at(stmt.id);
				for (auto& arg : print.list)
					if      (arg.type == "lit" )  printf("%s", outp.literals.at(arg.id).c_str());
					else if (arg.type == "expr")  r_expr(arg.id),  printf("%d", pop());
					else    error();
				printf("\n");
			}

			else if (stmt.type == "let") {
				const auto& let = outp.lets.at(stmt.id);
				r_expr(let.expression);
				r_varpath_set(let.varpath);
			}
			
			else  error();
	}

	void r_varpath_get(i32 idx) {
		const auto& vp = outp.varpaths.at(idx);
		push( dims.at(vp.list.at(0)) );
	}

	void r_varpath_set(i32 idx) {
		const auto& vp = outp.varpaths.at(idx);
		dims.at( vp.list.at(0) ) = pop();
	}

	void r_expr(i32 idx) {
		const auto& expr = outp.expressions.at(idx);
		i32 v = 0;
		for (auto& li : expr.list) {
			// printf("  [%s]  \n", li.c_str());
			const auto inst = Helpers::splitws(li);
			if      (inst.at(0) == "varpath")  r_varpath_get( stoi(inst.at(1).c_str()) );
			else if (inst.at(0) == "i")    push( stoi(inst.at(1).c_str()) );
			else if (inst.at(0) == "add")  push( pop() + pop() );
			else if (inst.at(0) == "sub")  v = pop(),  push( pop() - v );
			else if (inst.at(0) == "mul")  push( pop() * pop() );
			else if (inst.at(0) == "div")  v = pop(),  push( pop() / v );
			else    error();
		}
	}
};


Runtime1 run;


void ttr_function() {
	inp.load({
		"function foo()",
		"	dim a = 101",
		// "	let a = 11*2+3",
		"	print \"result: \", a",
		"end function",
		"",
	});
	ps_function();
	outp.show();
	printf("-----\n");
	printf("<running>\n");
	run.r_func("foo");
}