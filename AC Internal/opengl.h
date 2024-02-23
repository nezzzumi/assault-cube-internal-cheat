#pragma once

#include <Windows.h>


namespace OpenGL {
	extern DWORD returnAddress;

	DWORD GetwglSwapBuffersAddress();

	void Hook();

	void UnHook();
}