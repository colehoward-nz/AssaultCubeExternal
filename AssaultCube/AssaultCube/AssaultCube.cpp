// AssaultCube.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <Windows.h>
#include "proc.h"

int main()
{
    // Get processID
    DWORD processID = GetProcessId(L"ac_client.exe");

    // Get modulebaseAddress
    uintptr_t moduleBaseAddress = GetModuleBaseAddress(processID, L"ac_client.exe");

    // Get handle to process using OpenProcess
    HANDLE hProcess = 0;
    hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, processID);

    // Resolve base address of pointer chain
    uintptr_t dynamicPtrBaseAddress = moduleBaseAddress + 0x17E0A8;
    std::cout << "dynamicPtrBaseAddress = " << "0x" << std::hex << dynamicPtrBaseAddress << std::endl;



    // Resolve pointer chains
    std::vector<unsigned int> ammoOffsets = { 0x364, 0x14, 0x0 };
    std::vector<unsigned int> healthOffsets = { 0xEC };
    uintptr_t ammoAddress = FindDMAAddy(hProcess, dynamicPtrBaseAddress, ammoOffsets);
    uintptr_t healthAddress = FindDMAAddy(hProcess, dynamicPtrBaseAddress, healthOffsets);
    std::cout << "ammoAddress = " << "0x" << std::hex << ammoAddress << std::endl;
    std::cout << "healthAddress = " << "0x" << std::hex << healthAddress << std::endl;

    // Read values
    int ammoValue = 0;
    int healthValue = 0;
    ReadProcessMemory(hProcess, (BYTE*)ammoAddress, &ammoValue, sizeof(ammoValue), nullptr);
    ReadProcessMemory(hProcess, (BYTE*)healthAddress, &healthValue, sizeof(healthValue), nullptr);
    std::cout << "ammoValue = " << std::dec << ammoValue << std::endl;
    std::cout << "healthValue = " << std::dec << healthValue << std::endl;

    // Write to it
    int newAmmo = 1337;
    int newHealth = 1337;
    WriteProcessMemory(hProcess, (BYTE*)ammoAddress, &newAmmo, sizeof(newAmmo), nullptr);
    WriteProcessMemory(hProcess, (BYTE*)healthAddress, &newHealth, sizeof(newHealth), nullptr);

    // Read new values  
    ReadProcessMemory(hProcess, (BYTE*)ammoAddress, &ammoValue, sizeof(ammoValue), nullptr);
    ReadProcessMemory(hProcess, (BYTE*)healthAddress, &healthValue, sizeof(healthValue), nullptr);
    std::cout << "ammoValue = " << std::dec << ammoValue << std::endl;
    std::cout << "healthValue = " << std::dec << healthValue << std::endl;

    getchar();
    return 0;
}

