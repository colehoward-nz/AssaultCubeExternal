// AssaultCube.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <Windows.h>
#include "proc.h"
#include "mem.h"

int main()
{
    HANDLE hProcess = 0;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD processID = GetProcessId(L"ac_client.exe");

    uintptr_t moduleBaseAddress = 0, localPlayerPointer = 0, healthAddress = 0, ammoAddress = 0;
    bool bHealth = false, bAmmo = false, bRecoil = false;
    const int newValue = 1337;

    if (processID)
    {
        hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, processID);
        moduleBaseAddress = GetModuleBaseAddress(processID, L"ac_client.exe");
        localPlayerPointer = moduleBaseAddress + 0x0017F110;
        healthAddress = FindDMAAddy(hProcess, localPlayerPointer, { 0x0, 0xEC });
        ammoAddress = FindDMAAddy(hProcess, localPlayerPointer, { 0x364, 0x14, 0x0 });
    }
    else
    {
        std::cout << "Process not found, press <enter> to exit\n";
        getchar();
        return 0;
    }


    DWORD dwExit = 0;
    while (GetExitCodeProcess(hProcess, &dwExit) && dwExit == STILL_ACTIVE)
    {
        // Toggle continous write of 1337 health
        if (GetAsyncKeyState(VK_F1) & 1)
        {
            bHealth = !bHealth;
            if (bHealth)
            {
                SetConsoleTextAttribute(hConsole, 2);
                std::cout << "[toggle] infinte health: " << "ENABLED\n";
            }
            else
            {
                SetConsoleTextAttribute(hConsole, 4);
                std::cout << "[toggle] infinte health: " << "DISABLED\n";
            }
        }

        // Toggle inc/dec ammo hack
        if (GetAsyncKeyState(VK_F2) & 1)
        {
            bAmmo = !bAmmo;
            if (bAmmo)
            {
                mem::PatchEx((BYTE*)(moduleBaseAddress + 0xC73EF), (BYTE*)"\x90\x40", 2, hProcess);
                SetConsoleTextAttribute(hConsole, 2);
                std::cout << "[toggle] increasing ammo: " << "ENABLED\n";
            }
            else
            {
                mem::PatchEx((BYTE*)(moduleBaseAddress + 0xC73EF), (BYTE*)"\xFF\x08", 2, hProcess);
                SetConsoleTextAttribute(hConsole, 4);
                std::cout << "[toggle] increasing ammo: " << "DISABLED\n";
            }
        }

        // Exit and clean up
        if (GetAsyncKeyState(VK_INSERT) & 1)
        {
            return 0;
        }



        // Continous Write
        if (bHealth)
        {
            mem::PatchEx((BYTE*)healthAddress, (BYTE*)&newValue, sizeof(newValue), hProcess);
        }

        Sleep(20);
    }
    std::cout << "Process not found, press <enter> to exit.";
    getchar();
    return 0;
}

