#pragma once
#include <bits/stdc++.h>
#include "Account.h"

class AuthManager
{
private:
    static std::string hashString();
    static std::unordered_map<std::string, Account> _users;

public:
    static const std::string ACCOUNTS_FILE;
    static void loadUsers();
    static void saveUsers();

    AuthManager();
    ~AuthManager();

    Account signIn();
    bool signUp();

    bool doesUserExist(std::string login);
};