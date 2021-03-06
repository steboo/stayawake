#include "StayAwake.h"
#include <wchar.h>
#include <strsafe.h>

int main() {
	// Make sure we use Hungarian notation for that real Windows feel.
	// (What is the notation for a struct?)
	REASON_CONTEXT stReason;
	stReason.Version = POWER_REQUEST_CONTEXT_VERSION;
	stReason.Flags = POWER_REQUEST_CONTEXT_SIMPLE_STRING;
	stReason.Reason.SimpleReasonString = L"StayAwake has requested to stay awake.";

	// First we need to create a power request with the "reason" which is just a string in this case
	// (You can see a list of reasons in `powerqry.exe /requests`.)
	// MSDN says that this will only work in Windows 7/2008 R2 and up.
	HANDLE hRequest;
	hRequest = PowerCreateRequest(&stReason);

	if (hRequest == INVALID_HANDLE_VALUE) {
		DWORD dwError = GetLastError();
		errorHandle(dwError, L"PowerGetActiveScheme");
		ExitProcess(dwError);
	}

	POWER_REQUEST_TYPE eType = PowerRequestSystemRequired;

	// PowerSetRequest does the work.
	if (!PowerSetRequest(hRequest, eType)) {
		DWORD dwError = GetLastError();
		errorHandle(dwError, L"PowerSetRequest");
		CloseHandle(hRequest);
		ExitProcess(dwError);
	}

	wprintf(L"Staying awake... press any key to stop...");
	_getwch();

	// PowerClearRequest resets things to the way they were...
	if (!PowerClearRequest(hRequest, eType)) {
		DWORD dwError = GetLastError();
		errorHandle(dwError, L"PowerClearRequest");
		CloseHandle(hRequest);
		ExitProcess(dwError);
	}

	CloseHandle(hRequest);

	return 0;
}

void errorHandle(DWORD dwError, LPWSTR lpszFunction) {
	LPVOID lpMsgBuf;
	LPVOID lpDisplayBuf;

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dwError,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPWSTR)&lpMsgBuf,
		0, NULL);
	lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
		(lstrlen((LPCWSTR)lpMsgBuf) + lstrlen((LPCWSTR)lpszFunction) + 40) * sizeof(WCHAR));
	StringCchPrintf((LPWSTR)lpDisplayBuf,
		LocalSize(lpDisplayBuf) / sizeof(WCHAR),
		TEXT("%s failed with error %d: %s"),
		lpszFunction, dwError, lpMsgBuf);

	wprintf(lpDisplayBuf);

	LocalFree(lpMsgBuf);
	LocalFree(lpDisplayBuf);
}