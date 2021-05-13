#pragma once

// extern Input   inp;
// extern OutputB outp;
// Input::Results r1, r2;


// void ps_segment(const string& type);
// void ps_struct();
// void ps_dim();
// void ps_function();
// void ps_block();
// void ps_string_literal();
// void ps_varpath();
// void ps_expression();



struct Method2 {
	Input&    inp;
	OutputB&  outp;
	Input::Results r1;


// ----------------------------------------
// Basic structure parsing
// ----------------------------------------


	void ps_program() {
		ps_segment("dim");
		ps_segment("struct");
		ps_segment("function");
		inp.expect("eof");
	}

	void ps_segment(const string& type) {
		while (true)
			if      (inp.get("endl"))  ;                           // ingnore empty lines
			else if (type == "dim" && inp.peek("'dim"))            ps_dim();
			else if (type == "struct" && inp.peek("'struct"))      ps_struct();
			else if (type == "function" && inp.peek("'function"))  ps_function();
			else    break;
	}

	void ps_dim() {
		bool isarray = false;
		if    (inp.get("'dim @identifier @identifier", r1))  outp.dim_start(r1.at(0),  r1.at(1));
		else  inp.expect("'dim @identifier", r1),  outp.dim_start("int", r1.at(0));
		if    (inp.get("'[ ']"))  isarray = true,  outp.dim_isarray(true);
		if    (inp.get("'="))  isarray ? ps_varpath() : ps_expression();
		inp.expect("endall");
		outp.dim_end();
	}

	void ps_dim_short() {
		if      (inp.get("@identifier @identifier '[ ']", r1))  outp.dim_short(r1.at(0), r1.at(1), true);
		else if (inp.get("@identifier @identifier", r1))  outp.dim_short(r1.at(0), r1.at(1), false);
		else if (inp.get("@identifier '[ ']", r1))  outp.dim_short("int", r1.at(0), true);
		else    inp.expect("@identifier", r1),  outp.dim_short("int", r1.at(0), false);
	}

	void ps_struct() {
		inp.expect("'struct @identifier endl", r1);
		outp.struct_start(r1.at(0));
		while (true)
			if      (inp.get("endl"))  ; // ingnore empty lines
			else if (inp.peek("'end"))  break;
			else if (inp.peek("identifier"))  ps_dim_short(),  inp.expect("endl");
			else    break;
		inp.expect("'end 'struct endall");
		outp.struct_end();
	}

	void ps_function() {
		inp.expect("'function @identifier '(", r1);
		outp.func_start(r1.at(0));
		bool first = true;
		while (true)
			if      (first && inp.peek("identifier"))  first=false,  ps_dim_short();
			else if (inp.peek("', identifier"))  inp.get("',"),  ps_dim_short();
			else    break;
		inp.expect("') endl");
		ps_segment("dim");
		ps_block();
		inp.expect("'end 'function endall");
		outp.func_end();
	}

	void ps_printargs() {
		bool first = true;
		while (true) {
			if (inp.peek("endl"))  break;
			if (!first)  inp.expect("',");
			if      (inp.peek("'\""))         ps_string_literal();
			else    ps_expression();
			first = false;
		}
	}

	void ps_print() {
		inp.expect("'print");
		outp.print_start();
		ps_printargs();
		inp.expect("endl");
		outp.print_end();
	}

	void ps_prints() {
		inp.expect("'prints");
		outp.print_start();
		ps_varpath();
		inp.expect("',");
		ps_printargs();
		inp.expect("endl");
		outp.print_end();
	}

	void ps_input() {
		inp.expect("'input");
		outp.input_start();
		if (inp.peek("string_literal"))  ps_string_literal(),  inp.expect("',");
		ps_varpath();
		inp.expect("endl");
		outp.input_end();
	}

	void ps_if() {
		inp.expect("'if");
		outp.if_start();
		// outp.if_cond();
		ps_expression();
		inp.expect("endl");
		// outp.if_cond_end();
		ps_block();
		inp.expect("'end 'if endl");
		outp.if_end();
	}

