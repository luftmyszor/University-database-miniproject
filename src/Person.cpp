#include "Person.h"

void Person::inputName() {
    cout << "Enter name: ";
    getline(cin, name);
}

void Person::inputSurname() {
    cout << "Enter surname: ";
    getline(cin, surname);
}

void Person::inputDayOfBirth() {
    int day;
    while (true) {
        cout << "Enter day of birth (1-31): ";
        cin >> day;
        if (day >= 1 && day <= 31) {
            dayOfBirth = day;
            break;
        }
        cout << "Invalid day. Please enter a value between 1 and 31." << endl;
    }
}

void Person::inputMonthOfBirth() {
    int month;
    while (true) {
        cout << "Enter month of birth (1-12): ";
        cin >> month;
        if (month >= 1 && month <= 12) {
            monthOfBirth = month;
            break;
        }
        cout << "Invalid month. Please enter a value between 1 and 12." << endl;
    }
}

void Person::inputYearOfBirth() {
    int year;
    while (true) {
        cout << "Enter year of birth: ";
        cin >> year;
        if (year >= 1900 && year <= 2026) {
            yearOfBirth = year;
            break;
        }
        cout << "Invalid year. Please enter a value between 1900 and 2026." << endl;
    }
}

void Person::inputPESEL() {
    while (true) {
        cout << "Enter PESEL number (11 digits): ";
        cin >> peselNumber;
        if (peselNumber.length() == 11 && all_of(peselNumber.begin(), peselNumber.end(), ::isdigit)) {
            break;
        }
        cout << "Invalid PESEL. Please enter exactly 11 digits." << endl;
    }
}
