#include "EffectDriver.h"
#include "Hidsdi.h"
#include <cstdio>

void logmsg(char * msg);

EffectDriver::EffectDriver()
{
	logmsg("effectdriver constructor");
	// Initialize member variables
	refCount = 1;
	hidDevice = NULL;
	dID = 1;
}


EffectDriver::~EffectDriver()
{
	logmsg("effectdriver destructor");
	//CloseHandle(hidDevice);

}

HRESULT STDMETHODCALLTYPE EffectDriver::QueryInterface(REFIID interfaceId, PVOID * object) {
	if (IsEqualIID(interfaceId, IID_IUnknown) || IsEqualIID(interfaceId, IID_IDirectInputEffectDriver)) {
		AddRef();
		logmsg("effectdriver query interface ok");
		*object = this;
		return S_OK;
	} else {
		*object = NULL;
		logmsg("effectdriver query interface not found");
		return E_NOINTERFACE;
	}

}

ULONG STDMETHODCALLTYPE EffectDriver::AddRef(void) {
	
	InterlockedIncrement(&refCount);
	logmsg("effectdriver addref");
	return refCount;
}

ULONG STDMETHODCALLTYPE EffectDriver::Release(void) {
	
	InterlockedDecrement(&refCount);
	logmsg("effectdriver release");
	if (refCount == 0) {
		logmsg("effectdriver gone");
		delete this;
		return 0;
	}

	return refCount;
}

HRESULT STDMETHODCALLTYPE EffectDriver::DestroyEffect(DWORD deviceId, DWORD downloadId) {
	UNREFERENCED_PARAMETER(downloadId);
	UNREFERENCED_PARAMETER(deviceId);

	UINT8 outRep[] = { 1, 0x10, 2, 3,4 };
	//HidD_SetOutputReport(hidDevice, outRep, 5);

	return S_OK;
}

HRESULT STDMETHODCALLTYPE EffectDriver::DeviceID(DWORD diVer, DWORD externalId, DWORD fBegin, DWORD internalId, LPVOID diHidInitInfo) {
	UNREFERENCED_PARAMETER(internalId);
	UNREFERENCED_PARAMETER(fBegin);
	UNREFERENCED_PARAMETER(externalId);
	UNREFERENCED_PARAMETER(diVer);
	
	LPDIHIDFFINITINFO initInfo = (LPDIHIDFFINITINFO)diHidInitInfo;

	wcscpy(deviceStr, initInfo->pwszDeviceInterface);
	/*
	if (hidDevice == NULL) {
		hidDevice = CreateFileW(initInfo->pwszDeviceInterface, GENERIC_WRITE | GENERIC_READ,
					FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL);
	}
	else {
		logmsg("second device id");
	}
	/**
	UINT8 outRep[] = { 1, 1, 2, 3, 4 };
	outRep[2] = externalId;
	outRep[3] = refCount;
	outRep[4] = refCount;
	HidD_SetOutputReport(hidDevice, outRep, 5);
	*/
	return S_OK;
}



