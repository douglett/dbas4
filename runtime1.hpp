#pragma once
#include <map>
#include "helpers.hpp"
#include "outputb.hpp"


enum RUN_ERROR {
	RERR_ERROR = 1,
	RERR_TYPE,
};


struct Runtime1 {
	OutputB& outp;
	
	string ctrl;
	map<string, i32>     dims;
	vector<i32>          stack;
	vector<vector<i32>>  heap;


	i32  pop()       { i32 r = stack.at(stack.size()-1);  stack.pop_back();  return r; }
	void push(i32 v) { stack.push_back(v); }


	void error(RUN_ERROR err = RERR_ERROR) {
		WizError e;
			e.msg = "runtime error: " + to_string(err);
			e.buildmsg();
		throw e;
	}


	void init() {
		heap = { {} };  // assign heap 0 (null)
		// init global dims here
	}

	string heap_to_string(i32 heap_index) {
		string s;
		auto& mem = heap.at(heap_index);        // get heap
		for (int i = 0; i < mem.size() && mem.at(i) != 0; i++)
			s.push_back(mem.at(i));
		return s;
	}

	void r_func(const string& fname) {
		int i = 0;
		for (i = 0; i < outp.functions.size(); i++)
			if (outp.functions[i].id == fname)  break;
		const auto& fn = outp.functions.at(i);

		for (auto d : fn.dims)  r_dim(d);
		r_block(fn.block);
		if    (ctrl == "return")  ctrl = "";  // return stack top
		else  push(0);  // default return
	}
	
	void r_dim(i32 idx) {
		const auto& dim = outp.dims.at(idx);
		printf("dim :: %s %s\n", dim.type.c_str(), dim.id.c_str());

		if (dim.type == "int") {
			if    (dim.expression == -1)  dims[dim.id] = 0;
			else  r_expression(dim.expression),  dims[dim.id] = pop();
		}
		else if (dim.type == "string") {
			heap.push_back({});
			dims[dim.id] = heap.size()-1;
		}
		else  error(RERR_TYPE);
	}

	void r_block(i32 idx) {
		// printf("here123\n");
		const auto& block = outp.blocks.at(idx);
		for (auto& stmt : block.stmts)
			// 
			if (ctrl == "return")  break;
			
			else if (stmt.type == "print") {
				const auto& print = outp.prints.at(stmt.id);
				for (auto& arg : print.list)
					if      (arg.type == "lit" )     printf("%s", outp.literals.at(arg.id).c_str());
					else if (arg.type == "expr")     r_expression(arg.id),   printf("%d", pop());
					else if (arg.type == "varpath")  r_varpath_get(arg.id),  printf("%s", heap_to_string(pop()).c_str());
					else    error();
				printf("\n");
			}

			else if (stmt.type == "input") {
				const auto& inp = outp.inputs.at(stmt.id);
				cout << outp.literals.at(inp.prompt);   // show command prompt
				string input;
				getline(cin, input);                    // read from std-in
				r_varpath_get(inp.varpath);             // calculate heap index
				i32 heap_index = pop();
				auto& mem = heap.at(heap_index);        // get heap
				mem.resize(input.size());               // resize
				for (int i=0; i<input.size(); i++)      // copy string
					mem[i] = input[i];
			}

			else if (stmt.type == "if") {
				const auto& ifx = outp.ifs.at(stmt.id);
				for (auto& ifthen : ifx.ifthens)
					if  (r_expression(ifthen.cond), pop())  { r_block(ifthen.block);  break; }
			}

			else if (stmt.type == "while") {
				const auto& whilex = outp.whiles.at(stmt.id);
				while (r_expression(whilex.cond), pop())
					r_block(whilex.block);
			}
			
			else if (stmt.type == "return") {
				const auto& ret = outp.returns.at(stmt.id);
				if (ret.expression > -1)  r_expression(ret.expression);
				ctrl = "return";
			}

			else if (stmt.type == "call") {
				const auto& call = outp.calls.at(stmt.id);
				if (call.args.size() > 0) error();
				r_func(call.id);
			}

			// else if (stmt.type == "set") {

			else if (stmt.type == "let") {
				const auto& let = outp.lets.at(stmt.id);
				r_expression(let.expression);
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

	void r_expression(i32 idx) {
		const auto& expr = outp.expressions.at(idx);
		i32 v = 0;
		for (auto& li : expr.list) {
			// printf("  [%s]  \n", li.c_str());
			const auto inst = Helpers::splitws(li);
			if      (inst.at(0) == "varpath")  r_varpath_get( stoi(inst.at(1).c_str()) );
			else if (inst.at(0) == "i")        push( stoi(inst.at(1).c_str()) );
			else if (inst.at(0) == "add")      push( pop() + pop() );
			else if (inst.at(0) == "sub")      v = pop(),  push( pop() - v );
			else if (inst.at(0) == "mul")      push( pop() * pop() );
			else if (inst.at(0) == "div")      v = pop(),  push( pop() / v );
			else    error();
		}
	}
};