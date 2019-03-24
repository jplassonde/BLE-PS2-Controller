#include "ClassFactory.h"
#include "EffectDriver.h"
void logmsg(char * msg);

ClassFactory::ClassFactory() {
	refCount = 1;
	logmsg("Classfact constructor");
}

ClassFactory::~ClassFactory() {
}

HRESULT STDMETHODCALLTYPE ClassFactory::CreateInstance(IUnknown *, REFIID interfaceId, PVOID * object) {
	HRESULT result;
	EffectDriver * DiEffectDriver = new EffectDriver();
	logmsg("new DiEffectDriver");
	if (DiEffectDriver != NULL) {
		result = DiEffectDriver->QueryInterface(interfaceId, object);
		DiEffectDriver->Release();
		return result;
	} else {
		return E_OUTOFMEMORY;
	}
}

HRESULT STDMETHODCALLTYPE ClassFactory::QueryInterface(REFIID interfaceId, PVOID * object) {
	logmsg("Classfactory query");
	if (IsEqualIID(interfaceId, IID_IUnknown) || IsEqualIID(interfaceId, IID_IClassFactory)) {
		AddRef();
		*object = this;
		return S_OK;
	} else {
		logmsg("Classfact no interface");
		*object = NULL;
		return E_NOINTERFACE;
	}
}

ULONG STDMETHODCALLTYPE ClassFactory::AddRef() {
	
	InterlockedIncrement(&refCount);

	return refCount;
}

ULONG STDMETHODCALLTYPE ClassFactory::Release() {

	InterlockedDecrement(&refCount);
	
	if (refCount == 0) {
		logmsg("Classfactory gone");
		delete this;
		return 0;
	}

	return refCount;
}

HRESULT STDMETHODCALLTYPE ClassFactory::LockServer(BOOL lock) {
	UNREFERENCED_PARAMETER(lock);
	logmsg("lockserver");
	return S_OK;
}