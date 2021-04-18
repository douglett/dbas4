#pragma once

// Parse State
struct PState {
	enum PSTATE_T {
		PS_NONE = 0,
		PS_STRUCT_BLOCK,
		PS_GLOBAL_BLOCK,
		PS_FUNCTION_BLOCK,
		PS_STRUCT,
		PS_DIM,
		PS_DIM_SHORT,
		PS_FUNCTION,
		PS_FUNCTION_ARGS,
		PS_FUNCTION_END,
		PS_BLOCK,
		PS_STMT_PRINT,
		PS_STMT_PRINT_ARGS,
		PS_STMT_IF,
		PS_STMT_IF2,
		PS_STMT_IF_END,
		PS_STMT_WHILE,
		PS_STMT_WHILE2,
		PS_STMT_WHILE_END,
		PS_STMT_RETURN,
		PS_STMT_RETURN_END,
		PS_STRING_LITERAL,
		PS_EXPRESSION,
		PS_VARPATH,
	};
	vector<PSTATE_T> _stack;
	PSTATE_T current() { return _stack.size() ? _stack.back() : PS_NONE; }
	void push(PSTATE_T state) { _stack.push_back(state); }
	void pushall(const vector<PSTATE_T>& states) { for (auto st : states) _stack.push_back(st); }
	void pop() { if (_stack.size()) _stack.pop_back(); };
};


PState state;



// void dim_short(Input::Results& r1) {
// 	if      (inp.get("@identifier @identifier '[ ']", r1))  outp.func_arg(r1.at(0), r1.at(1), true);
// 	else if (inp.get("@identifier @identifier", r1))  outp.func_arg(r1.at(0), r1.at(1), false);
// 	else if (inp.get("@identifier '[ ']", r1))  outp.func_arg("int", r1.at(0), true);
// 	else 	inp.expect("@identifier", r1),  outp.func_arg("int", r1.at(0), false);
// }


// void expression() {
// 	printf("	expression:\n");
// }


