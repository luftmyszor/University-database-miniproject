#pragma once
// Account.h
// Purpose: small model for user accounts (type, login, password hash)
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
    std::string getPasswordHash() const;
    std::string getLogin() const;
    static std::string hash(std::string);
    Account(int type, std::string login, std::string password);
    // construct with already-hashed password when `isHash` is true
    Account(int type, std::string login, std::string password, bool isHash);
    Account();
    int getType() const;
    std::string getTypeStr() const;
    
};