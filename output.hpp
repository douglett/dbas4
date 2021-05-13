#pragma once


// ----------------------------------------
// Output base 
// ----------------------------------------


struct Output {
	int clear() {  // reset
		return 1;
	}


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


	void block_start() {
		printf("block start\n");
	}
	void block_end() {
		printf("	block end\n");
	}	
	void print_start() {
		printf("print start\n");
	}
	void print_end() {
		printf("	print end\n");
	}
	void input_start() {
		printf("input start\n");
	}
	void input_end() {
		printf("	input end\n");
	}
	void if_start() {
		printf("if start\n");
	}
	void if_end() {
		printf("	if end\n");
	}
	void while_start() {
		printf("while start\n");
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