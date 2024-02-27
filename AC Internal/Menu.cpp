#pragma once

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_opengl2.h"
#include <Windows.h>
#include <iostream>

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace Menu {
	bool noRecoil = true;
	bool isOpened = true;
	HWND gameHwnd = FindWindowA(NULL, "AssaultCube");
	LONG_PTR originalWndProc;

	typedef int(__cdecl* SDL_SetRelativeMouseMode)(bool enabled);
	SDL_SetRelativeMouseMode _SDL_SetRelativeMouseMode = (SDL_SetRelativeMouseMode)GetProcAddress(GetModuleHandle(L"SDL2.dll"), "SDL_SetRelativeMouseMode");
	

	bool Toggle() {
		Menu::isOpened = !Menu::isOpened;

		return Menu::isOpened;
	}

	LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{

		if (msg == WM_KEYDOWN && wParam == VK_INSERT) {
			Menu::Toggle();

			return false;
		}

		if (Menu::isOpened) {

			ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);

			return true;
		}

		return CallWindowProcW((WNDPROC)originalWndProc, gameHwnd, msg, wParam, lParam);
	}

	void Init() {
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		ImGui_ImplWin32_Init(gameHwnd);
		ImGui_ImplOpenGL2_Init();
		ImGui::StyleColorsDark();
		ImGuiStyle& style = ImGui::GetStyle();
		style.WindowMinSize = ImVec2(400, 275);

		// handling inputs
		originalWndProc = SetWindowLongPtr(gameHwnd, GWLP_WNDPROC, (LONG_PTR)WndProc);
	}

	void Draw() {
		if (!isOpened) {
			_SDL_SetRelativeMouseMode(true);
			return;
		}

		_SDL_SetRelativeMouseMode(false);
		ImGui_ImplOpenGL2_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		if (!ImGui::Begin("Cheat", NULL, ImGuiWindowFlags_NoCollapse)) {
			ImGui::End();
			return;
		}		

		ImGui::GetIO().MouseDrawCursor = isOpened;
		ImGui::GetIO().WantCaptureMouse = isOpened;

		ImGui::End();
		ImGui::Render();
		ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
	}

	void End() {
		SetWindowLongPtr(gameHwnd, GWLP_WNDPROC, (LONG_PTR)originalWndProc);
	}
}