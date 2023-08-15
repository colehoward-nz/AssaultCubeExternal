#pragma once

#include <Windows.h>
#include <TlHelp32.h>

namespace mem
{
	void PatchEx(BYTE* destination, BYTE* source, unsigned int size, HANDLE hProcess);
	void NopEx(BYTE* destination, unsigned int size, HANDLE hProcess);
}
