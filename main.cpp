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
				// TODO: word boundry check needed?
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
			else if (pattern == "integer") {
				while (Helpers::is_num(input.peek()))  result += input.get();
				return result.length() > 0;
			}
			else if (pattern == "string_literal") {
				if (input.peek() != '"')  return 0;
				input.get();
				while (input.peek()!='\n' && input.peek()!=EOF && input.peek()!='\"')
					result += input.get();
				if (input.peek() != '"')  return 0;
				input.get();
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
		close();
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
		close();
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


#include "output.hpp"
#include "outputb.hpp"
#include "outputc.hpp"


Input   inp;
OutputB outp;
// OutputC outp;

#include "method1.hpp"
#include "method2.hpp"
#include "tests.hpp"

int main() {
	printf("hello world\n");
	
	// tt_struct();
	// tt_struct2();
	// tt_globals();
	// tt_function();
	// tt_expression();
	// tt_block();
	tt_if();
	// tt_if2();
	// tt_variables();
}