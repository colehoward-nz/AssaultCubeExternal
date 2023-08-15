#include "mem.h"

void mem::PatchEx(BYTE* destination, BYTE* source, unsigned int size, HANDLE hProcess)
{
	DWORD oldprotect;
	VirtualProtectEx(hProcess, destination, size, PAGE_EXECUTE_READWRITE, &oldprotect);
	WriteProcessMemory(hProcess, destination, source, size, nullptr);
	VirtualProtectEx(hProcess, destination, size, oldprotect, &oldprotect);
}

void mem::NopEx(BYTE* destination, unsigned int size, HANDLE hProcess)
{
	BYTE* nopArray = new BYTE[size];
	memset(nopArray, 0x90, size);
	
	PatchEx(destination, nopArray, size, hProcess);
	delete[] nopArray;
}
