#pragma once

#include <Windows.h>
#include <objbase.h>

class ClassFactory : public IClassFactory
{
public:
	ClassFactory();
	~ClassFactory();
	
	HRESULT STDMETHODCALLTYPE CreateInstance(IUnknown *, REFIID interfaceId, PVOID * object);
	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID interfaceId, PVOID * object);
	ULONG STDMETHODCALLTYPE AddRef();
	ULONG STDMETHODCALLTYPE Release();
	HRESULT STDMETHODCALLTYPE LockServer(BOOL lock);
private:
	ULONG refCount;
};

