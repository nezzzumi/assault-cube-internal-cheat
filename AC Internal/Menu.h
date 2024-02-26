#pragma once
#include "imgui_impl_opengl2.h"
#include <Windows.h>

namespace Menu {
	LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	void Init();
	void Draw();
	void End();
	bool Toggle();
}