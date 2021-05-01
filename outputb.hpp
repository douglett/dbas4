#pragma once


// ----------------------------------------
// Output method B (structure building)
// ----------------------------------------


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
	vector<string> literals;
};
struct wb_stmt_input {
	string prompt;
};
struct wb_stmt_if {
	vector<uint32_t> conds;
	vector<uint32_t> blocks;
};
struct wb_stmt_while {
};
struct wb_stmt_return {
};
struct wb_stmt_call {
	string id;
};
struct wb_stmt_set {
};
struct wb_stmt_let {
	uint32_t varpath, expression;
};
struct wb_varpath {
	vector<string> list;
};
struct wb_expression {
	vector<string> list;
};



struct OutputB : Output {
	enum PSTATE_T {
		PS_NONE = 0,
		PS_EXPRESSION,
		PS_VARPATH,
		// PS_STRUCT_BLOCK,
		// PS_GLOBAL_BLOCK,
		// PS_FUNCTION_BLOCK,
		PS_STRUCT,
		PS_DIM,
		// PS_DIM_SHORT,
		PS_FUNCTION,
		// PS_FUNCTION_ARGS,
		// PS_FUNCTION_END,
		// PS_BLOCK,
		PS_STMT_BLOCK,
		PS_STMT_PRINT,
		// PS_STMT_PRINT_ARGS,
		PS_STMT_INPUT,
		PS_STMT_IF,
		// PS_STMT_IF2,
		// PS_STMT_IF_END,
		PS_STMT_WHILE,
		// PS_STMT_WHILE2,
		// PS_STMT_WHILE_END,
		PS_STMT_RETURN,
		// PS_STMT_RETURN_END,
		// PS_STRING_LITERAL,
		PS_STMT_CALL,
		PS_STMT_SET,
		PS_STMT_LET,
	};
	struct pstate {
		PSTATE_T pstate;
		uint32_t id;
	};
	vector<pstate>           state;

	vector<wb_expression>    expressions;
	vector<wb_varpath>       varpaths;
	vector<wb_struct>        structs;
	vector<wb_dim>           dims;
	vector<wb_function>      funcs;
	vector<wb_stmt_block>    blocks;
	vector<wb_stmt_print>    prints;
	vector<wb_stmt_input>    inputs;
	vector<wb_stmt_if>       ifs;
	vector<wb_stmt_while>    whiles;
	vector<wb_stmt_return>   returns;
	vector<wb_stmt_call>     calls;
	vector<wb_stmt_set>      sets;
	vector<wb_stmt_let>      lets;


	PSTATE_T curstate() { return state.size() ? state.back().pstate : PS_NONE; }
	int      curid()    { return state.size() ? state.back().id     : -1; }


	void struct_start(const string& id) {
		structs.push_back({ id });
		state.push_back({ PS_STRUCT, structs.size()-1 });
	}
	void struct_end() {
		state.pop_back();
		printf("structs count: %d\n", structs.size());
	}
	void dim_short(const string& type, const string& id, bool isarray) {
		if      (curstate() == PS_STRUCT)    structs.at(curid()).members.push_back({ type, id, isarray });
		else if (curstate() == PS_FUNCTION)  funcs.at(curid()).args.push_back({ type, id, isarray });
		else    Output::dim_short(type, id, isarray);
	}
	void dim_start(const string& type, const string& id) {
		dims.push_back({ type, id });
		state.push_back({ PS_DIM, dims.size()-1 });
	}
	void dim_isarray(bool val) {
		dims.at(curid()).isarray = val;
	}
	void dim_end() {
		state.pop_back();
	}
	void func_start(const string& id) {
		funcs.push_back({ id });
		state.push_back({ PS_FUNCTION, funcs.size()-1 });
	}
	void func_end() {
		state.pop_back();
	}


