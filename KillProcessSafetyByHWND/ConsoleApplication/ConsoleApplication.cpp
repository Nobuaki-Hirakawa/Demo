// ConsoleApplication.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"

HWND GetWindowHandle(const DWORD target_id)
{
	auto hWnd = GetTopWindow(nullptr);
	do {
		if (GetWindowLong(hWnd, GWLP_HWNDPARENT) != 0 || !IsWindowVisible(hWnd))
			continue;

		DWORD process_id;
		GetWindowThreadProcessId(hWnd, &process_id);
		if (target_id == process_id)
			return hWnd;
	} while ((hWnd = GetNextWindow(hWnd, GW_HWNDNEXT)) != nullptr);

	return nullptr;
}

int main()
{
	// Launch application
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	::ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_SHOWNORMAL;
	::CreateProcessW(L"Wpf.exe", nullptr, nullptr, nullptr, FALSE, 0, nullptr, nullptr, &si, &pi);

	_tprintf(_T("Terminate application: %d\n"), pi.dwProcessId);

	// Wait until application completely starts to create window handle
	Sleep(1000);
	auto handle = GetWindowHandle(pi.dwProcessId);
	::PostMessage(handle, WM_CLOSE, 0, 0);

	// Terminate forcibly if application does not quit in 5sec.
	if (::WaitForSingleObject(pi.hProcess, 5000) == WAIT_TIMEOUT)
	{
		_tprintf(_T("Terminate forcibly"));
		TerminateProcess(pi.hProcess, 0);
	}

	::CloseHandle(pi.hThread);
	::CloseHandle(pi.hProcess);

    return 0;
}

