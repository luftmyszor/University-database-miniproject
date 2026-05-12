#pragma once
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
        void inputName();
        void inputSurname();
        void inputDayOfBirth();
        void inputMonthOfBirth();
        void inputYearOfBirth();
        void inputPESEL();
};


