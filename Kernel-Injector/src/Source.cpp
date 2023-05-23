#include"../ext/Utils/Utils.h"
#define DIRECTINPUT_VERSION 0x0800
using namespace Utils;

__declspec() void* __stdcall AntiAttachFunc()
{
	while (!pAntiAttach->IsDebuggerAttached)
	{
		if (pGetProcess->GetProcessByImage(xorstr_(L"ida64.exe")) || pGetProcess->GetProcessByImage(xorstr_(L"x64dbg.exe")) || pGetProcess->GetProcessByImage(xorstr_(L"cheatengine-x86_64.exe")) ||
			pGetProcess->GetProcessByImage(xorstr_(L"cheatengine-x86_64-SSE4-AVX2.exe")) || pGetProcess->GetProcessByImage(xorstr_(L"OLLYDBG.exe")) || pGetProcess->GetProcessByImage(xorstr_(L"dlest64.exe")) != NULL)
		{
			pAntiAttach->IsDebuggerAttached = TRUE;
			LI_FN(exit).get()(EXIT_FAILURE);
		}
	}

	return pAntiAttach->ReturnBuffer;
}

int APIENTRY main(const HWND hWnd, UINT uMsg, CHAR* Args[])
{
	pWindow->CreateWindowExAC(xorstr_("Loader"), xorstr_("Color 04"), 500, 450, TRUE);

	if (!Utils::pGetProcess->GetProcessByImage(xorstr_(L"ida64.exe")) || !Utils::pGetProcess->GetProcessByImage(xorstr_(L"x64dbg.exe")) || !Utils::pGetProcess->GetProcessByImage(xorstr_(L"cheatengine-x86_64.exe")) ||
		!Utils::pGetProcess->GetProcessByImage(xorstr_(L"cheatengine-x86_64-SSE4-AVX2.exe")) || !Utils::pGetProcess->GetProcessByImage(xorstr_(L"OLLYDBG.exe")) || !Utils::pGetProcess->GetProcessByImage(xorstr_(L"dlest64.exe")))
	{
		pAntiAttach->NTWindowsCreateThread((LPTHREAD_START_ROUTINE)AntiAttachFunc, NULL, NULL);
		Initialize();

	}

	return 0;
} 