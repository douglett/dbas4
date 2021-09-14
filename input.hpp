#pragma once
#include <fstream>
#include <sstream>
#include "helpers.hpp"


// struct WizParseError : WizError {
// 	int errorcode = 0;
// 	int lineindex = -1;
// 	virtual void buildmsg() {
// 		msg = "ParseError: error code " + to_string(errorcode) + ", on line " + to_string(lineindex+1);
// 	}
// };


// ----------------------------------------
// Source input matching pattern
// ----------------------------------------


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
		// if      (patt.front() == '\'')  patt = patt.substr(1), type = PT_LITERAL;
		// else if (patt.front() == '@' )  patt = patt.substr(1), type = PT_RULE, is_record = true;
		// else    type = PT_RULE;
		if      (patt.front() == '@' )  patt = patt.substr(1), is_record = true;
		if      (patt.front() == '\'')  patt = patt.substr(1), type = PT_LITERAL;
		else    type = PT_RULE;
		assert(patt.length() > 0);
		pattern = patt;
	}
	int record() {
		return is_record;
	}
	int wordbreak(iostream& input, const string& str) {
		if      (str.length() == 0)  return 0;
		else if (!Helpers::is_alphanum(str.back()))  return 1;
		else if (Helpers::is_alphanum(str.back()) && !Helpers::is_alphanum(input.peek()))  return 1;
		else    return 0;
	}
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
			// return 1;
			return wordbreak(input, result);
		case PT_RULE:
			if (pattern == "identifier") {
				if (!Helpers::is_alpha(input.peek()))  return 0;
				result += input.get();
				while (Helpers::is_alphanum(input.peek()))  result += input.get();
				// return 1;
				return wordbreak(input, result);
			}
			else if (pattern == "integer") {
				while (Helpers::is_num(input.peek()))  result += input.get();
				// return result.length() > 0;
				return result.length() > 0 && wordbreak(input, result);
			}
			else if (pattern == "string_literal") {
				if (input.peek() != '"')  return 0;
				input.get();
				while (input.peek()!='\n' && input.peek()!=EOF && input.peek()!='\"')
					result += input.get();
				if (input.peek() != '"')  return 0;
				input.get();
				// return 1;
				return wordbreak(input, result);
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


// ----------------------------------------
// Source input read
// ----------------------------------------


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
		return type = 1,  1;
	}
	int close() {
		ss = stringstream();
		if (fs.is_open())
			fs.close();
		return type = 0,  1;
	}
	int clear() {  // reset
		return close();
	}
	int load(const vector<string>& lines) {
		close();
		for (int i = 0; i < lines.size(); i++)
			if (i == 0) ss << lines[i];
			else        ss << endl << lines[i];
		ss.seekg(ios::beg), ss.clear();
		return type = 0,  1;
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
	int expect(const string& patterns) {
		Results r;
		return expect(patterns, r);
	}
	int expect(const string& patterns, Results& r) {
		if (get(patterns, r))  return 1;
		WizParseError p;
			p.error_code = WIZERR_EXPECT_TOKEN;
			p.line_no    = lineindex();
			p.error_text = "expected: [ " + patterns + " ]";
			p.buildmsg();
		throw p;
		// return 0;
	}

	// file info
	int lineindex() {
		int count = 0, c = 0;
		auto pos = tell();
		seek(ios::beg);
		while (tell() < pos && (c = stream().get()) != EOF)
			if (c == '\n')  count++;
		return seek(pos), count;
	}
};