#pragma once
#include <bits/stdc++.h>

class Course
{
private:
    std::string courseId;
    std::string name;
    std::string teacherLogin;

public:
    Course();
    Course(const std::string &id, const std::string &n, const std::string &tLogin);

    std::string getCourseId() const;
    std::string getName() const;
    std::string getTeacherLogin() const;

    std::string toString() const;
    static Course fromString(const std::string &line);
    void display() const;
};