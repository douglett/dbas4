#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <cassert>

using namespace std;


// useful stuff
struct Helpers {
	static int is_alpha    (char c) { return (c>='A' && c<='Z') || (c>='a' && c<='z') || c=='_'; }
	static int is_num      (char c) { return c>='0' && c<='9'; }
	static int is_alphanum (char c) { return is_alpha(c) || is_num(c); }
};


// Source file matching pattern
struct Pattern {
	enum PT_TYPE {
		PT_NONE = 0,
		PT_LITERAL = 1,
		PT_RULE,
	};
	PT_TYPE type = PT_NONE;
	bool    is_record = false;
	string  pattern;

	static vector<Pattern> compile(const string& patterns) {
		vector<Pattern> vp;
		stringstream ss(patterns);
		string p;
		while (ss >> p)  vp.push_back( Pattern(p) );
		return vp;
	}

	Pattern(string patt) {
		assert(patt.length() > 0);
		if      (patt.front() == '\'')  patt = patt.substr(1), type = PT_LITERAL;
		else if (patt.front() == '@' )  patt = patt.substr(1), type = PT_RULE, is_record = true;
		else    type = PT_RULE;
		assert(patt.length() > 0);
		pattern = patt;
	}
	int record() { return is_record; }
	int match(iostream& input, string& result) {
		result = "";
		switch (type) {
		case PT_NONE:
			assert("Pattern::match > matching on blank pattern" == NULL);
		case PT_LITERAL:
			for (auto c : pattern) {
				if (input.peek() != c)  return 0;
				result.push_back(input.get());
			}
			return 1;
		case PT_RULE:
			if (pattern == "identifier") {
				if (!Helpers::is_alpha(input.peek()))  return 0;
				result += input.get();
				while (Helpers::is_alphanum(input.peek()))  result += input.get();
				return 1;
			}
			else if (pattern == "endl")
				return input.peek()=='\n' ? (input.get(), 1) : 0;
			else if (pattern == "eof")
				return input.peek()==EOF ? (input.get(), 1) : 0;
			else if (pattern == "endall")
				return input.peek()=='\n' || input.peek()==EOF ? (input.get(), 1) : 0;
			fprintf(stderr, "Pattern::match > unknown pattern [%s]\n", pattern.c_str());
			exit(1);
		}
		return 0;
	}
};


// Source Input
struct Input {
	typedef  vector<string>  Results;
	int type = 0;
	stringstream ss;
	fstream fs;

	// init functions
	int open(const string& fname) {
		fs.open(fname, ios::in);
		if (!fs.is_open())
			return fprintf(stderr, "could not open file: %s\n", fname.c_str()), 0;
		return type = 1, 1;
	}
	int close() {
		ss = stringstream();
		if (fs.is_open())
			fs.close();
		type = 0;
		return 1;
	}
	int load(const vector<string>& lines) {
		for (int i = 0; i < lines.size(); i++)
			if (i == 0) ss << lines[i];
			else        ss << endl << lines[i];
		ss.seekg(ios::beg), ss.clear();
		return type = 0, 1;
	}

	// helpers
	iostream& stream() { return type ? (iostream&)fs : (iostream&)ss; }
	streampos tell() { return stream().tellg(); }
	void      seek(streampos pos) { stream().seekg(pos), stream().clear(); }
	// int       eof() { return stream().peek(), stream().eof(); }
	// char      getch() { return stream().get(); }
	int clearws() {
		while (stream().peek() == ' ' || stream().peek() == '\t')  stream().get();
		return stream().clear(), 0;
	}

	// parsing functions
	int get(const string& patterns, Results& r) {
		string s;
		r = {};
		auto plist = Pattern::compile(patterns);
		auto pos = tell();
		for (auto& p : plist) {
			clearws();
			if (!p.match(stream(), s))  return seek(pos), 0; // cancel and rewind stream
			if (p.record())  r.push_back(s);
		}
		return 1;
	}
	int get(const string& patterns) {
		Results r;
		return get(patterns, r);
	}
	int peek(const string& patterns) {
		auto pos = tell();
		int  res = get(patterns);
		return seek(pos), res;
	}
	int expect(const string& patterns, Results& r, int errorcode=1) {
		if    (get(patterns, r)) return 1;
		else  return error(errorcode);
	}
	int expect(const string& patterns, int errorcode=1) {
		Results r;
		return expect(patterns, r, errorcode);
	}

	// error recording
	int error(int errorcode) {
		fprintf(stderr, "error code: %d\n", errorcode);
		fprintf(stderr, "	on line: %d\n", lineindex()+1);
		exit(1);
		return 0;
	}
	int lineindex() {
		int count = 0, c = 0;
		auto pos = tell();
		seek(ios::beg);
		while (tell() < pos && (c = stream().get()) != EOF)
			if (c == '\n')  count++;
		return seek(pos), count;
	}
};


