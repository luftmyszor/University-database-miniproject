#include "CLIManager.h"
#include "Utils.h"
void CLIManager::CLI()
{
    try {
        switch (_curentState)
        {
        case START:
            std::cout << "========================================\n";
            std::cout << "  University Database CLI\n";
            std::cout << "  Manage students, courses and grades\n";
            std::cout << "  Roles: admin, teacher, student\n";
            std::cout << "  Usage: enter the option number then press Enter\n";
            if (auth.doesUserExist("admin")) {
                std::cout << "  Tip: default admin account exists: login 'admin' / password 'admin'\n";
            }
            std::cout << "========================================\n";
            std::cout << "1) Sign in\n";
            std::cout << "2) Sign up\n";
            std::cout << "3) Exit\n";
            std::cout << "> ";
            switch (getOption())
            {
            case 1:
                try {
                    _currentUser = auth.signIn();
                } catch (const std::exception &e) {
                    std::cout << "Sign in failed: " << e.what() << "\n";
                    break;
                }

                // dispatch to role menus
                switch (_currentUser.getType()) {
                    case 0: // ADMIN
                        adminMenu();
                        break;
                    case 1: // TEACHER
                        teacherMenu();
                        break;
                    case 2: // STUDENT
                        studentMenu();
                        break;
                    default:
                        break;
                }
                break;
            case 2:
                auth.signUp();
                break;
            case 3: {
                std::cout << "Exiting... saving data\n";
                try {
                    students.saveToFile("data/students.txt", [](const Student &s){ return s.toString(); });
                    teachers.saveToFile("data/teachers.txt", [](const Teacher &t){ return t.toString(); });
                } catch (const std::exception &e) {
                    std::cout << "Warning: failed to save data: " << e.what() << "\n";
                }
                try { AuthManager::saveUsers(); } catch (...) {}
                _running = false;
                break;
            }
            default:
                break;
            }
            break;

        default:
            break;
        }
    } catch (const std::exception &e) {
        std::cout << "Error: " << e.what() << "\n";
    }
}

CLIManager::CLIManager()
{
    _curentState = START;
    _running = true;
    // load data files if present
    try {
        students.loadFromFile("data/students.txt", [](const std::string &s){ return Student::fromString(s); });
    } catch (...) {}
    try {
        teachers.loadFromFile("data/teachers.txt", [](const std::string &s){ return Teacher::fromString(s); });
    } catch (...) {}
}

int CLIManager::getOption()
{
    int option;
    if (!(std::cin >> option)) {
        std::cin.clear();
        std::string dummy;
        std::getline(std::cin, dummy);
        return -1;
    }
    return option;
}

bool CLIManager::isRunning() const {
    return _running;
}

