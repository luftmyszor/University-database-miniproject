#pragma once
// Grade.h
// Purpose: small struct representing a grade (subject, value, teacherLogin).
#include <bits/stdc++.h>
using namespace std;

struct Grade {
	string subject;
	double value;
	string teacherLogin;

	Grade() : subject(""), value(0.0), teacherLogin("") {}
	Grade(const string &s, double v, const string &t) : subject(s), value(v), teacherLogin(t) {}

	string toString() const {
		ostringstream oss;
		oss << subject << ":" << value << ":" << teacherLogin;
		return oss.str();
	}

	static Grade fromString(const string &line) {
		Grade g;
		// expected format: subject:value:teacher
		size_t p1 = line.find(':');
		if (p1 == string::npos) return g;
		size_t p2 = line.find(':', p1 + 1);
		if (p2 == string::npos) return g;
		g.subject = line.substr(0, p1);
		try {
			g.value = stod(line.substr(p1 + 1, p2 - p1 - 1));
		} catch (...) {
			g.value = 0.0;
		}
		g.teacherLogin = line.substr(p2 + 1);
		return g;
	}
};
