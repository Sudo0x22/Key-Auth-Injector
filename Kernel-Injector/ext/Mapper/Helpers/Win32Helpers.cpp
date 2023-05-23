#include "Win32Helpers.h"
#include "../../Protection/xorstr.h"
#include "../../Protection/lazy_importer.h"

std::string GetErrorCodeDescription(DWORD err)
{
	std::string result;

	char* buffer = nullptr;
	LI_FN(FormatMessageA).get()(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, err, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (char*)&buffer, 0, NULL);

	if (buffer)
	{
		// replace first CRLF with null terminator
		for (int i = 0; buffer && buffer[i]; ++i)
		{
			if (buffer[i] == '\r')
			{
				if (buffer[i + 1] == '\n')
				{
					buffer[i] = 0;
					break;
				}
			}
		}
		result = buffer;
		LocalFree(buffer);
	}
	else
	{
		result = xorstr_("failed to get error code description");
	}

	return result;
}

DWORD SetPrivilege(HANDLE hToken, LPCTSTR lpszPrivilege, BOOL bEnablePrivilege)
{
	LUID luid;
	DWORD bRet = 0;

	if (LI_FN(LookupPrivilegeValue).get()(NULL, lpszPrivilege, &luid))
	{
		TOKEN_PRIVILEGES tp;
		LI_FN(memset)(&tp, 0, sizeof(tp));
		tp.PrivilegeCount = 1;
		tp.Privileges[0].Luid = luid;
		tp.Privileges[0].Attributes = (bEnablePrivilege) ? SE_PRIVILEGE_ENABLED : 0;

		LI_FN(AdjustTokenPrivileges).get()(hToken, FALSE, &tp, NULL, (PTOKEN_PRIVILEGES)NULL, (PDWORD)NULL);
	}
	bRet = LI_FN(GetLastError).get()();
	return bRet;
}

void GetDebugPrivilege()
{
	HANDLE hProcess = LI_FN(GetCurrentProcess).get()();
	HANDLE hToken;
	if (LI_FN(OpenProcessToken).get()(hProcess, TOKEN_ADJUST_PRIVILEGES, &hToken))
	{
		//printf("[*] obtaining debug privilege\n");
		DWORD errCode = SetPrivilege(hToken, SE_DEBUG_NAME, TRUE);
		if (errCode != 0)
		{
			//printf("\t[-] SetPrivilege failed: %s\n", GetLastErrorCodeDescriptionCstr());
		}
		else
		{
			//puts("\t[+] success");
		}
		LI_FN(CloseHandle).get()(hToken);
	}
}


int GetProcessBitness(HANDLE process)
{
	BOOL isWow64;
	if (!LI_FN(IsWow64Process).get()(process, &isWow64))
	{
		//printf("[-] IsWow64Process failed: %s\n", GetLastErrorCodeDescriptionCstr());
		return 0;
	}

	if (isWow64)
	{
		// 64-bit processor and 32-bit process
		return 32;
	}
	else
	{
		// process arch is same as processor arch
		SYSTEM_INFO sysInfo;
		LI_FN(GetNativeSystemInfo).get()(&sysInfo);

		if (sysInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_INTEL)
			return 32;
		else
			return 64;
	}

	return 0;
}
