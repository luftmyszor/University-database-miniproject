#include "Course.h"

Course::Course() : courseId(""), name(""), teacherLogin("") {}

Course::Course(const std::string &id, const std::string &n, const std::string &tLogin)
    : courseId(id), name(n), teacherLogin(tLogin) {}

std::string Course::getCourseId() const { return courseId; }
std::string Course::getName() const { return name; }
std::string Course::getTeacherLogin() const { return teacherLogin; }

std::string Course::toString() const
{
    return courseId + ";" + name + ";" + teacherLogin;
}

Course Course::fromString(const std::string &line)
{
    std::istringstream iss(line);
    std::string id, name, tLogin;
    std::getline(iss, id, ';');
    std::getline(iss, name, ';');
    std::getline(iss, tLogin, ';');
    return Course(id, name, tLogin);
}

void Course::display() const
{
    std::cout << "Course [" << courseId << "] " << name
              << " | Coordinator: " << (teacherLogin.empty() ? "Unassigned" : teacherLogin) << "\n";
}