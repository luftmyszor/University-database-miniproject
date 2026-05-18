#include <bits/stdc++.h>
// main.cpp - program entry
// Purpose: construct `CLIManager` and run the interactive CLI loop.

#include "src/CLIManager.h"
#include "src/AuthManager.h"

int main()
{
    CLIManager CLI = CLIManager();
    while (CLI.isRunning())
        CLI.CLI();
}

// CLI - Command Line Interface