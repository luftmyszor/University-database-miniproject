#include "AuthManager.h"

#include "DataManager.h"

// static storage
std::unordered_map<std::string, Account> AuthManager::_users;
const std::string AuthManager::ACCOUNTS_FILE = "data/accounts.txt";

std::string AuthManager::hashString() {
    return std::to_string(std::chrono::system_clock::now().time_since_epoch().count());
}

void AuthManager::loadUsers() {
    try {
        auto lines = DataManager::readFromFile(ACCOUNTS_FILE);
        for (const auto &l : lines) {
            if (l.empty()) continue;
            std::istringstream iss(l);
            std::string token;
            std::vector<std::string> parts;
            while (std::getline(iss, token, ';')) parts.push_back(token);
            if (parts.size() >= 3) {
                int type = std::stoi(parts[0]);
                std::string login = parts[1];
                std::string passhash = parts[2];
                _users.emplace(login, Account(type, login, passhash, true));
            }
        }
    } catch (...) {
        // ignore load errors
    }
    // ensure at least one admin exists
    if (_users.empty()) {
        _users.emplace(std::string("admin"), Account(0, std::string("admin"), std::string("admin")));
        saveUsers();
    }
}

void AuthManager::saveUsers() {
    std::vector<std::string> lines;
    for (const auto &p : _users) {
        const Account &a = p.second;
        std::ostringstream oss;
        oss << a.getType() << ";" << a.getLogin() << ";" << a.getPasswordHash();
        lines.push_back(oss.str());
    }
    try {
        DataManager::saveToFile(ACCOUNTS_FILE, lines);
    } catch (...) {
        // ignore save errors
    }
}

AuthManager::AuthManager() {
    loadUsers();
}

AuthManager::~AuthManager() {
    saveUsers();
}

bool AuthManager::signUp()
{
    std::cout << "Login :";
    std::string login;
    std::cin >> login;

    if (doesUserExist(login))
    {
        std::cout << "User already exists\n";
        return false;
    }

    std::cout << "Password :";
    std::string password;
    std::cin >> password;

    std::cout << "Account type | 1-admin | 2-teacher | 3-student |:";
    int type;
    std::cin >> type;

    // normalize input (1->ADMIN, 2->TEACHER, 3->STUDENT)
    if (type < 1 || type > 3) type = 3;
    type = type - 1;

    _users.emplace(login, Account(type, login, password));
    saveUsers();
    std::cout << "User successfully created\n";
    return true;
}

Account AuthManager::signIn()
{
    std::cout << "Login :";
    std::string login;
    std::cin >> login;

    if (!doesUserExist(login))
    {
        std::cout << "User doesn't exists\n";
        throw std::runtime_error("User doesn't exist");
    }

    Account user = _users[login];

    std::cout << "Password :";
    std::string password;
    std::cin >> password;

    if (user.getPasswordHash() != Account::hash(password))
    {
        std::cout << "Incorrect password\n";
        throw std::runtime_error("Incorrect password");
    }

    std::cout << "Signed in as " << login << " " << user.getTypeStr() << "\n";
    return user;
}

bool AuthManager::doesUserExist(std::string login)
{
    return (_users.find(login) != _users.end());
}