void CLIManager::adminMenu() {
    while (true) {
        std::cout << "\n--- ADMIN MENU ---\n";
        std::cout << "1 - List students\n";
        std::cout << "2 - List teachers\n";
        std::cout << "3 - Create student record\n";
        std::cout << "4 - Create teacher record\n";
        std::cout << "5 - Save data\n";
        std::cout << "6 - Sort students by average\n";
        std::cout << "7 - Sign out\n";
        std::cout << "8 - Show all people (polymorphic view)\n";
        std::cout << "9 - List unique subjects\n";
        int opt = getOption();
        switch (opt) {
            case 1: {
                auto &all = students.getAll();
                if (all.empty()) std::cout << "(no students)\n";
                for (auto it = all.begin(); it != all.end(); ++it) it->display();
                break;
            }
            case 2: {
                auto &all = teachers.getAll();
                if (all.empty()) std::cout << "(no teachers)\n";
                for (auto it = all.begin(); it != all.end(); ++it) {
                    it->display();
                    if (!it->getSubjects().empty()) {
                        std::cout << "Subjects: ";
                        printRange(it->getSubjects().begin(), it->getSubjects().end(), ", ");
                    }
                }
                break;
            }
            case 3: {
                std::string login, name, surname, pesel;
                int d,m,y;
                std::cout << "Login: "; std::cin >> login;
                std::cout << "Name: "; std::cin >> std::ws; std::getline(std::cin, name);
                std::cout << "Surname: "; std::getline(std::cin, surname);
                std::cout << "Day: "; std::cin >> d;
                std::cout << "Month: "; std::cin >> m;
                std::cout << "Year: "; std::cin >> y;
                std::cout << "PESEL: "; std::cin >> pesel;
                Student s(login, name, surname, d, m, y, pesel);
                students.add(s);
                std::cout << "Student record created. To create account for this user, use Sign up.\n";
                break;
            }
            case 4: {
                std::string login, name, surname, pesel;
                int d,m,y;
                std::cout << "Login: "; std::cin >> login;
                std::cout << "Name: "; std::cin >> std::ws; std::getline(std::cin, name);
                std::cout << "Surname: "; std::getline(std::cin, surname);
                std::cout << "Day: "; std::cin >> d;
                std::cout << "Month: "; std::cin >> m;
                std::cout << "Year: "; std::cin >> y;
                std::cout << "PESEL: "; std::cin >> pesel;
                Teacher t(login, name, surname, d, m, y, pesel);
                teachers.add(t);
                std::cout << "Teacher record created. To create account for this user, use Sign up.\n";
                break;
            }
            case 5: {
                try {
                    students.saveToFile("data/students.txt", [](const Student &s){ return s.toString(); });
                    teachers.saveToFile("data/teachers.txt", [](const Teacher &t){ return t.toString(); });
                    std::cout << "Data saved.\n";
                } catch (const std::exception &e) {
                    std::cout << "Save failed: " << e.what() << "\n";
                }
                break;
            }
            case 6: {
                // sort students by average (descending)
                students.sortBy([](const Student &a, const Student &b){ return a.average() > b.average(); });
                std::cout << "Students sorted by average.\n";
                break;
            }
            case 7:
                return; // sign out
            case 8: {
                // polymorphic view using base class pointers
                std::vector<Person*> people;
                for (auto it = students.getAll().begin(); it != students.getAll().end(); ++it) {
                    Student *s = new Student(*it);
                    people.push_back(s);
                }
                for (auto it = teachers.getAll().begin(); it != teachers.getAll().end(); ++it) {
                    Teacher *t = new Teacher(*it);
                    people.push_back(t);
                }
                for (auto it = people.begin(); it != people.end(); ++it) {
                    (*it)->display();
                }
                // cleanup
                for (auto it = people.begin(); it != people.end(); ++it) delete *it;
                break;
            }
            case 9: {
                std::set<std::string> uniq;
                for (auto it = teachers.getAll().begin(); it != teachers.getAll().end(); ++it) {
                    for (auto sit = it->getSubjects().begin(); sit != it->getSubjects().end(); ++sit)
                        uniq.insert(*sit);
                }
                if (uniq.empty()) {
                    std::cout << "(no subjects)\n";
                } else {
                    std::cout << "Subjects:\n";
                    for (auto it = uniq.begin(); it != uniq.end(); ++it) std::cout << "  " << *it << "\n";
                }
                break;
            }
            default:
                break;
        }
    }
}

void CLIManager::teacherMenu() {
    // find teacher object by current login
    auto it = teachers.find_if([this](const Teacher &t){ return t.getLogin() == _currentUser.getLogin(); });
    if (it == teachers.getAll().end()) {
        std::cout << "Teacher data not found.\n";
        return;
    }
    Teacher &me = *it;

    while (true) {
        std::cout << "\n--- TEACHER MENU ---\n";
        std::cout << "1 - View my info (no PESEL)\n";
        std::cout << "2 - Assign grade to a student\n";
        std::cout << "3 - Sign out\n";
        int opt = getOption();
        switch (opt) {
            case 1: {
                // Display without PESEL
                cout << "Name: " << me.getName() << " " << me.getSurname() << "\n";
                cout << "Date of birth: " << me.getDay() << "." << me.getMonth() << "." << me.getYear() << "\n";
                break;
            }
            case 2: {
                std::string studentLogin, subject;
                double value;
                std::cout << "Student login: "; std::cin >> studentLogin;
                auto sit = students.find_if([&](const Student &s){ return s.getLogin() == studentLogin; });
                if (sit == students.getAll().end()) { std::cout << "Student not found\n"; break; }
                std::cout << "Subject: "; std::cin >> std::ws; std::getline(std::cin, subject);
                std::cout << "Grade value: "; std::cin >> value;
                me.assignGrade(*sit, subject, value);
                std::cout << "Grade assigned.\n";
                break;
            }
            case 3:
                return;
            default:
                break;
        }
    }
}

void CLIManager::studentMenu() {
    // find student by login
    auto it = students.find_if([this](const Student &s){ return s.getLogin() == _currentUser.getLogin(); });
    if (it == students.getAll().end()) {
        std::cout << "Student data not found.\n";
        return;
    }
    Student &me = *it;
    while (true) {
        std::cout << "\n--- STUDENT MENU ---\n";
        std::cout << "1 - View my info and grades\n";
        std::cout << "2 - Sign out\n";
        int opt = getOption();
        switch (opt) {
            case 1:
                me.display();
                break;
            case 2:
                return;
            default:
                break;
        }
    }
}