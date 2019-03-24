#pragma once

#include "windows.h"
#include <initguid.h>
DEFINE_GUID(GUID_DIFFBTEST, 0xDEADDEED, 0x0000, 0xFFFF, 0x64, 0x32, 0x16, 0x08, 0x04, 0x02, 0x01, 0x42);

BOOL WINAPI DllMain(HINSTANCE moduleHandle, DWORD reason, PVOID reserved);
STDAPI DllGetClassObject(REFCLSID classId, REFIID interfaceId, LPVOID *object);