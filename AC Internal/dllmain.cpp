#include "pch.h"
#include <iostream>
#include <Windows.h>
#include "classes.h"
#include "offsets.h"


LPCWSTR moduleName = L"ac_client.exe";

void hackThread(HMODULE hModule) {
	AllocConsole();
	FILE* f;
	freopen_s(&f, "CONOUT$", "w", stdout);

	DWORD moduleBase = (DWORD)GetModuleHandle(moduleName);

	if (!moduleBase) {
		std::cout << moduleName << " not found!" << std::endl;
		return;
	}

	bool bHealth, bShield, bMagnet, bAmmo;
	bHealth = bShield = bMagnet = bAmmo = false;

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

	if (f != NULL) {
		fclose(f);
	}

	FreeConsole();
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

		Beep(500, 250);
		Beep(500, 250);

		break;
	}
	return TRUE;
}
