#include "dllmain.h"
#include "ClassFactory.h"

void logmsg(char * msg);


BOOL APIENTRY DllMain(HINSTANCE moduleHandle, DWORD reason, PVOID reserved) {
	UNREFERENCED_PARAMETER(moduleHandle);
	UNREFERENCED_PARAMETER(reserved);

	switch (reason) {
	case DLL_PROCESS_ATTACH: 
		logmsg("Dll attach");
		break;
	case DLL_PROCESS_DETACH:
		logmsg("dll detach");
		break;
	default:
		break;
	}
	return true;
}

STDAPI DllGetClassObject(REFCLSID classId, REFIID interfaceId, LPVOID *object) {
	logmsg("dllmainGetClass");
	HRESULT result;
	
	if (IsEqualGUID(classId, GUID_DIFFBTEST)) {
		ClassFactory * Factory = new ClassFactory();
		logmsg("creating new factory");
		
		if (Factory != NULL) {
			result = Factory->QueryInterface(interfaceId, object);
			Factory->Release();
		} else {
			result = E_OUTOFMEMORY;
			logmsg("cfoom");

		}
	} else {
		result = CLASS_E_CLASSNOTAVAILABLE;
		logmsg("cfnotavail");

	}
	
	return result;
}

STDAPI DllCanUnloadNow() {
	logmsg("dllcanunload");
	return S_OK;
}
