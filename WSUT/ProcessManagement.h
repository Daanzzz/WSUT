// ProcessManagement.h
#pragma once

#include <Windows.h>
#include <map>
#include <string>
#include <vector>

class ProcessManagement {
public:
    // Adds a process to the provided maps and vector
    // Parameters:
    // * processID - The ID of the process to add
    // * procs - A map to store process IDs and their corresponding names
    // * pIDs - A vector to store process IDs
    static void AddProc(DWORD processID, std::vector<std::pair<int, std::string>>& procs);

    // Retrieves a list of running process IDs and their names
    // Parameters:
    // * pIDs - A vector to store the process IDs
    // * procs - A map to store process IDs and their corresponding names
    // Returns
    // * Returns TRUE if the process enumeration is successful, FALSE otherwise
    static BOOL GetProcs(std::vector<std::pair<int, std::string>>& procs);
    
    // Constantly updates the process update (when needed)
    // * Returns 0 any time
    static int ProcUpdater();

    // Terminates a process
    // Parameters:
    // * pID - The ID of the process to terminate
    static int EndProcess(int pID);

    
};