HRESULT STDMETHODCALLTYPE EffectDriver::DownloadEffect(DWORD deviceId, DWORD internalEffectType, LPDWORD downloadId, LPCDIEFFECT effect, DWORD flags) {
	char motoron;

	if (flags & DIEP_NODOWNLOAD) {
		logmsg("nodl");
		return S_OK;
	}
	if (!(flags & DIEP_START)) {
		logmsg("dl and start");
		return S_OK;
	}

	if (*downloadId == NULL) {
		*downloadId = dID++;
		logmsg("id null");
	}

	DICONSTANTFORCE param = *(DICONSTANTFORCE*)(effect->lpvTypeSpecificParams);

	if (param.lMagnitude != lastMagnitude) {
		if (param.lMagnitude == 0) {
			motoron = 0;
		} else {
			motoron = 0xff;
		}
		UINT8 outRep[] = { 1, 2, motoron, motoron, *downloadId };
		hidDevice = CreateFileW(deviceStr, GENERIC_WRITE | GENERIC_READ,
			FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL);
		if (WriteFile(hidDevice, outRep, 5, NULL, NULL) == FALSE) {
			char buffer[30];
			logmsg("writefile false");
			snprintf(buffer, 30, "errorcode: %d", GetLastError());
			logmsg(buffer);
		
		}
		CloseHandle(hidDevice);


		//HidD_SetOutputReport(hidDevice, outRep, 5);
		lastMagnitude = param.lMagnitude;
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE EffectDriver::Escape(DWORD id, DWORD effect, LPDIEFFESCAPE escape) {
	UINT8 outRep[] = { 1, 9, 0,0,0 };
	//HidD_SetOutputReport(hidDevice, outRep, 5);

	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE EffectDriver::GetEffectStatus(DWORD deviceId, DWORD downloadId, LPDWORD statusCode) {

	*statusCode = 0;
	UINT8 outRep[] = { 1, 3, 4, 5, 6 };
	hidDevice = CreateFileW(deviceStr, GENERIC_WRITE | GENERIC_READ,
		FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL);
	if (WriteFile(hidDevice, outRep, 5, NULL, NULL) == FALSE) {
		char buffer[30];
		logmsg("writefile false");
		snprintf(buffer, 30, "errorcode: %d", GetLastError());
		logmsg(buffer);

	}
	CloseHandle(hidDevice);
	//HidD_SetOutputReport(hidDevice, outRep, 5);

	return S_OK;
}

HRESULT STDMETHODCALLTYPE EffectDriver::GetForceFeedbackState(DWORD deviceId, LPDIDEVICESTATE deviceState) {
	deviceState->dwState = DIGFFS_ACTUATORSON | DIGFFS_POWERON | DIGFFS_USERFFSWITCHON;
	UINT8 outRep[] = { 1, 4, 4, 5, 6 };
	//HidD_SetOutputReport(hidDevice, outRep, 5);
	hidDevice = CreateFileW(deviceStr, GENERIC_WRITE | GENERIC_READ,
		FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL);
	if (WriteFile(hidDevice, outRep, 5, NULL, NULL) == FALSE) {
		char buffer[30];
		logmsg("writefile false");
		snprintf(buffer, 30, "errorcode: %d", GetLastError());
		logmsg(buffer);

	}
	CloseHandle(hidDevice);
	return S_OK;
}

HRESULT STDMETHODCALLTYPE EffectDriver::GetVersions(LPDIDRIVERVERSIONS version) {
	version->dwFFDriverVersion = 0x800;
	version->dwFirmwareRevision = 1;
	version->dwHardwareRevision = 1;
	logmsg("getver");
	return S_OK;
}

HRESULT STDMETHODCALLTYPE EffectDriver::SendForceFeedbackCommand(DWORD deviceId, DWORD state) {
	UINT8 outRep[] = { 1, 5, 4, 5, 6 };
	outRep[2] = (state & 0xFF00) >> 8;
	outRep[3] = state & 0x00ff;
	outRep[4] = deviceId;
		
	//HidD_SetOutputReport(hidDevice, outRep, 5);
	hidDevice = CreateFileW(deviceStr, GENERIC_WRITE | GENERIC_READ,
		FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL);
	if (WriteFile(hidDevice, outRep, 5, NULL, NULL) == FALSE) {
		char buffer[30];
		logmsg("writefile false");
		snprintf(buffer, 30, "errorcode: %d", GetLastError());
		logmsg(buffer);

	}
	CloseHandle(hidDevice);
	switch (state) {
	case DISFFC_RESET:
		break;
	case DISFFC_STOPALL:
		break;
	case DISFFC_PAUSE:
		break;
	case DISFFC_CONTINUE:
		break;
	case DISFFC_SETACTUATORSON:
		break;
	case DISFFC_SETACTUATORSOFF:
		break;
	default:
		break;
	}

	return S_OK;

}

HRESULT STDMETHODCALLTYPE EffectDriver::SetGain(DWORD deviceId, DWORD gain) {
	UINT8 outRep[] = { 1, 6, 8, 5, 6 };
	outRep[2] = (gain & 0x00FF0000) >> 16;
	outRep[3] = (gain & 0x0000FF00) >> 8;
	outRep[4] = (gain & 0x000000ff);
	//HidD_SetOutputReport(hidDevice, outRep, 5);

	return S_OK;
}

HRESULT STDMETHODCALLTYPE EffectDriver::StartEffect(DWORD deviceId, DWORD downloadId, DWORD mode, DWORD iterations) {
	UINT8 outRep[] = { 1, 7, 4, 5, 6 };
	//HidD_SetOutputReport(hidDevice, outRep, 5);
	logmsg("starteffect");

	return S_OK;
}

HRESULT STDMETHODCALLTYPE EffectDriver::StopEffect(DWORD deviceId, DWORD downloadId) {
	UNREFERENCED_PARAMETER(deviceId);
	UNREFERENCED_PARAMETER(downloadId);
	UINT8 outRep[] = { 1, 8, 4, 5, 6 };
	//HidD_SetOutputReport(hidDevice, outRep, 5);
	logmsg("stopeffect");


	return S_OK;
}
