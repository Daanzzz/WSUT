// ProcessManagement.cpp

#include "ProcessManagement.h"
#include <psapi.h>
#include <iostream>

#define MAX_PROCESSES 1024
#define MAX_PROCS_PER_PAGE 26

std::string ProcessManagement::GetInput(){
    std::string input = "";
    std::cout << "> ";
    std::cin >> input;

    return input;
}

void ProcessManagement::AddProc(DWORD processID, std::vector<std::pair<int, std::string>>& procs) {
    TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");

    // get a handle to the process

    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |
        PROCESS_ALL_ACCESS, // all access to the process is needed in order to read
        FALSE, processID);

    // get the process name
    if (NULL != hProcess) {
        HMODULE hMod;
        DWORD cbNeeded;

        if (EnumProcessModules(hProcess, &hMod, sizeof(hMod),
            &cbNeeded)) {
            if (GetModuleBaseName(hProcess, hMod, szProcessName,
                sizeof(szProcessName) / sizeof(TCHAR))) {

                // details should be printed only when GetModuleBaseName
                // didn't fail to get proccess name 
                std::wstring pnT(&szProcessName[0]); //convert to wstring
                std::string pn(pnT.begin(), pnT.end()); //and convert to string

                std::pair<int, std::string> temp = std::make_pair(processID, pn);
                procs.push_back(temp);
            }
        }
    }

    // Release the handle to the process
    CloseHandle(hProcess);

}

BOOL ProcessManagement::GetProcs(std::vector<std::pair<int, std::string>>& procs) {
    DWORD aProcesses[MAX_PROCESSES], cbNeeded, cProcesses;
    unsigned int i;

    if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded)) {
        return FALSE;
    }

    // calculate how many process identifiers were returned

    cProcesses = cbNeeded / sizeof(DWORD);

    // print the name and process identifier for each process

    for (i = 0; i < cProcesses; i++) {
        if (aProcesses[i] != 0) {
            // add each valid process to the map and vector
            AddProc(aProcesses[i], procs);
        }
    }

    return TRUE;
}

// Clears the console and then prints all the information titles (PID etc...)
void ClearConsole(int currentPage, int neededPages) {
    std::cout << "\033c"; // ANSI escape code to reset the console
    std::cout << "\x1B[31mPID\033[0m\t"; // PID in red color
    std::cout << "\x1B[32mPNAME\033[0m\t";
    printf("\033[2;47;35mPage %d/%d\033[0m", currentPage + 1, neededPages + 1);
    std::cout << "\n\x1B[31m---\033[0m\t";
    std::cout << "\x1B[32m-----\033[0m\t\n";
    // Clears the console and then prints all the information titles (PID etc...)
}

int ProcessManagement::ProcUpdater() {
    std::vector<std::pair<int, std::string>> previousProcesses;  // store the previous list of processes

    int currentPage = 0;
    int neededPages = previousProcesses.size() / MAX_PROCS_PER_PAGE;

    bool pageFlag = false;

    while (true) {
        std::vector<std::pair<int, std::string>> processes;
        BOOL success = ProcessManagement::GetProcs(processes);
        neededPages = processes.size() / MAX_PROCS_PER_PAGE;

        if (GetAsyncKeyState(0x58) & 0x8000) {
            return 0;
        }

        if (GetAsyncKeyState(0x46) & 0x8000) {
            if (currentPage < neededPages) {
                currentPage++;
                pageFlag = true;
            }
        }
        else if (GetAsyncKeyState(0x42) & 0x8000) {
            if (currentPage != 0) {
                currentPage--;
                pageFlag = true;
            }
        }

        if (success) {
            // only clear the console if there's a change in the list of processes
            if (processes != previousProcesses || pageFlag) {
                ClearConsole(currentPage, neededPages);

                // update the previousProcesses with the current processes.
                previousProcesses = processes;

                // iterate through the map to display process IDs and names
                int i = currentPage * MAX_PROCS_PER_PAGE;
                int LastIndex = i + MAX_PROCS_PER_PAGE;
                int pSize = processes.size();

                for (i; i < LastIndex && i < pSize; i++) {
                    std::cout << processes[i].first << "\t" << processes[i].second << std::endl;
                }

                pageFlag = false;
            }
        }
        else {
            std::cout << "Failed to retrieve the list of processes\n";
        }

        Sleep(100);
    }

    return 0;
}

int ProcessManagement::EndProcess(int pID) {
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |
        PROCESS_ALL_ACCESS, // all access to the process is needed in order to read
        FALSE, pID);

    if (hProcess == NULL) { // checking if OpenProcess has failed
        return 0;
    }
    UINT eCode = 1;

    BOOL ret = TerminateProcess(hProcess, 1);

    if (!ret) {
        return 0;
    }

    return 1;
}
