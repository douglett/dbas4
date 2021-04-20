#pragma once

Input::Results r1, r2;

void ps_struct();
void ps_dim();
void ps_function();
void ps_block();
void ps_expression();

void ps_string_literal() {
	inp.expect("@string_literal", r1);
	outp.string_literal(r1.at(0));
}

void ps_varpath() {
	inp.expect("@identifier", r1);
	outp.varpath(r1.at(0));
	while (true)
		if      (inp.get("'. @identifier", r1))  outp.varpath("."+r1.at(0));
		else if (inp.get("'[")) {
			outp.varpath("[");
			ps_expression();
			inp.expect("']");
			outp.varpath("]");
		}
		else    break;
}

void ps_expression() {
	// TODO
	if    (inp.get("@integer", r1))  outp.varpath(r1.at(0));
	else  ps_varpath();
}

void ps_segment(const string& type) {
	while (true)
		if      (inp.get("endl"))  ; // ingnore empty lines
		else if (type == "dim" && inp.peek("'dim"))        ps_dim();
		else if (type == "struct" && inp.peek("'struct"))  ps_struct();
		else    break;
}

void ps_dim() {
	if    (inp.get("'dim @identifier @identifier", r1))  outp.dim_start(r1.at(0), r1.at(1));
	else  inp.expect("'dim @identifier", r1), outp.dim_start("int", r1.at(0));
	if    (inp.get("'[ ']")) outp.dim_isarray(true);
	if    (inp.get("'="))  ps_expression();
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
	ps_block();
	inp.expect("'end 'function endall");
	outp.func_end();
}

void ps_printargs() {
	// while (true) {
	// 	if      (inp.peek("'\""))         ps_string_literal();
	// 	else if (inp.peek("identifier"))  ps_varpath();
	// 	else    inp.expect("identifier");
	// 	if      (inp.peek("endl"))  break;
	// 	else    inp.expect("',");
	// }
	bool first = true;
	while (true) {
		if (inp.peek("endl"))  break;
		if (!first)  inp.expect("',");
		if      (inp.peek("'\""))         ps_string_literal();
		else if (inp.peek("identifier"))  ps_varpath();
		else    inp.expect("identifier");
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
	ps_expression();
	inp.expect("endl");
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
		else    inp.expect("'end")     ;
		// else    break;
}