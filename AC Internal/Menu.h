#pragma once
#include "imgui_impl_opengl2.h"
#include <Windows.h>

namespace Menu {
	namespace Options {
		namespace Aimbot {
			extern bool aimbot;
			extern bool trigger;
		}
		namespace Player {
			extern bool godMode;
			extern bool unlimitedHealth;
			extern bool unlimitedShield;
		}
		namespace Weapon {
			extern bool noRecoil;
			extern bool unlimitedAmmo;
		}
		namespace Misc {
			extern bool magnet;

			extern bool isPositioned;
			extern float magnetPosX;
			extern float magnetPosY;
			extern float magnetPosZ;
		}
	}

	LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	void Init();
	void Draw();
	void End();
	bool Toggle();
}