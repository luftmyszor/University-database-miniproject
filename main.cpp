#include <bits/stdc++.h>
#include "src/CLIManager.h"
#include "src/AuthManager.h"

int main()
{
    CLIManager CLI = CLIManager();
    while (CLI.isRunning())
        CLI.CLI();
}

// CLI - Command Line Interface