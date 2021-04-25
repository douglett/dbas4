#pragma once


// ----------------------------------------
// Output base 
// ----------------------------------------


struct Output {
	// vector<string> output;

	void struct_start(const string& id) {
		printf("struct name: [%s]\n", id.c_str());
	}
	void struct_end() {
		printf("	struct end\n");
	}
	void dim_short(const string& type, const string& id, bool isarray) {
		printf("	member: [%s] [%s] %s\n", type.c_str(), id.c_str(), (isarray ? "array" : ""));
	}
	void dim_start(const string& type, const string& id) {
		printf("dim [%s] as [%s]\n", type.c_str(), id.c_str());
	}
	void dim_isarray(bool val) {
		printf("	array [%d]\n", 0);
	}
	void dim_end() {
		printf("	dim end\n");
	}
	void func_start(const string& id) {
		printf("function start: [%s]\n", id.c_str());
	}
	void func_end() {
		printf("	function end\n");
	}


	void print_start() {
		printf("print start:\n");
	}
	void print_end() {
		printf("	print end\n");
	}
	void input_start() {
		printf("input start:\n");
	}
	void input_end() {
		printf("	input end\n");
	}
	void if_start() {
		printf("if start:\n");
	}
	void if_end() {
		printf("	if end\n");
	}
	void while_start() {
		printf("while start:\n");
	}
	void while_end() {
		printf("	while end\n");
	}
	void return_start() {
		printf("return start\n");
	}
	void return_end() {
		printf("	return end\n");
	}
	void call_start(const string& id) {
		printf("call start: [%s]\n", id.c_str());
	}
	void call_end() {
		printf("	call end\n");
	}
	void set_start() {
		printf("set start\n");
	}
	void set_end() {
		printf("	set end\n");
	}
	void let_start() {
		printf("let start\n");
	}
	void let_end() {
		printf("	let end\n");
	}


	void string_literal(const string& literal) {
		printf("	literal: [%s]\n", literal.c_str());
	}
	void ex_start() {
		printf("expression start\n");
	}
	void ex_push(const string& ex) {
		printf("	%s\n", ex.c_str());
	}
	void ex_end() {
		printf("	expression end\n");
	}
	void varpath_start(const string& id) {
		printf("varpath: [%s]\n", id.c_str());
	}
	void varpath_push(const string& path) {
		printf("	%s\n", path.c_str());
	}
	void varpath_end() {
		printf("	varpath end\n");
	}
};



// ----------------------------------------
// Output method B (structure building)
// ----------------------------------------



struct wb_expression {
	vector<string> list;
};
struct wb_varpath {
	vector<string> list;
};
struct wb_dimshort {
	string type, id;
	bool isarray;
};
struct wb_dim {
	string type, id;
	bool isarray = false;
	int arraylength = 0;
	wb_dim(string _type, string _id) : type(_type), id(_id) {}
};
struct wb_struct {
	string id;
	vector<wb_dimshort> members;
};
struct wb_function {
	string id;
	vector<wb_dimshort> args;
};
struct wb_stmt_print {
	vector<string>   literals;
};
struct wb_stmt_input {
	string prompt;
};
struct wb_stmt_if {
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
	vector<PSTATE_T>         state;
	vector<wb_expression>    exprs;
	vector<wb_varpath>       varpaths;
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


	PSTATE_T cstate() { return state.size() ? state.back() : PS_NONE; }


	void struct_start(const string& id) {
		// Output::struct_start(id);
		state.push_back(PS_STRUCT);
		structs.push_back({ id });
	}
	void struct_end() {
		// Output::struct_end();
		state.pop_back();
		printf("structs count: %d\n", structs.size());
	}
	void dim_short(const string& type, const string& id, bool isarray) {
		// Output::dim_short(type, id, isarray);
		if      (cstate() == PS_STRUCT)    structs.back().members.push_back({ type, id, isarray });
		else if (cstate() == PS_FUNCTION)  funcs.back().args.push_back({ type, id, isarray });
		else    Output::dim_short(type, id, isarray);
	}
	void dim_start(const string& type, const string& id) {
		state.push_back(PS_DIM);
		dims.push_back(wb_dim(type, id));
	}
	void dim_isarray(bool val) {
		dims.back().isarray = val;
	}
	void dim_end() {
		state.pop_back();
	}
	void func_start(const string& id) {
		state.push_back(PS_FUNCTION);
		funcs.push_back({ id });
	}
	void func_end() {
		state.pop_back();
	}


	void print_start() {
		state.push_back(PS_STMT_PRINT);
		prints.push_back({});
	}
	void print_end() {
		state.pop_back();
	}
	void input_start() {
		state.push_back(PS_STMT_INPUT);
		inputs.push_back({ "> " });
	}
	void input_end() {
		state.pop_back();
	}
	void if_start() {
		state.push_back(PS_STMT_IF);
		ifs.push_back({});
	}
	void if_end() {
		state.pop_back();
	}
	void while_start() {
		state.push_back(PS_STMT_WHILE);
		whiles.push_back({});
	}
	void while_end() {
		state.pop_back();
	}
	void return_start() {
		state.push_back(PS_STMT_RETURN);
		returns.push_back({});
	}
	void return_end() {
		state.pop_back();
	}
	void call_start(const string& id) {
		state.push_back(PS_STMT_CALL);
		calls.push_back({ id });
	}
	void call_end() {
		state.pop_back();
	}
	void set_start() {
		state.push_back(PS_STMT_SET);
		sets.push_back({});
	}
	void set_end() {
		state.pop_back();
	}
	void let_start() {
		state.push_back(PS_STMT_LET);
		lets.push_back({});
	}
	void let_end() {
		state.pop_back();
	}


	void string_literal(const string& literal) {
		if      (cstate() == PS_STMT_PRINT)   prints.back().literals.push_back({ literal });
		else if (cstate() == PS_STMT_INPUT)   inputs.back().prompt = literal;
		else    Output::string_literal(literal);
	}
	void ex_start() {
		state.push_back(PS_EXPRESSION);
		exprs.push_back({});
	}
	void ex_push(const string& ex) {
		exprs.back().list.push_back(ex);
	}
	void ex_end() {
		state.pop_back();
	}
	void varpath_start(const string& id) {
		state.push_back(PS_VARPATH);
		varpaths.push_back({ {id} });
	}
	void varpath_push(const string& path) {
		varpaths.back().list.push_back(path);
	}
	void varpath_end() {
		state.pop_back();
	}
};