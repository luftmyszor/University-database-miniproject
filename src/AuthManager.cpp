#include "AuthManager.h"

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

    _users.emplace(login, Account(type, login, password));
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
        throw("User doesn't exist");
    }

    Account user = _users[login];

    std::cout << "Password :";
    std::string password;
    std::cin >> password;

    if (user.getPasswordHash() != Account::hash(password))
    {
        std::cout << "Incorrect password\n";
        throw("Incorrect password");
    }

    std::cout << "Signed in as " << login << " " << user.getTypeStr() << "\n";
    return user;
}

bool AuthManager::doesUserExist(std::string login)
{
    return (_users.find(login) != _users.end());
}