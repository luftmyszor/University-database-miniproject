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

double Student::average() const
{
    if (grades.empty())
        return 0.0;
    double sum = 0.0;
    for (auto it = grades.begin(); it != grades.end(); ++it)
    {
        sum += it->value;
    }
    return sum / grades.size();
}

string Student::toString() const
{
    std::ostringstream oss;
    oss << login << ";" << Person::toString() << ";";
    for (size_t i = 0; i < enrolledCourses.size(); ++i)
    {
        if (i)
            oss << ",";
        oss << enrolledCourses[i];
    }
    return oss.str();
}

Student Student::fromString(const std::string &line)
{
    // expected: login;name;surname;day;month;year;pesel;course1,course2...
    std::vector<std::string> parts;
    std::istringstream iss(line);
    std::string token;
    while (std::getline(iss, token, ';'))
        parts.push_back(token);

    Student s;
    if (parts.size() < 7)
        return s;
    s = Student(parts[0], parts[1], parts[2], stoi(parts[3]), stoi(parts[4]), stoi(parts[5]), parts[6]);

    if (parts.size() >= 8 && !parts[7].empty())
    {
        std::istringstream css(parts[7]);
        std::string ctoken;
        while (std::getline(css, ctoken, ','))
        {
            s.enroll(ctoken);
        }
    }
    return s;
}

void Student::display() const
{
    Person::display();
    cout << "Login: " << login << "\n";
    cout << "Grades:\n";
    if (grades.empty())
        cout << "  (no grades)\n";
    for (auto it = grades.begin(); it != grades.end(); ++it)
    {
        cout << "  " << it->subject << ": " << it->value << " (by " << it->teacherLogin << ")\n";
    }
    cout << "Average: " << average() << "\n";
}
