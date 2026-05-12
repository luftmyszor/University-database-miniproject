#pragma once
#include <bits/stdc++.h>

class Account
{
private:
    enum types
    {
        ADMIN,
        TEACHER,
        STUDENT
    };
    int _type;
    std::string _login;
    std::string _passwordHash;

public:
    std::string getPasswordHash();
    std::string getLogin();
    static std::string hash(std::string);
    Account(int type, std::string login, std::string password);
    Account();
    int getType();
    std::string getTypeStr();
};