void ploop() {
	int running = 1;
	Input::Results r1, r2;

	while (running)
	switch (state.current()) {

	case PState::PS_NONE:
		printf("no state.\n");
		running = 0;
		break;

	case PState::PS_STRUCT_BLOCK:
	case PState::PS_GLOBAL_BLOCK:
	case PState::PS_FUNCTION_BLOCK:
		if      (inp.get("endl"))  ;
		else if (state.current() == PState::PS_STRUCT_BLOCK && inp.peek("'struct"))  state.push(PState::PS_STRUCT);
		else if (state.current() == PState::PS_GLOBAL_BLOCK && inp.peek("'dim"))  state.push(PState::PS_DIM);
		else if (state.current() == PState::PS_FUNCTION_BLOCK && inp.peek("'function"))  state.push(PState::PS_FUNCTION);
		else    state.pop();
		break;

	case PState::PS_STRUCT:
		inp.expect("'struct @identifier endl", r1);
		outp.struct_start(r1.at(0));
		while (true)
			if      (inp.peek("'end"))  break;
			else if (inp.get("endl"))  ; // ingnore empty lines
			// else if (inp.peek("identifier"))  state.push(PState::PS_DIM_SHORT);
			else if (inp.get("@identifier @identifier '[ '] endl", r2))  outp.struct_addmember(r2.at(0), r2.at(1), true);
			else if (inp.get("@identifier @identifier endl", r2))  outp.struct_addmember(r2.at(0), r2.at(1), false);
			else if (inp.get("@identifier '[ '] endl", r2))  outp.struct_addmember("int", r2.at(0), true);
			else if (inp.get("@identifier endl", r2))  outp.struct_addmember("int", r2.at(0), false);
			else    break;
		inp.expect("'end 'struct endall");
		outp.struct_end();
		state.pop();
		break;
	
	case PState::PS_DIM:
		if   (inp.get("'dim @identifier @identifier", r1))  outp.dim_start(r1.at(0), r1.at(1));
		else inp.expect("'dim @identifier", r1), outp.dim_start("int", r1.at(0));
		if (inp.get("'[ ']")) outp.dim_isarray(true);
		if (inp.get("'="))  ; // expr
		inp.expect("endall");
		outp.dim_end();
		state.pop();
		break;

	case PState::PS_DIM_SHORT:
		if      (inp.get("@identifier @identifier '[ ']", r1))  outp.dim_short(r1.at(0), r1.at(1), true);
		else if (inp.get("@identifier @identifier", r1))  outp.dim_short(r1.at(0), r1.at(1), false);
		else if (inp.get("@identifier '[ ']", r1))  outp.dim_short("int", r1.at(0), true);
		else 	inp.expect("@identifier", r1),  outp.dim_short("int", r1.at(0), false);
		state.pop();
		break;

	// case PState::PS_FUNCTION:
	// 	inp.expect("'function @identifier '(", r1);
	// 	outp.func_start(r1.at(0));
	// 	while (true) {
	// 		if      (inp.get("@identifier @identifier '[ ']", r2))  outp.func_arg(r2.at(0), r2.at(1), true);
	// 		else if (inp.get("@identifier @identifier", r2))  outp.func_arg(r2.at(0), r2.at(1), false);
	// 		else if (inp.get("@identifier '[ ']", r2))  outp.func_arg("int", r2.at(0), true);
	// 		else if (inp.get("@identifier", r2))  outp.func_arg("int", r2.at(0), false);
	// 		else    break;
	// 		if (!inp.get("',")) break;
	// 	}
	// 	inp.expect("') endl");
	// 	state.pop();
	// 	state.push(PState::PS_FUNCTION_END);
	// 	state.push(PState::PS_BLOCK);
	// 	break;
	
	case PState::PS_FUNCTION:
		inp.expect("'function @identifier '(", r1);
		outp.func_start(r1.at(0));
		state.pop();
		state.push(PState::PS_FUNCTION_END); // end-function keywords
		if    (inp.peek("@identifier"))  state.pushall({ PState::PS_FUNCTION_ARGS, PState::PS_DIM_SHORT }); // handle first argument, then list
		else  inp.expect("') endl"); // empty list
		break;
	case PState::PS_FUNCTION_ARGS:
		if      (inp.get("',"))  state.push(PState::PS_DIM_SHORT);
		else    inp.expect("') endl"),  state.pop();
		break;
	case PState::PS_FUNCTION_END:
		inp.expect("'end 'function endall");
		outp.func_end();
		state.pop();
		break;

	case PState::PS_BLOCK:
		if      (inp.get("endl"))  ;
		else if (inp.peek("'print"))  state.push(PState::PS_STMT_PRINT);
		// else if (inp.peek("'prints"))  state.push(PState::PS_STMT_PRINTS);
		// else if (inp.peek("'input"))  ;
		else if (inp.peek("'if"))  state.push(PState::PS_STMT_IF);
		else if (inp.peek("'while"))  state.push(PState::PS_STMT_WHILE);
		else if (inp.peek("'return"))  state.push(PState::PS_STMT_RETURN);
		// else if (inp.peek("@identifier"))  ;
		// else if (inp.peek("'call"))  ;
		// else if (inp.peek("'let'"))  ;
		// else if (inp.peek("'set'"))  ;
		// else    state.pop();
		else if (inp.peek("'end"))  state.pop();
		else    inp.expect("'end");  // unexpected in block
		break;

	case PState:: PS_STMT_PRINT:
		inp.expect("'print");
		outp.print_start();
		state.pop();
		if      (inp.peek("'\""))  state.pushall({ PState::PS_STMT_PRINT_ARGS, PState::PS_STRING_LITERAL });
		else if (inp.peek("identifier"))  state.pushall({ PState::PS_STMT_PRINT_ARGS, PState::PS_VARPATH });
		else    inp.expect("endl"),  outp.print_end();
		break;
	case PState::PS_STMT_PRINT_ARGS:
		if      (inp.peek("', '\""))  inp.get("',"),  state.push(PState::PS_STRING_LITERAL);
		else if (inp.peek("', identifier"))  inp.get("',"),  state.push(PState::PS_VARPATH);
		else    inp.expect("endl"),  state.pop();
		break;

	case PState::PS_STMT_IF:
		inp.expect("'if");
		outp.if_start();
		// expression();
		state.pop();
		state.pushall({ PState::PS_STMT_IF2, PState::PS_EXPRESSION });
		break;
	case PState::PS_STMT_IF2:
		// inp.expect("'then endl");
		inp.expect("endl");
		state.pop();
		state.pushall({ PState::PS_STMT_IF_END, PState::PS_BLOCK });
		break;
	case PState::PS_STMT_IF_END:
		inp.expect("'end 'if endl");
		outp.if_end();
		state.pop();
		break;
	
	case PState::PS_STMT_WHILE:
		inp.expect("'while");
		outp.while_start();
		state.pop();
		state.pushall({ PState::PS_STMT_WHILE2, PState::PS_EXPRESSION });
		break;
	case PState::PS_STMT_WHILE2:
		// inp.expect("'then endl");
		inp.expect("endl");
		state.pop();
		state.pushall({ PState::PS_STMT_WHILE_END, PState::PS_BLOCK });
		break;
	case PState::PS_STMT_WHILE_END:
		inp.expect("'end 'while endl");
		outp.while_end();
		state.pop();
		break;

	case PState::PS_STMT_RETURN:
		inp.expect("'return");
		outp.return_start();
		state.pop();
		if    (inp.get("endl"))  outp.return_end();
		else  state.pushall({ PState::PS_STMT_RETURN_END, PState::PS_EXPRESSION });
		break;
	case PState::PS_STMT_RETURN_END:
		inp.expect("endl");
		outp.return_end();
		state.pop();
		break;

	case PState::PS_STRING_LITERAL:
		inp.peek("'\"") || inp.expect("'\"");
		inp.expect("@string_literal", r1);
		outp.string_literal(r1.at(0));
		state.pop();
		break;

	case PState::PS_EXPRESSION:
	case PState::PS_VARPATH:
		inp.expect("@identifier", r1);
		outp.varpath(r1.at(0));
		state.pop();
		break;

	} // end switch
}


// void stmt_if() {
// 	inp.expect("'if");
// 	outp.if_start();
// 	expression();
// 	inp.expect("endl");
// 	stmt_block();
// 	inp.expect("'end 'if endl");
// 	outp.if_end();
// }