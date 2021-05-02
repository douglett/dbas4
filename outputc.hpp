struct ostate {
	string type;
	int count, x;
};


struct OutputC : Output {
	vector<string>  ln;
	vector<ostate>  state;
	int count = 0;

	void oprint(const string& s) {
		ln.push_back(s);
		string spc = s.at(0) == ':' ? "" : "  .";
		printf("%s%s\n", spc.c_str(), s.c_str());
	}
	void statelabel() {
		const auto& st = state.back();
		oprint(":" + st.type + "_" + to_string(st.count) + "_" + to_string(st.x));
	}


	void dim_short(const string& type, const string& id, bool isarray) {
		if (state.back().type == "function")  oprint("dimlocal "+id);
	}
	void func_start(const string& id) {
		state.push_back({ "function", ++count, 1 });
		statelabel();
	}
	void func_end() {
		oprint("ret");
		state.pop_back();
	}


	void print_start() {
		state.push_back({ "print", ++count, 1 });
	}
	void print_next() {
		oprint("print");
	}
	void print_end() {
		oprint("lit endl");
		oprint("print");
		state.pop_back();
	}
	void if_start() {
		state.push_back({ "if", ++count, 1 });
		statelabel();
	}
	// void if_cond() {
	// 	const auto& st = state.back();
	// 	oprint("ifn if_" + to_string(st.count) + "_" + to_string(st.x+1));
	// }
	void if_end() {
		state.back().x++;
		statelabel();
		state.pop_back();
	}
	// void while_start() {
	// 	state.push_back({ "while", ++count, 1 });
	// 	statelabel();
	// }
	// void while_end() {
	// 	oprint("goto :while_"+to_string(state.back().count)+"_1");
	// 	state.back().x++;
	// 	statelabel();
	// 	state.pop_back();
	// }
	void let_start() {
	}
	void let_end() {
		oprint("setmem $2 $1 \t; TODO");
	}


	void string_literal(const string& literal) {
		oprint("lit \"" + literal + "\"");
	}
	void ex_start() {
		// state.push_back({ "ex" });
	}
	void ex_push(const string& ex) {
		oprint(ex);
	}
	void ex_end() {
		// state.pop_back();
	}
	void varpath_start(const string& id) {
		// state.push_back(PS_VARPATH);
		// varpaths.push_back({ {id} });
		oprint("get "+id);
	}
	void varpath_push(const string& path) {
		oprint(path);
	}
	void varpath_end() {
		// state.pop_back();
	}
};