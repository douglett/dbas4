#pragma once

void pse_atom() {
	if    (inp.get("@integer", r1))  outp.ex_push(r1.at(0));
	else  ps_varpath();
}

void pse_mul() {
	pse_atom();
	if      (inp.get("'*"))  pse_mul(),  outp.ex_push("*");
	else if (inp.get("'/"))  pse_mul(),  outp.ex_push("/");
}
void pse_add() {
	pse_mul();
	if      (inp.get("'+"))  pse_add(),  outp.ex_push("+");
	else if (inp.get("'-"))  pse_add(),  outp.ex_push("-");
}
void pse_compare() {
	pse_add();
	if (	inp.get("'==", r1) || inp.get("'!=", r1) 
			|| inp.get("'>=", r1) || inp.get("'<=", r1) 
			|| inp.get("'<", r1) || inp.get("'>", r1)
		) {
		string op = r1.at(0);
		pse_compare();
		outp.ex_push(op);
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