// ASM Output
struct Output {
	vector<string> output;
	void struct_start(const string& id) {
		printf("struct name: [%s]\n", id.c_str());  }
	void struct_addmember(const string& type, const string& id, bool isarray) {
		printf("	member: [%s] [%s] %s\n", type.c_str(), id.c_str(), (isarray ? "array" : ""));  }
	void struct_end() {
		printf("	struct end\n");  }
	void dim_start(const string& type, const string& id) {
		printf("dim [%s] as [%s]\n", type.c_str(), id.c_str());  }
	void dim_isarray(bool val) {
		printf("	array [%d]\n", 0);  }
	void dim_end() {
		printf("	dim end\n");  }
	void func_start(const string& id) {}
	void func_arg(const string& type, const string& id, bool isarray) {}
	void func_end() {}
};


// Parse State
struct PState {
	enum PSTATE_T {
		PS_NONE = 0,
		PS_STRUCT_BLOCK,
		PS_GLOBAL_BLOCK,
		PS_FUNCTION_BLOCK,
		PS_STRUCT,
		PS_GLOBAL,
		PS_FUNCTION,
		PS_FUNCTION_END,
		PS_BLOCK,
	};
	vector<PSTATE_T> _stack;
	PSTATE_T current() { return _stack.size() ? _stack.back() : PS_NONE; }
	void push(PSTATE_T state) { _stack.push_back(state); }
	void pop() { if (_stack.size()) _stack.pop_back(); };
};


Input  inp;
Output outp;
PState state;


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
		else if (state.current() == PState::PS_GLOBAL_BLOCK && inp.peek("'dim"))  state.push(PState::PS_GLOBAL);
		else if (state.current() == PState::PS_FUNCTION_BLOCK && inp.peek("'function"))  state.push(PState::PS_FUNCTION);
		else    state.pop();
		break;

	case PState::PS_STRUCT:
		inp.expect("'struct @identifier endl", r1);
		outp.struct_start(r1.at(0));
		while (true)
			if      (inp.peek("'end"))  break;
			else if (inp.get("endl"))  ; // ingnore empty lines
			else if (inp.get("@identifier @identifier '[ '] endl", r2))  outp.struct_addmember(r2.at(0), r2.at(1), true);
			else if (inp.get("@identifier @identifier endl", r2))  outp.struct_addmember(r2.at(0), r2.at(1), false);
			else if (inp.get("@identifier '[ '] endl", r2))  outp.struct_addmember("int", r2.at(0), true);
			else if (inp.get("@identifier endl", r2))  outp.struct_addmember("int", r2.at(0), false);
			else    break;
		inp.expect("'end 'struct endall");
		outp.struct_end();
		state.pop();
		break;
	
	case PState::PS_GLOBAL:
		if   (inp.get("'dim @identifier @identifier", r1))  outp.dim_start(r1.at(0), r1.at(1));
		else inp.expect("'dim @identifier", r1), outp.dim_start("int", r1.at(0));
		if (inp.get("'[ ']")) outp.dim_isarray(true);
		if (inp.get("'="))  ; // expr
		inp.expect("endall");
		outp.dim_end();
		state.pop();
		break;

	case PState::PS_FUNCTION:
		inp.expect("'function @identifier '(", r1);
		outp.func_start(r1.at(0));
		while (true) {
			if      (inp.get("@identifier @identifier '[ ']", r2))  outp.func_arg(r2.at(0), r2.at(1), true);
			else if (inp.get("@identifier @identifier", r2))  outp.func_arg(r2.at(0), r2.at(1), false);
			else if (inp.get("@identifier '[ ']", r2))  outp.func_arg("int", r2.at(0), true);
			else if (inp.get("@identifier", r2))  outp.func_arg("int", r2.at(0), false);
			if (!inp.get("',")) break;
		}
		inp.expect("') endl");
		state.pop();
		state.push(PState::PS_FUNCTION_END);
		state.push(PState::PS_BLOCK);
		break;

	case PState::PS_FUNCTION_END:
		inp.expect("'end 'function endl");
		outp.func_end();
		state.pop();
		break;

	case PState::PS_BLOCK:
		while (true) {
			if      (inp.get("endl"))  ;
			// else if (inp.get("@identifier"))  ;
		}
		state.pop();
		break;
	} // end switch
}


void test_struct() {
	inp.load({
		"struct test",
		"	a1",
		"	b2",
		"	c3",
		"end struct",
		// "",
	});
	state.push(PState::PS_STRUCT);
	ploop();
}
void test_struct2() {
	inp.load({
		"struct test1",
		"	a",
		"	b[]",
		"end struct",
		"",
		"struct test1",
		"	float a",
		"	float b[]",
		"end struct",
	});
	state.push(PState::PS_STRUCT_BLOCK);
	ploop();
}
void test_globals() {
	inp.load({
		"dim a",
		"dim b[]",
		"",
		"dim float c[]",
	});
	state.push(PState::PS_GLOBAL_BLOCK);
	ploop();
}


int main() {
	printf("hello world\n");
	
	// farttest(); exit(1);
	
	// test_struct();
	test_struct2();
	// test_globals();
}