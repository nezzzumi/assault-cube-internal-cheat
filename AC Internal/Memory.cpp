#pragma once
#include <Windows.h>


namespace Memory {
	DWORD Protect(DWORD address, int size, DWORD permission) {
		DWORD old;

		VirtualProtect((LPVOID)address, size, permission, &old);

		return old;
	}
	
	template<typename T>
	void Write(DWORD address, T value) {
		*((T*)address) = value;
	}

	DWORD CodeCave(DWORD address, DWORD function, int size) {
		DWORD old = Memory::Protect(address, size, PAGE_EXECUTE_READWRITE);
		
		Memory::Write<BYTE>(address, 0xe9); // jmp opcode
		Memory::Write<DWORD>(address + 0x1, function - (address + (DWORD)5));

		return old;
	}
}