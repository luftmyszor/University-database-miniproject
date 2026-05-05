#pragma once
#include <bits/stdc++.h>

class AuthManager
{
private:
    static bool isLoggedIn;
    static std::string hashString();

public:
    static bool signIn();
    static bool signUp();
};