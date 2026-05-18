#include "Account.h"
// Account.cpp
// Purpose: Account implementation including simple password hashing helper.

Account::Account(int type, std::string login, std::string password)
{
    switch (type)
    {
    case ADMIN:
        _type = ADMIN;
        break;
    case TEACHER:
        _type = TEACHER;
        break;
    case STUDENT:
        _type = STUDENT;
        break;
    default:
        break;
    }

    _login = login;
    _passwordHash = hash(password);
}

Account::Account(int type, std::string login, std::string password, bool isHash)
{
    switch (type)
    {
    case ADMIN:
        _type = ADMIN;
        break;
    case TEACHER:
        _type = TEACHER;
        break;
    case STUDENT:
        _type = STUDENT;
        break;
    default:
        break;
    }

    _login = login;
    if (isHash)
        _passwordHash = password;
    else
        _passwordHash = hash(password);
}

std::string Account::getLogin() const { return _login; }
std::string Account::getPasswordHash() const { return _passwordHash; }
int Account::getType() const { return _type; };

std::string Account::getTypeStr() const
{
    switch (getType())
    {
    case 0:
        return "ADMIN";
    case 1:
        return "TEACHER";
    case 2:
        return "STUDENT";
    }
    return "UNKNOWN";
}

std::string Account::hash(std::string str)
{
    const unsigned long long p = 31;
    const unsigned long long m = 1e9 + 9;
    unsigned long long hash_value = 0;
    unsigned long long p_pow = 1;
    for (char c : str)
    {
        hash_value = (hash_value + (c - 'a' + 1) * p_pow) % m;
        p_pow = (p_pow * p) % m;
    }
    std::stringstream ss;
    ss << std::hex << hash_value;
    return ss.str();
}

Account::Account()
{
    _type = STUDENT;
    _login = "";
    _passwordHash = "";
}