#pragma once

#include <iostream>

uintptr_t dwLocalPlayer = 0x18AC00;
uintptr_t dwEntityList = 0x10f4f8;
uintptr_t dwEntityListLength = 0x10f500;

// Trigger
uintptr_t dwDisplayNametag = 0xad9d;
uintptr_t dwDisplayNametagOriginalCall = 0x607c0;
uintptr_t dwDisplayNametagReturn = 0xada2;

// No recoil
uintptr_t dwRecoilInstruction = 0x5baad;