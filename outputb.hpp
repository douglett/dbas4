#pragma once


// ----------------------------------------
// Output method B (structure building)
// ----------------------------------------


enum PSTATE_T {
	PS_NONE = 0,
	PS_EXPRESSION,
	PS_VARPATH,
	PS_STRUCT,
	PS_DIM,
	PS_FUNCTION,
	PS_STMT_BLOCK,
	PS_STMT_PRINT,
	PS_STMT_INPUT,
	PS_STMT_IF,
	PS_STMT_WHILE,
	PS_STMT_RETURN,
	PS_STMT_CALL,
	PS_STMT_SET,
	PS_STMT_LET,
};


struct wb_dim {
	string type, id;
	bool isarray;
};
struct wb_dimshort {
	string type, id;
	bool isarray;
};
struct wb_struct {
	string id;
	vector<wb_dimshort> members;
};
struct wb_function {
	string id;
	vector<wb_dimshort> args;
};
struct wb_stmt_block {
	vector<string> stmts;
};
struct wb_stmt_print {
	vector<string> list;
};
struct wb_stmt_input {
	string prompt;
	int varpath;
};
struct wb_stmt_if {
	vector<int> conds;
	vector<int> blocks;
};
struct wb_stmt_while {
	int cond, block;
};
struct wb_stmt_return {
	int expression;
};
struct wb_stmt_call {
	string id;
	vector<int> args;
};
struct wb_stmt_set {
	int varpath1, varpath2;
};
struct wb_stmt_let {
	int varpath, expression;
};
struct wb_varpath {
	vector<string> list;
};
struct wb_expression {
	vector<string> list;
};



struct OutputB : Output {
	struct pstate {
		PSTATE_T pstate;
		int id;
	};
	vector<pstate>           state;

	vector<wb_struct>        structs;
	vector<wb_dim>           dims;
	vector<wb_function>      funcs;
	vector<wb_stmt_print>    prints;
	vector<wb_stmt_input>    inputs;
	vector<wb_stmt_if>       ifs;
	vector<wb_stmt_while>    whiles;
	vector<wb_stmt_return>   returns;
	vector<wb_stmt_call>     calls;
	vector<wb_stmt_set>      sets;
	vector<wb_stmt_let>      lets;
	vector<wb_stmt_block>    blocks;
	vector<wb_varpath>       varpaths;
	// vector<wb_expression>    expressions = { {{"i 0"}}, {{"i 1"}} };
	vector<wb_expression>    expressions;


	PSTATE_T curstate() { return state.size() ? state.back().pstate : PS_NONE; }
	int      curid()    { return state.size() ? state.back().id     : -1; }


	void struct_start(const string& id) {
		structs.push_back({ id });
		state.push_back({ PS_STRUCT, int(structs.size()-1) });
	}
	void struct_end() {
		state.pop_back();
	}
	void dim_short(const string& type, const string& id, bool isarray) {
		if      (curstate() == PS_STRUCT)    structs.at(curid()).members.push_back({ type, id, isarray });
		else if (curstate() == PS_FUNCTION)  funcs.at(curid()).args.push_back({ type, id, isarray });
		else    Output::dim_short(type, id, isarray);
	}
	void dim_start(const string& type, const string& id) {
		dims.push_back({ type, id });
		state.push_back({ PS_DIM, int(dims.size()-1) });
	}
	void dim_isarray(bool val) {
		dims.at(curid()).isarray = val;
	}
	void dim_end() {
		state.pop_back();
	}
	void func_start(const string& id) {
		funcs.push_back({ id });
		state.push_back({ PS_FUNCTION, int(funcs.size()-1) });
	}
	void func_end() {
		state.pop_back();
	}


	void block_start() {
		blocks.push_back({});
		int idx = blocks.size() - 1;
		if      (curstate() == PS_STMT_IF)     ifs.at(curid()).blocks.push_back(idx);
		else if (curstate() == PS_STMT_WHILE)  whiles.at(curid()).block = idx;
		else    printf("%d  ", curstate()),  Output::block_start();
		state.push_back({ PS_STMT_BLOCK, idx });
	}
	void _block_append_stmt(const string& type, int id) {
		if    (curstate() == PS_STMT_BLOCK)  blocks.at(curid()).stmts.push_back(type + " " + to_string(id));
		else  printf("  %d  (%s $%d to block)\n", curstate(), type.c_str(), id);
	}
	void block_end() {
		state.pop_back();
	}	
	void print_start() {
		prints.push_back({});
		state.push_back({ PS_STMT_PRINT, int(prints.size()-1) });
	}
	void print_end() {
		state.pop_back();
	}
	void input_start() {
		inputs.push_back({ "> " });
		_block_append_stmt("input", inputs.size()-1);
		state.push_back({ PS_STMT_INPUT, int(inputs.size()-1) });
	}
	void input_end() {
		state.pop_back();
	}
	void if_start() {
		ifs.push_back({});
		_block_append_stmt("if", ifs.size()-1);
		state.push_back({ PS_STMT_IF, int(ifs.size()-1) });
	}
	void if_end() {
		state.pop_back();
	}
	void while_start() {
		whiles.push_back({});
		_block_append_stmt("while", whiles.size()-1);
		state.push_back({ PS_STMT_WHILE, int(whiles.size()-1) });
	}
	void while_end() {
		state.pop_back();
	}
	void return_start() {
		returns.push_back({});
		_block_append_stmt("return", returns.size()-1);
		state.push_back({ PS_STMT_RETURN, int(returns.size()-1) });
	}
	void return_end() {
		state.pop_back();
	}
	void call_start(const string& id) {
		calls.push_back({ id });
		_block_append_stmt("call", calls.size()-1);
		state.push_back({ PS_STMT_CALL, int(calls.size()-1) });
	}
	void call_end() {
		state.pop_back();
	}
	void set_start() {
		sets.push_back({ -1, -1 });
		_block_append_stmt("set", sets.size()-1);
		state.push_back({ PS_STMT_SET, int(sets.size()-1) });
	}
	void set_end() {
		state.pop_back();
	}
	void let_start() {
		lets.push_back({});
		_block_append_stmt("let", lets.size()-1);
		state.push_back({ PS_STMT_LET, int(lets.size()-1) });
	}
	void let_end() {
		state.pop_back();
	}


