#pragma once
// Person.h
// Purpose: base class for `Student` and `Teacher` storing basic personal data.
#include <bits/stdc++.h>
using namespace std;

class Person {
private:
    string name;
    string surname;
    int dayOfBirth;
    int monthOfBirth;
    int yearOfBirth;
    string peselNumber;

public:
    Person();
    Person(const string &name, const string &surname, int day, int month, int year, const string &pesel);
    virtual ~Person();

    void inputName();
    void inputSurname();
    void inputDayOfBirth();
    void inputMonthOfBirth();
    void inputYearOfBirth();
    void inputPESEL();

    string getName() const;
    string getSurname() const;
    string getPESEL() const;
    int getDay() const;
    int getMonth() const;
    int getYear() const;

    virtual string toString() const;
    virtual void display() const;
};


