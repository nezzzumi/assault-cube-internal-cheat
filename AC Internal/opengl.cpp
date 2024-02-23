#pragma once

#include <Windows.h>
#include "Memory.h"
#include <iostream>
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_opengl2.h"

namespace OpenGL {
	DWORD returnAddress;
	DWORD oldPerm;
	bool sla;

	DWORD GetwglSwapBuffersAddress() {
		HMODULE openglBaseAddress = GetModuleHandle(L"opengl32.dll");

		if (!openglBaseAddress) {
			MessageBox(NULL, L"OpenGL not found", L"Error!!", MB_OK);
			return NULL;
		}

		return (DWORD)GetProcAddress(openglBaseAddress, "wglSwapBuffers");
	}

	void Draw() {
		ImGui_ImplOpenGL2_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("DearCube", &sla, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse);
		ImGui::BeginTabBar("oi");
		ImGui::EndTabBar();
		ImGui::End();

		ImGui::EndFrame();
		ImGui::Render();
		ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
	}

	__declspec(naked) void wglSwapBuffersCodeCave() {
		returnAddress = OpenGL::GetwglSwapBuffersAddress() + 0x5;

		_asm {
			pushad
			popad
		}

		OpenGL::Draw(); // TODO: create a new file 'Menu.cpp'?

		_asm {
			mov edi, edi
			push ebp
			mov ebp, esp
			jmp returnAddress
		}
	}

	void Hook() {
		oldPerm = Memory::CodeCave(GetwglSwapBuffersAddress(), (DWORD)&wglSwapBuffersCodeCave, 5);
		std::cout << std::hex << GetwglSwapBuffersAddress() << std::endl;
		std::cout << std::hex << wglSwapBuffersCodeCave << std::endl;
	}

	void UnHook() {
		BYTE originalBytes[5] = { 0x8b, 0xff, 0x55, 0x8b, 0xec };

		for (size_t i = 0; i < sizeof(originalBytes); i++)
		{
			Memory::Write<BYTE>(GetwglSwapBuffersAddress() + i, originalBytes[i]);
		}

		Memory::Protect(GetwglSwapBuffersAddress(), sizeof(originalBytes), oldPerm);
	}
}