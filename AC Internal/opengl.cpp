#pragma once

#include <Windows.h>
#include "Memory.h"
#include <iostream>
#include "Menu.h"

namespace OpenGL {
	DWORD returnAddress;
	DWORD oldPerm;

	DWORD GetwglSwapBuffersAddress() {
		HMODULE openglBaseAddress = GetModuleHandle(L"opengl32.dll");

		if (!openglBaseAddress) {
			MessageBox(NULL, L"OpenGL not found", L"Error!!", MB_OK);
			return NULL;
		}

		return (DWORD)GetProcAddress(openglBaseAddress, "wglSwapBuffers");
	}

	void Draw() {
		Menu::Draw();
	}

	__declspec(naked) void wglSwapBuffersCodeCave() {
		returnAddress = OpenGL::GetwglSwapBuffersAddress() + 0x5;

		_asm {
			pushad
			popad
		}

		OpenGL::Draw();

		_asm {
			mov edi, edi
			push ebp
			mov ebp, esp
			jmp returnAddress
		}
	}

	void Hook() {
		oldPerm = Memory::CodeCave(GetwglSwapBuffersAddress(), (DWORD)&wglSwapBuffersCodeCave, 5);
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