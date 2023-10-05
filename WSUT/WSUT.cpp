#include "ProcessManagement.h"
#include <iostream>
#include <map>
#include <vector>
#include <Windows.h>
#include <string>

#define ASCII_ART " _      __  ____  __  __  ______\n" "| | /| / / / __/ / / / / /_  __/\n" "| |/ |/ / _\\ \\  / /_/ /   / /\n" "|__/|__/ /___/  \\____/   /_/\n\nWindows System Utility Tool by Daanzzz\n\n";

// program commands
#define PROCESS_MANAGEMENT_COMMAND "pm"
#define TERMINATE_PROCESS "tp"
#define HELP_COMMAND "help"
#define EXIT_COMMAND "exit"

void guide() {
    std::cout << "\nGuide:\npm - process management\ntp - terminate a process\nhelp - shows the guide\nexit - exits the program\n\n*Note* - in order to go back to the menu hold x\n\n";
}

int main(void) {
    std::cout << ASCII_ART;

    guide();
    std::string choice = "";

    while (choice != EXIT_COMMAND) {
        choice = ProcessManagement::GetInput();

        if (choice == PROCESS_MANAGEMENT_COMMAND) {
            ProcessManagement::ProcUpdater();
            std::cout << "\033c"; // ANSI escape code to reset the console
        }
        else if (choice == HELP_COMMAND) {
            guide();
        }
        else if (choice == EXIT_COMMAND) {
            std::cout << "Goodbye.\n";
        }
        else if (choice == TERMINATE_PROCESS) {
            std::cout << "enter process ID: ";
            int pID = 0;
            std::cin >> pID;

            if (!ProcessManagement::EndProcess(pID)) {
                std::cout << "Error: didn't terminate process!\n";
            }
            else {
                std::cout << "Process terminated\n";
            }
        }
        else {
            std::cout << "Unkown command.\n";
        }
    }

    return 0;
}
