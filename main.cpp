#include <bits/stdc++.h>
#include "src/CLIManager.h"

int main()
{
    CLIManager CLI = CLIManager();
    while (CLI.isRunning())
        CLI.CLI();
}

// CLI - Command Line Interface