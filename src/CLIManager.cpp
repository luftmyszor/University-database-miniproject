#include "CLIManager.h"
void CLIManager::CLI()
{

    switch (_curentState)
    {
    case START:
        std::cout << "1 - sign in\n";
        std::cout << "2 - sign up\n";
        switch (getOption())
        {
        case 1:
            _currentUser = auth.signIn();
            break;
        case 2:
            auth.signUp();
            break;
        default:
            break;
        }
        break;

    default:
        break;
    }
}
CLIManager::CLIManager()
{
    _curentState = START;
}

int CLIManager::getOption()
{
    int option;
    std::cin >> option;
    return option;
}