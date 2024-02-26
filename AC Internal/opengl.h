#pragma once

#include <Windows.h>

namespace OpenGL {
	extern DWORD returnAddress;
	extern DWORD oldPerm;

	DWORD GetwglSwapBuffersAddress();

	void Draw();

	void wglSwapBuffersCodeCave();

	void Hook();

	void UnHook();
}