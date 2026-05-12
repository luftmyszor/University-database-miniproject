#pragma once
#include <bits/stdc++.h>
#include "Account.h"

class AuthManager
{
private:
    static std::string hashString();
    static std::unordered_map<std::string, Account> _users;

public:
    Account signIn();
    bool signUp();

    bool doesUserExist(std::string login);
};