#include "Teacher.h"
// Teacher.cpp
// Purpose: teacher methods including assigning grades and serialization.

Teacher::Teacher() : Person(), login(""), subjects() {}

Teacher::Teacher(const std::string &login, const std::string &name, const std::string &surname,
                 int day, int month, int year, const std::string &pesel)
    : Person(name, surname, day, month, year, pesel), login(login), subjects()
{
}

Teacher::~Teacher() {}

const std::string &Teacher::getLogin() const { return login; }

void Teacher::addSubject(const std::string &s) { subjects.push_back(s); }

const std::vector<std::string> &Teacher::getSubjects() const { return subjects; }

void Teacher::assignGrade(Student &student, const std::string &subject, double value) {
    Grade g(subject, value, login);
    student.addGrade(g);
}

string Teacher::toString() const {
    std::ostringstream oss;
    oss << login << ";" << Person::toString() << ";";
    for (size_t i = 0; i < subjects.size(); ++i) {
        if (i) oss << ",";
        oss << subjects[i];
    }
    return oss.str();
}

Teacher Teacher::fromString(const std::string &line) {
    // expected: login;name;surname;day;month;year;pesel;subj1,subj2...
    std::vector<std::string> parts;
    std::istringstream iss(line);
    std::string token;
    while (std::getline(iss, token, ';')) parts.push_back(token);

    Teacher t;
    if (parts.size() < 7) return t;
    t.login = parts[0];
    std::string name = parts[1];
    std::string surname = parts[2];
    int day = stoi(parts[3]);
    int month = stoi(parts[4]);
    int year = stoi(parts[5]);
    std::string pesel = parts[6];
    t = Teacher(t.login, name, surname, day, month, year, pesel);

    if (parts.size() >= 8 && !parts[7].empty()) {
        std::istringstream sss(parts[7]);
        std::string stoken;
        while (std::getline(sss, stoken, ',')) {
            t.addSubject(stoken);
        }
    }

    return t;
}

void Teacher::display() const {
    Person::display();
    cout << "Login: " << login << "\n";
    cout << "Subjects: ";
    if (subjects.empty()) cout << "(none)";
    for (size_t i = 0; i < subjects.size(); ++i) {
        if (i) cout << ", ";
        cout << subjects[i];
    }
    cout << "\n";
}
