#pragma once
#include <Windows.h>

namespace Memory {
	DWORD Protect(DWORD address, int size, DWORD permission);
	
	template<typename T>
	void Write(DWORD address, T value);

	DWORD CodeCave(DWORD address, DWORD function, int size);
}