	void string_literal(const string& literal) {
		if      (curstate() == PS_STMT_PRINT)   prints.at(curid()).list.push_back("string_literal " + literal);
		else if (curstate() == PS_STMT_INPUT)   inputs.at(curid()).prompt = literal;
		else    printf("%d  ", curstate()),  Output::string_literal(literal);
	}
	void ex_start() {
		expressions.push_back({});
		int idx = expressions.size() - 1;
		if      (curstate() == PS_STMT_IF)      ifs.at(curid()).conds.push_back(idx);
		else if (curstate() == PS_STMT_WHILE)   whiles.at(curid()).cond = idx;
		else if (curstate() == PS_STMT_RETURN)  returns.at(curid()).expression = idx;
		else if (curstate() == PS_STMT_LET)     lets.at(curid()).expression = idx;
		else if (curstate() == PS_STMT_CALL)    calls.at(curid()).args.push_back(idx);
		else if (curstate() == PS_VARPATH)      varpaths.at(curid()).list.push_back("expression "+to_string(idx));
		else if (curstate() == PS_STMT_PRINT)   prints.at(curid()).list.push_back("expression "+to_string(idx));
		else    printf("%d  ", curstate()),  Output::ex_start();
		state.push_back({ PS_EXPRESSION, idx });
	}
	void ex_push(const string& ex) {
		expressions.at(curid()).list.push_back(ex);
	}
	void ex_end() {
		state.pop_back();
	}
	void varpath_start(const string& id) {
		varpaths.push_back({ {id} });
		int idx = varpaths.size() - 1;
		if      (curstate() == PS_EXPRESSION)   expressions.at(curid()).list.push_back("varpath "+to_string(idx));
		else if (curstate() == PS_STMT_LET)     lets.at(curid()).varpath = idx;
		else if (curstate() == PS_STMT_INPUT)   inputs.at(curid()).varpath = idx;
		else if (curstate() == PS_STMT_SET) {
			if      (sets.at(curid()).varpath1 == -1) sets.at(curid()).varpath1 = idx;
			else if (sets.at(curid()).varpath2 == -1) sets.at(curid()).varpath2 = idx;
		}
		else    printf("%d  ", curstate()),  Output::varpath_start(id);
		state.push_back({ PS_VARPATH, idx });
	}
	void varpath_push(const string& path) {
		varpaths.at(curid()).list.push_back(path);
	}
	void varpath_end() {
		state.pop_back();
	}


	void show() {
		// structs
		printf(":structs:      $%d\n", structs.size());
		for (int i = 0; i < structs.size(); i++) {
			printf("  %s\n", structs[i].id.c_str());
			for (const auto& d : structs[i].members)
				printf("\t%s %s %s\n", d.type.c_str(), d.id.c_str(), (d.isarray ? "[]" : ""));
		}
		printf(":dims:         $%d\n", dims.size());
		for (int i = 0; i < dims.size(); i++) {
			printf("  $%d  %s %s %s\n", i, dims[i].type.c_str(), dims[i].id.c_str(), (dims[i].isarray ? "[]" : ""));
		}
		printf(":funcs:        $%d\n", funcs.size());
		for (int i = 0; i < funcs.size(); i++) {
			printf("  %s\n", funcs[i].id.c_str());
			for (const auto& d : funcs[i].args)
				printf("\t%s %s %s\n", d.type.c_str(), d.id.c_str(), (d.isarray ? "[]" : ""));
		}


		// ifs
		printf(":ifs:          $%d\n", ifs.size());
		for (int i = 0; i < ifs.size(); i++) {
			printf("  $%d\n", i);
			for (const auto& c : ifs[i].conds)
				printf("\tcond   $%d\n", c);
			for (const auto& b : ifs[i].blocks)
				printf("\tblock  $%d\n", b);
		}
		// prints
		printf(":prints:       $%d\n", prints.size());
		for (int i = 0; i < prints.size(); i++) {
			printf("  $%d\n", i);
			for (const auto& s : prints[i].list)
				printf("\t%s\n", s.c_str());
		}


		// blocks
		printf(":blocks:       $%d\n", blocks.size());
		for (int i = 0; i < blocks.size(); i++) {
			printf("  $%d\n", i);
			for (const auto& s : blocks[i].stmts)
				printf("\t%s\n", s.c_str());
		}
		// varpaths
		printf(":varpaths:     $%d\n", varpaths.size());
		for (int i = 0; i < varpaths.size(); i++) {
			printf("  $%d  ", i);
			for (const auto& l : varpaths[i].list)
				printf("%s  ", l.c_str());
			printf("\n");
		}
		// expressions
		printf(":expressions:  $%d\n", expressions.size());
		for (int i = 0; i < expressions.size(); i++) {
			printf("  $%d  ", i);
			for (const auto& l : expressions[i].list)
				printf("%s  ", l.c_str());
			printf("\n");
		}


		printf(":inputs:       $%d\n", inputs.size());
		printf(":whiles:       $%d\n", whiles.size());
		printf(":returns:      $%d\n", returns.size());
		printf(":calls:        $%d\n", calls.size());
		printf(":sets:         $%d\n", sets.size());
	}
};