#pragma once
#include <bits/stdc++.h>
class CLIManager
{
private:
    enum states
    {
        START,
        SIGNUP,
        SIGNIN,
    };

public:
    std::vector<std::string> getVarArgsFromConsole();
    void CLI(int state);
};