	void ps_while() {
		inp.expect("'while");
		outp.while_start();
		ps_expression();
		inp.expect("endl");
		ps_block();
		inp.expect("'end 'while endl");
		outp.while_end();
	}

	void ps_return() {
		inp.expect("'return");
		outp.return_start();
		if (!inp.peek("endl"))  ps_expression();
		inp.expect("endl");
		outp.return_end();
	}

	void ps_call() {
		inp.expect("'call @identifier '(", r1);
		outp.call_start(r1.at(0));
		bool first = true;
		while (true) {
			if (inp.peek("')"))  break;
			if (!first)  inp.expect("',");
			ps_expression();
			first = false;
		}
		inp.expect("') endl");
		outp.call_end();
	}

	void ps_set() {
		inp.expect("'set");
		outp.set_start();
		ps_varpath();
		inp.expect("'=");
		ps_varpath();
		inp.expect("endl");
		outp.set_end();
	}

	void ps_let() {
		inp.expect("'let");
		outp.let_start();
		ps_varpath();
		inp.expect("'=");
		ps_expression();
		inp.expect("endl");
		outp.let_end();
	}

	void ps_block() {
		outp.block_start();
		while (true)
			if      (inp.get ("endl"))     ;
			else if (inp.peek("'end"))     break;
			else if (inp.peek("'print"))   ps_print();
			else if (inp.peek("'prints"))  ps_prints();
			else if (inp.peek("'input"))   ps_input();
			else if (inp.peek("'if"))      ps_if();
			else if (inp.peek("'while"))   ps_while();
			else if (inp.peek("'return"))  ps_return();
			// else if (inp.peek("@identifier"))  ;
			else if (inp.peek("'call"))    ps_call();
			else if (inp.peek("'set"))     ps_set();
			else if (inp.peek("'let"))     ps_let();
			else    inp.expect("'end")     ;  // force error
			// else    break;
		outp.block_end();
	}

	void ps_string_literal() {
		inp.expect("@string_literal", r1);
		outp.string_literal(r1.at(0));
	}

	void ps_varpath() {
		inp.expect("@identifier", r1);
		outp.varpath_start(r1.at(0));
		while (true)
			if      (inp.get("'. @identifier", r1))  outp.varpath_push("."+r1.at(0));
			else if (inp.get("'["))  ps_expression(),  inp.expect("']"),  outp.varpath_push("getmem $2 $1");
			else    break;
		outp.varpath_end();
	}


// ----------------------------------------
// Expression parsing 
// ----------------------------------------


	void pse_atom() {
		if    (inp.get("@integer", r1))  outp.ex_push("i "+r1.at(0));
		else  ps_varpath();
	}

	void pse_mul() {
		pse_atom();
		if      (inp.get("'*"))  pse_mul(),  outp.ex_push("mul");
		else if (inp.get("'/"))  pse_mul(),  outp.ex_push("div");
	}
	void pse_add() {
		pse_mul();
		if      (inp.get("'+"))  pse_add(),  outp.ex_push("add");
		else if (inp.get("'-"))  pse_add(),  outp.ex_push("sub");
	}
	void pse_compare() {
		pse_add();
		// printf("here1\n");
		if (	inp.get("@'==", r1) || inp.get("@'!=", r1) 
				|| inp.get("@'>=", r1) || inp.get("@'<=", r1) 
				|| inp.get("@'<", r1) || inp.get("@'>", r1)
			) {
			// printf("here2 [%s]\n", r1.at(0).c_str());
			string op = r1.at(0);
			pse_compare();
			outp.ex_push("cmp "+op);
			// printf("here3\n");
		}
	}

	// TODO: 
	//		and/or
	// 		brackets

	void ps_expression() {
		outp.ex_start();
		pse_compare();
		outp.ex_end();
	}


};  // end Method2