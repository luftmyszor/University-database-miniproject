#include "Student.h"
// Student.cpp
// Purpose: student methods including parsing/serialization and grade handling.

Student::Student() : Person(), login(""), grades() {}

Student::Student(const std::string &login, const std::string &name, const std::string &surname,
                 int day, int month, int year, const std::string &pesel)
    : Person(name, surname, day, month, year, pesel), login(login), grades()
{
}

Student::~Student() {}

const std::string &Student::getLogin() const { return login; }

void Student::addGrade(const Grade &g) { grades.push_back(g); }

double Student::average() const {
    if (grades.empty()) return 0.0;
    double sum = 0.0;
    for (auto it = grades.begin(); it != grades.end(); ++it) {
        sum += it->value;
    }
    return sum / grades.size();
}

string Student::toString() const {
    std::ostringstream oss;
    // login;person;grade1,grade2...
    oss << login << ";" << Person::toString() << ";";
    for (size_t i = 0; i < grades.size(); ++i) {
        if (i) oss << ",";
        oss << grades[i].toString();
    }
    return oss.str();
}

Student Student::fromString(const std::string &line) {
    // expected: login;name;surname;day;month;year;pesel;grade1,grade2...
    std::vector<std::string> parts;
    std::istringstream iss(line);
    std::string token;
    while (std::getline(iss, token, ';')) parts.push_back(token);

    Student s;
    if (parts.size() < 7) return s;
    s.login = parts[0];
    std::string name = parts[1];
    std::string surname = parts[2];
    int day = stoi(parts[3]);
    int month = stoi(parts[4]);
    int year = stoi(parts[5]);
    std::string pesel = parts[6];
    s = Student(s.login, name, surname, day, month, year, pesel);

    if (parts.size() >= 8 && !parts[7].empty()) {
        std::istringstream gss(parts[7]);
        std::string gtoken;
        while (std::getline(gss, gtoken, ',')) {
            Grade g = Grade::fromString(gtoken);
            s.addGrade(g);
        }
    }

    return s;
}

void Student::display() const {
    Person::display();
    cout << "Login: " << login << "\n";
    cout << "Grades:\n";
    if (grades.empty()) cout << "  (no grades)\n";
    for (auto it = grades.begin(); it != grades.end(); ++it) {
        cout << "  " << it->subject << ": " << it->value << " (by " << it->teacherLogin << ")\n";
    }
    cout << "Average: " << average() << "\n";
}
