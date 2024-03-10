#include "pch.h"
#include <iostream>
#include <Windows.h>
#include "classes.h"
#include "offsets.h"
#include <math.h>
#include "functions.h"
#include "Memory.h"
#include "opengl.h"
#include "Menu.h"

bool isRecoilPatched = false;

LPCWSTR moduleName = L"ac_client.exe";
DWORD moduleBase = (DWORD)GetModuleHandle(moduleName);

int isAimingEnemy;
bool wasFiring, bTrigger = false;


uintptr_t originalReturnAddress, originalCallAddress;
INPUT input;

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
		else if (wasFiring) {
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

	bTrigger = false;

	Menu::Init();
	OpenGL::Hook();

	while (true)
	{
		if (GetAsyncKeyState(VK_END)) {
			OpenGL::UnHook();
			Menu::End();
			if (isRecoilPatched) Memory::PatchBytes(dwRecoilInstruction, recoilOriginalBytes);
			break;
		}

		uintptr_t ptrLocalPlayer = *(uintptr_t*)(moduleBase + dwLocalPlayer);
		if (ptrLocalPlayer == NULL) {
			continue;
		}

		Player* me = (Player*)ptrLocalPlayer;

		if (Menu::Options::Weapon::noRecoil && !isRecoilPatched) {
			Memory::PatchBytes(dwRecoilInstruction, recoilPatchBytes);
			isRecoilPatched = true;
		}
		else if (!Menu::Options::Weapon::noRecoil && isRecoilPatched) {
			Memory::PatchBytes(dwRecoilInstruction, recoilOriginalBytes);
			isRecoilPatched = false;
		}

		if (Menu::Options::Player::godMode) {
			me->health = 1337;
			me->shield = 1337;
		}

		if (Menu::Options::Weapon::unlimitedAmmo) {
			me->currentWeapon->currentMagazine->ammo = 1337;
			me->currentWeapon->magazines->ammo = 1337;
			me->grenades = 1337;
		}

		if (Menu::Options::Aimbot::aimbot || Menu::Options::Misc::magnet) {
			int playersCount = *(int*)(moduleBase + dwEntityListLength);

			if (playersCount < 1) {
				continue;
			}

			uintptr_t ptrEntityList = *(uintptr_t*)(moduleBase + dwEntityList);

			float closestPlayer = -1.0;
			float closestYaw = 0;
			float closestPitch = 0;

			for (size_t i = 1; i < playersCount; i++)
			{

				uintptr_t ptrEntity = *(uintptr_t*)(ptrEntityList + (i * 4));
				Player* player = (Player*)ptrEntity;

				if (!player || player->dead) {
					continue;
				}

				if (Menu::Options::Aimbot::aimbot) {
					// thanks https://gamehacking.academy/lesson/5/6
					float absPosX = player->position.x - me->position.x;
					float absPosY = player->position.y - me->position.y;

					float distance = euclidean_distance(absPosX, absPosY);

					if (closestPlayer == -1.0 || distance < closestPlayer) {
						closestPlayer = distance;

						float azimuthXY = atan2f(absPosY, absPosX);
						float yaw = azimuthXY * (180.0 / M_PI);

						if (absPosY < 0) {
							absPosY *= -1;
						}

						if (absPosY < 5) {
							if (absPosX < 0) {
								absPosX *= -1;
							}
							absPosY = absPosX;
						}

						float absPosZ = player->position.z - me->position.z;
						float azimuthZ = atan2f(absPosZ, absPosY);
						float pitch = azimuthZ * (180 / M_PI);

						closestYaw = yaw + 90;
						closestPitch = pitch;
					}
				}

				if (Menu::Options::Misc::magnet) {
					if (!Menu::Options::Misc::isPositioned) {
						Menu::Options::Misc::magnetPosX = me->position.x + 5;
						Menu::Options::Misc::magnetPosY = me->position.y + 5;
						Menu::Options::Misc::magnetPosZ = me->position.z;
					}

					player->position.x = Menu::Options::Misc::magnetPosX;
					player->position.y = Menu::Options::Misc::magnetPosY;
					player->position.z = Menu::Options::Misc::magnetPosZ;
				}
			}

			if (Menu::Options::Aimbot::aimbot) {
				me->angle.x = closestYaw;
				me->angle.y = closestPitch;
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

		//unhookDisplayNametags();

		Beep(500, 250);
		Beep(500, 250);

		break;
	}
	return TRUE;
}
