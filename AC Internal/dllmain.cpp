#include "pch.h"
#include <iostream>
#include <Windows.h>
#include "classes.h"
#include "offsets.h"


LPCWSTR moduleName = L"ac_client.exe";
DWORD moduleBase = (DWORD)GetModuleHandle(moduleName);

int isAimingEnemy;
bool wasFiring = false;
uintptr_t originalReturnAddress, originalCallAddress;
INPUT input;

bool bHealth, bShield, bMagnet, bAmmo, bTrigger;

__declspec(naked) void triggerBotCodeCave() {
	originalCallAddress = moduleBase + dwDisplayNametagOriginalCall;
	originalReturnAddress = moduleBase + dwDisplayNametagReturn;

	__asm {
		call originalCallAddress;
		pushad
		mov isAimingEnemy, eax
	} 

	if (bTrigger) {
		input = { 0 };

		if (isAimingEnemy) {
			input.type = INPUT_MOUSE;
			input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
			SendInput(1, &input, sizeof(INPUT));
			wasFiring = true;
		}
		else if(wasFiring) {
			input.type = INPUT_MOUSE;
			input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
			SendInput(1, &input, sizeof(INPUT));
			wasFiring = false;
		}
	}

	__asm {
		popad
		jmp originalReturnAddress
	}
}


void hookDisplayNametags() {
	unsigned char* triggerHookLocation = (unsigned char*)(dwDisplayNametag + moduleBase);
	DWORD old;
	VirtualProtect((void*)triggerHookLocation, 5, PAGE_EXECUTE_READWRITE, &old);

	// jmp opcode
	*triggerHookLocation = 0xe9;
	// code cave address (new_location - original_location + 5)
	*(DWORD*)(triggerHookLocation + 1) = (DWORD)&triggerBotCodeCave - ((DWORD)triggerHookLocation + 5);
}

void unhookDisplayNametags() {
	unsigned char* triggerHookLocation = (unsigned char*)(dwDisplayNametag + moduleBase);
	
	*triggerHookLocation = 0xe8;
	*(unsigned char*)(triggerHookLocation + 1) = 0x1e;
	*(unsigned char*)(triggerHookLocation + 2) = 0x5a;
	*(unsigned char*)(triggerHookLocation + 3) = 0x05;
	*(unsigned char*)(triggerHookLocation + 4) = 0x00;
}

void hackThread(HMODULE hModule) {
	if (!moduleBase) {
		MessageBox(0, L"Module not found!", L"Warning", MB_OK | MB_ICONWARNING);
		return;
	}

	#ifdef _DEBUG
	AllocConsole();
	FILE* f;
	freopen_s(&f, "CONOUT$", "w", stdout);
	#endif // DEBUG

	bTrigger = bHealth = bShield = bMagnet = bAmmo = true;
	
	hookDisplayNametags();
	
	while (true)
	{
		Sleep(100);

		if (GetAsyncKeyState(VK_END)) {
			break;
		}

		uintptr_t ptrLocalPlayer = *(uintptr_t*)(moduleBase + dwLocalPlayer);

		if (ptrLocalPlayer == NULL) {
			continue;
		}

		Player* me = (Player*)ptrLocalPlayer;

		if (bHealth) {
			me->health = 1337;
		}

		if (bShield) {
			me->shield = 1337;
		}

		if (bAmmo) {
			me->currentWeapon->clip->ammo = 1337;
		}

		int iEntityListLength = *(int*)(moduleBase + dwEntityListLength);

		if (iEntityListLength < 1) {
			continue;
		}

		uintptr_t ptrEntityList = *(uintptr_t*)(moduleBase + dwEntityList);

		for (size_t i = 1; i < iEntityListLength; i++)
		{
			uintptr_t ptrEntity = *(uintptr_t*)(ptrEntityList + (i * 4));
			Player* player = (Player*)ptrEntity;

			if (!player || !me->isEnemy(player)) {
				continue;
			}

			if (bMagnet) {
				player->position.x = me->position.x + 5;
				player->position.y = me->position.y + 5;
				player->position.z = me->position.z;
			}
		}
	}

	#ifdef _DEBUG
	if (f != NULL) {
		fclose(f);
	}

	FreeConsole();
	#endif // DEBUG
	
	FreeLibraryAndExitThread(hModule, 0);
}


BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		Beep(500, 500);
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)hackThread, hModule, 0, 0);
		break;

	case DLL_PROCESS_DETACH:
		if (lpReserved != nullptr)
		{
			break; // do not do cleanup if process termination scenario
		}

		unhookDisplayNametags();

		Beep(500, 250);
		Beep(500, 250);

		break;
	}
	return TRUE;
}
