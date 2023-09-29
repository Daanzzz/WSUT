// ProcessManagement.cpp

#include "ProcessManagement.h"
#include <psapi.h>

#define MAX_PROCESSES 1024

void ProcessManagement::addProc(DWORD processID, std::map<int, std::string>& procs, std::vector<int>& pIDs) {
    TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");

    // Get a handle to the process.

    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |
        PROCESS_ALL_ACCESS, // All access to the process is needed in order to read
        FALSE, processID);

    // Get the process name.
    if (NULL != hProcess) {
        HMODULE hMod;
        DWORD cbNeeded;

        if (EnumProcessModules(hProcess, &hMod, sizeof(hMod),
            &cbNeeded)) {
            if (GetModuleBaseName(hProcess, hMod, szProcessName,
                sizeof(szProcessName) / sizeof(TCHAR))) {

                // Details should be printed only when GetModuleBaseName
                // didn't fail to get proccess name 
                std::wstring pnT(&szProcessName[0]); //convert to wstring
                std::string pn(pnT.begin(), pnT.end()); //and convert to string.
                pIDs.push_back(processID);
                procs[processID] = pn;
            }
        }
    }

    // Release the handle to the process.
    CloseHandle(hProcess);

}

BOOL ProcessManagement::getProcs(std::vector<int>& pIDs, std::map<int, std::string>& procs) {
    DWORD aProcesses[MAX_PROCESSES], cbNeeded, cProcesses;
    unsigned int i;

    if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded)) {
        return FALSE;
    }

    // Calculate how many process identifiers were returned.

    cProcesses = cbNeeded / sizeof(DWORD);

    // Print the name and process identifier for each process.

    for (i = 0; i < cProcesses; i++) {
        if (aProcesses[i] != 0) {
            // Add each valid process to the map and vector.
            addProc(aProcesses[i], procs, pIDs);
        }
    }

    return TRUE;
}