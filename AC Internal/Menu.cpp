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
	bool isOpened = false;
	HWND gameHwnd = FindWindowA(NULL, "AssaultCube");
	LONG_PTR originalWndProc;

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
			return;
		}

		ImGui_ImplOpenGL2_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		if (!ImGui::Begin("Cheat", NULL, ImGuiWindowFlags_NoCollapse)) {
			ImGui::End();
			return;
		}

		if (ImGui::BeginMenuBar()) {
			if (ImGui::BeginMenu("oi")) {
				ImGui::SeparatorText("Mini apps");
				ImGui::MenuItem("Main menu bar", NULL, &noRecoil);
				ImGui::Text("dear imgui says hello! (%s) (%d)", IMGUI_VERSION, IMGUI_VERSION_NUM);

				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
		ImGui::Text("dear imgui says hello! (%s) (%d)", IMGUI_VERSION, IMGUI_VERSION_NUM);

		ImGui::End();

		ImGui::GetIO().MouseDrawCursor = isOpened;
		//ImGui::GetIO().WantCaptureMouse = isOpened;

		ImGui::EndFrame();
		ImGui::Render();
		ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
	}

	void End() {
		SetWindowLongPtr(gameHwnd, GWLP_WNDPROC, (LONG_PTR)originalWndProc);
	}
}