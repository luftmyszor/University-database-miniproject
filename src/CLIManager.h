#pragma once
#include <bits/stdc++.h>
#include "AuthManager.h"
#include "Account.h"
class CLIManager
{
private:
    enum states
    {
        START,
        SIGNUP,
        SIGNIN,
    };
    int _curentState;
    Account _currentUser;

public:
    AuthManager auth = AuthManager();
    CLIManager();
    void CLI();
    int getOption();
};