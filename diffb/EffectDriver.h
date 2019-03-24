#pragma once

#ifndef	DIRECTINPUT_VERSION
#define	DIRECTINPUT_VERSION	0x0800
#endif

#include <initguid.h>
#include <dinput.h>
#include <dinputd.h>
#include <Windows.h>




#define MAX_EFFECTS 0xFF

// Commands
enum command : UINT8 {
	START_EFFECT= 0,
	STOP_EFFECT = 1,
	NEW_EFFECT	= 2,
	SET_GAIN	= 3
};

class EffectDriver : public IDirectInputEffectDriver
{
public:
	EffectDriver();
	~EffectDriver();

	// COM
	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID interfaceId, PVOID * object);
	ULONG STDMETHODCALLTYPE AddRef();
	ULONG STDMETHODCALLTYPE Release();

	// DirectInput FFB
	HRESULT STDMETHODCALLTYPE DestroyEffect(DWORD deviceId, DWORD downloadId);
	HRESULT STDMETHODCALLTYPE DeviceID(DWORD diVer, DWORD externalId, DWORD fBegin, DWORD internalId, LPVOID diHidInitInfo);
	HRESULT STDMETHODCALLTYPE DownloadEffect(DWORD deviceId, DWORD internalEffectType, LPDWORD downloadId, LPCDIEFFECT effect, DWORD flags);
	HRESULT STDMETHODCALLTYPE Escape(DWORD id, DWORD effect, LPDIEFFESCAPE escape);
	HRESULT STDMETHODCALLTYPE GetEffectStatus(DWORD deviceId, DWORD downloadId, LPDWORD statusCode);
	HRESULT STDMETHODCALLTYPE GetForceFeedbackState(DWORD deviceId, LPDIDEVICESTATE deviceState);
	HRESULT STDMETHODCALLTYPE GetVersions(LPDIDRIVERVERSIONS version);
	HRESULT STDMETHODCALLTYPE SendForceFeedbackCommand(DWORD deviceId, DWORD state);
	HRESULT STDMETHODCALLTYPE SetGain(DWORD deviceId, DWORD gain);
	HRESULT STDMETHODCALLTYPE StartEffect(DWORD deviceId, DWORD downloadId, DWORD mode, DWORD iterations);
	HRESULT STDMETHODCALLTYPE StopEffect(DWORD deviceId, DWORD downloadId);

private:
	ULONG refCount;
	HANDLE hidDevice;
	DWORD dID;
	LONG lastMagnitude;
	WCHAR deviceStr[512];
};