	void block_start() {
		blocks.push_back({});
		if    (curstate() == PS_STMT_IF)  ifs.at(curid()).blocks.push_back(blocks.size()-1);
		else  printf("%d \n", curstate());
		state.push_back({ PS_STMT_BLOCK, blocks.size()-1 });
	}
	void block_end() {
		state.pop_back();
	}	
	void print_start() {
		prints.push_back({});
		state.push_back({ PS_STMT_PRINT, prints.size()-1 });
	}
	void print_next() {
	}
	void print_end() {
		state.pop_back();
	}
	void input_start() {
		inputs.push_back({ "> " });
		state.push_back({ PS_STMT_INPUT, inputs.size()-1 });
	}
	void input_end() {
		state.pop_back();
	}
	void if_start() {
		ifs.push_back({});
		// printf("if: %d\n", curstate());
		if    (curstate() == PS_STMT_BLOCK)  blocks.at(curid()).stmts.push_back("if "+to_string(ifs.size()-1));
		else  printf("%d ", curstate()),  Output::if_start();
		state.push_back({ PS_STMT_IF, ifs.size()-1 });
	}
	void if_cond() {
	}
	void if_cond_end() {
	}
	void if_end() {
		state.pop_back();
	}
	void while_start() {
		whiles.push_back({});
		state.push_back({ PS_STMT_WHILE, whiles.size()-1 });
	}
	void while_end() {
		state.pop_back();
	}
	void return_start() {
		returns.push_back({});
		state.push_back({ PS_STMT_RETURN, returns.size()-1 });
	}
	void return_end() {
		state.pop_back();
	}
	void call_start(const string& id) {
		calls.push_back({ id });
		state.push_back({ PS_STMT_CALL, calls.size()-1 });
	}
	void call_end() {
		state.pop_back();
	}
	void set_start() {
		sets.push_back({});
		state.push_back({ PS_STMT_SET, sets.size()-1 });
	}
	void set_end() {
		state.pop_back();
	}
	void let_start() {
		lets.push_back({});
		if    (curstate() == PS_STMT_BLOCK)  blocks.at(curid()).stmts.push_back("let "+to_string(lets.size()-1));
		// else  printf("%d ", curstate()),  Output::let_start();
		state.push_back({ PS_STMT_LET, lets.size()-1 });
	}
	void let_end() {
		state.pop_back();
	}


	void string_literal(const string& literal) {
		if      (curstate() == PS_STMT_PRINT)   prints.at(curid()).literals.push_back({ literal });
		else if (curstate() == PS_STMT_INPUT)   inputs.at(curid()).prompt = literal;
		else    printf("%d  ", curstate()),  Output::string_literal(literal);
	}
	void ex_start() {
		expressions.push_back({});
		if      (curstate() == PS_STMT_IF)  ifs.at(curid()).conds.push_back(expressions.size()-1);
		// else if (curstate() == PS_VARPATH)
		else if (curstate() == PS_STMT_LET)  lets.at(curid()).expression = expressions.size()-1;
		else    printf("%d  ", curstate()),  Output::ex_start();
		state.push_back({ PS_EXPRESSION, expressions.size()-1 });
	}
	void ex_push(const string& ex) {
		expressions.at(curid()).list.push_back(ex);
	}
	void ex_end() {
		state.pop_back();
	}
	void varpath_start(const string& id) {
		varpaths.push_back({ {id} });
		if      (curstate() == PS_EXPRESSION)   expressions.at(curid()).list.push_back("varpath "+to_string(varpaths.size()-1));
		else if (curstate() == PS_STMT_LET)   lets.at(curid()).varpath = curid();
		else    printf("%d  ", curstate()),  Output::varpath_start(id);
		state.push_back({ PS_VARPATH, varpaths.size()-1 });
	}
	void varpath_push(const string& path) {
		varpaths.at(curid()).list.push_back(path);
	}
	void varpath_end() {
		state.pop_back();
	}


	void show() {
		// ifs
		printf(":ifs:          $%d\n", ifs.size());
		for (int i = 0; i < ifs.size(); i++) {
			printf("  $%d\n", i);
			for (const auto& c : ifs[i].conds)
				printf("\tcond   $%d\n", c);
			for (const auto& b : ifs[i].blocks)
				printf("\tblock  $%d\n", b);
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


		printf(":structs:      $%d\n", structs.size());
		printf(":dims:         $%d\n", dims.size());
		printf(":funcs:        $%d\n", funcs.size());
		printf(":prints:       $%d\n", prints.size());
		printf(":inputs:       $%d\n", inputs.size());
		printf(":whiles:       $%d\n", whiles.size());
		printf(":returns:      $%d\n", returns.size());
		printf(":calls:        $%d\n", calls.size());
		printf(":sets:         $%d\n", sets.size());
	}
};