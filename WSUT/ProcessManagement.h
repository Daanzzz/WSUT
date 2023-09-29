// ProcessManagement.h
#pragma once

#include <Windows.h>
#include <map>
#include <string>
#include <vector>

class ProcessManagement {
public:
    // Adds a process to the provided maps and vector.
    // Parameters:
    // * processID - The ID of the process to add.
    // * procs - A map to store process IDs and their corresponding names.
    // * pIDs - A vector to store process IDs.
	static void addProc(DWORD processID, std::map<int, std::string>& procs, std::vector<int>& pIDs);

    // Retrieves a list of running process IDs and their names.
    // Parameters:
    // * pIDs - A vector to store the process IDs.
    // * procs - A map to store process IDs and their corresponding names.
    // Returns:
    // * Returns TRUE if the process enumeration is successful, FALSE otherwise.
	static BOOL getProcs(std::vector<int>& pIDs, std::map<int, std::string>& procs);
};
