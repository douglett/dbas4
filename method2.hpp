#pragma once

Input::Results r1, r2;

void ps_struct();
void ps_dim();
void ps_function();
void ps_block();

void ps_string_literal() {
	inp.expect("@string_literal", r1);
	outp.string_literal(r1.at(0));
}

void ps_varpath() {
	inp.expect("@identifier", r1);
	outp.varpath(r1.at(0));
}

void ps_expression() {
	// TODO
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
	if      (inp.get("@identifier @identifier '[ '] endl", r1))  outp.dim_short(r1.at(0), r1.at(1), true);
	else if (inp.get("@identifier @identifier endl", r1))  outp.dim_short(r1.at(0), r1.at(1), false);
	else if (inp.get("@identifier '[ '] endl", r1))  outp.dim_short("int", r1.at(0), true);
	else    inp.expect("@identifier endl", r1),  outp.dim_short("int", r1.at(0), false);
}

void ps_struct() {
	inp.expect("'struct @identifier endl", r1);
	outp.struct_start(r1.at(0));
	while (true)
		if      (inp.get("endl"))  ; // ingnore empty lines
		else if (inp.peek("'end"))  break;
		else if (inp.peek("identifier"))  ps_dim_short();
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

void ps_print() {
	inp.expect("'print");
	outp.print_start();
	bool first = true;
	while (true) {
		if (inp.peek("endl"))  break;
		if (!first)  inp.expect("',");
		first = false;
		if      (inp.peek("'\""))  ps_string_literal();
		else if (inp.peek("identifier"))  ps_varpath();
		else    inp.expect("identifier");
	}  
	inp.expect("endl");
	outp.print_end();
}

void ps_if() {
	inp.expect("'if");
	outp.if_start();
	ps_expression();
	inp.expect("endl");
	ps_block();
	inp.expect("'end 'if endl");

}

void ps_block() {
	while (true)
		if      (inp.get("endl"))  ;
		else if (inp.peek("'print"))   ps_print();
		// else if (inp.peek("'prints"))  ;
		// else if (inp.peek("'input"))  ;
		else if (inp.peek("'if"))      ps_if();
		// else if (inp.peek("'while"))   ps_while();
		// else if (inp.peek("'return"))  ps_return();
		// else if (inp.peek("@identifier"))  ;
		// else if (inp.peek("'call"))  ;
		// else if (inp.peek("'let'"))  ;
		// else if (inp.peek("'set'"))  ;
		else    break;
	inp.expect("'end");  // unexpected in block
}