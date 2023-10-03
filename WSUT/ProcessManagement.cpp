// ProcessManagement.cpp

#include "ProcessManagement.h"
#include <psapi.h>
#include <iostream>

#define MAX_PROCESSES 1024

void ProcessManagement::addProc(DWORD processID, std::map<int, std::string>& procs, std::vector<int>& pIDs) {
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
                pIDs.push_back(processID);
                procs[processID] = pn;
            }
        }
    }

    // Release the handle to the process
    CloseHandle(hProcess);

}

BOOL ProcessManagement::getProcs(std::vector<int>& pIDs, std::map<int, std::string>& procs) {
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
            addProc(aProcesses[i], procs, pIDs);
        }
    }

    return TRUE;
}

void ClearConsole() {
    std::cout << "\033c"; // ANSI escape code to reset the console
}

int ProcessManagement::ProcUpdater() {
    std::map<int, std::string> previousProcesses;  // store the previous list of processes

    while (true) {
        if (GetKeyState('X') & 0x8000){
            return 0;
        }

        std::map<int, std::string> processes;
        std::vector<int> processIDs;

        BOOL success = ProcessManagement::getProcs(processIDs, processes);

        if (success) {
            // only clear the console if there's a change in the list of processes
            if (processes != previousProcesses) {
                ClearConsole();

                // update the previousProcesses with the current processes.
                previousProcesses = processes;

                // iterate through the map to display process IDs and names
                for (const auto& entry : processes) {
                    std::cout << "Process ID: " << entry.first << "\tProcess Name: " << entry.second << std::endl;
                }
            }
        }
        else {
            std::cout << "Failed to retrieve the list of processes\n";
        }

        Sleep(1000);
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
