#pragma once
#include"../Mapper/MapDriver.h"
#include"../Auth/utils.hpp"
#include"../Auth/skStr.h"
#include "../Auth/auth.hpp"
#include"../Mapper/Driver/kdmapper.hpp"

std::string tm_to_readable_time(tm ctx);
static std::time_t string_to_timet(std::string timestamp);
static std::tm timet_to_tm(time_t timestamp);
const std::string compilation_date = (std::string)skCrypt(__DATE__);
const std::string compilation_time = (std::string)skCrypt(__TIME__);

std::string name = skCrypt("").decrypt(); // application name. right above the blurred text aka the secret on the licenses tab among other tabs
std::string ownerid = skCrypt("").decrypt(); // ownerid, found in account settings. click your profile picture on top right of dashboard and then account settings.
std::string secret = skCrypt("").decrypt(); // app secret, the blurred text on licenses tab and other tabs
std::string version = skCrypt("1.0").decrypt(); // leave alone unless you've changed version on website
std::string url = skCrypt("https://keyauth.win/api/1.2/").decrypt(); // change if you're self-hosting

using namespace KeyAuth;

api KeyAuthApp(name, ownerid, secret, version, url);

typedef struct
{
	DWORD64 dwEP;
	PVOID pParam;
} CALL_FUNC, * pCALL_FUNC;
typedef DWORD(*_Function)(VOID* p);

void WINAPI MyFunctionThread(pCALL_FUNC pCMF)
{
	if (pCMF != NULL && pCMF->dwEP != NULL)
	{
		_Function function = (_Function)pCMF->dwEP;
		function(pCMF->pParam);
	}
}

namespace Utils
{
	class GetProcess
	{
	public:
		HWND hWindow = NULL;
		ULONG64* ProcessID = nullptr;
		LPCSTR szWndClass = NULL;

	public:
		DWORD GetProcessByImage(const wchar_t* const ProcImage)
		{
			const HANDLE hEntrySnap = LI_FN(CreateToolhelp32Snapshot).get()(TH32CS_SNAPPROCESS, 0);
			PROCESSENTRY32W ProcEntry = { 0 };
			ProcEntry.dwSize = sizeof(PROCESSENTRY32);

			if (hEntrySnap == INVALID_HANDLE_VALUE)return-1;
			if (LI_FN(Process32FirstW).get()(hEntrySnap, &ProcEntry) == FALSE)return -1;

			while (LI_FN(Process32NextW).get()(hEntrySnap, &ProcEntry))
			{
				if (!LI_FN(wcscmp)(ProcEntry.szExeFile, ProcImage))
				{
					LI_FN(CloseHandle).get()(hEntrySnap);
					return ProcEntry.th32ProcessID;
				}
			}

			LI_FN(CloseHandle).get()(hEntrySnap);
			return 0;
		}
	}; GetProcess* pGetProcess;

	class WindowDebug
	{
	public:
		const char* format{};
		int szCode{};
		DWORD lpMilliseconds{};
	public:
		__declspec() void __stdcall PrintDebugMsgEx(LPCSTR Message, ...)
		{
			typedef const char*(__stdcall* DebugMessage)(LPCSTR);
			DebugMessage print_status = reinterpret_cast<DebugMessage>(LI_FN(puts)(Message));
		}

		__declspec() void __stdcall WndSleep(DWORD lpSeconds)
		{
			LI_FN(Sleep).get()(lpSeconds);
		}

		__declspec() void __stdcall WndExit(int Code)
		{
			LI_FN(exit).get()(Code);
		}
		
		__declspec() void __stdcall WndClear(LPCSTR Command)
		{
			LI_FN(system).get()(Command);
		}

	}; WindowDebug* pWindowDebugEx;

	class CreateFileFromMemory
	{
	public:
		size_t size{};
		const char* addr{};
		const uintptr_t address{};
	public:
		bool CreateFileEx(const std::string& file_path, const char* file_addr, size_t file_size)
		{
			std::ofstream file_ofstream(file_path.c_str(), std::ios_base::out | std::ios_base::binary);
			if (!file_ofstream.write(file_addr, file_size))
			{
				file_ofstream.close();
				return false;
			}
			file_ofstream.close();
			return true;
		}

		bool CreateFileA(const std::string& file_path, std::string data)
		{
			std::vector<std::uint8_t> bytes = KeyAuthApp.download(data);
			std::ofstream result (file_path, std::ios_base::out | std::ios_base::binary);
			result.write((char*)bytes.data(), bytes.size());
			result.close();
			return true;
		}

		CreateFileFromMemory(const char* image_title, const char* file_addr, size_t file_size)
		{
			char temp_dir[MAX_PATH] = { 0 };
			const uint32_t GetTempDir = LI_FN(GetTempPathA).get()(sizeof(temp_dir), temp_dir);
			const char* const image = image_title;

			const std::string image_path = std::string(temp_dir) + xorstr_("\\") + image;
			std::remove(image_path.c_str());

			if (!CreateFileEx(image_path, file_addr, file_size)) {
				return;
			}

			LI_FN(CreateFileW).get()(xorstr_(L""), GENERIC_READ | GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		}
	}; CreateFileFromMemory* pCreateFileEx;

	class DoesFileExist {
	public:
		bool DoesFileExistEx(std::string file_dir)
		{
			struct stat buffer;
			return (stat(file_dir.c_str(), &buffer) == 0);
		}
	}; DoesFileExist* pDoesFileExist;

	class hKeySys
	{
	public:
		HKEY hLocalKey = NULL;
		HKEY hCurrentUser = NULL;
	public:

	}; hKeySys* pKeySys;

	class Window
	{
	public:
		DWORD ProcessID = NULL;
		HWND hWindow;
		RECT WindowRect;
		void* WndBuffer[1024];

	public:
		_declspec() void* __stdcall CreateWindowExAC(const char* lpWndTitle, const char* TextColor, int lpWidth, int lpHeight, BOOL lpRepaint)
		{
			LI_FN(SetConsoleTitleA).get()(lpWndTitle);
			LI_FN(system).get()(TextColor);
			this->hWindow = LI_FN(GetConsoleWindow).get()();
			LI_FN(GetWindowRect).get()(this->hWindow, &this->WindowRect);
			LI_FN(MoveWindow).get()(this->hWindow, this->WindowRect.left, this->WindowRect.top, lpWidth, lpHeight, lpRepaint);
			return this->WndBuffer;
		}
	}; Window* pWindow = new Window();


	class AntiDebug
	{
	public:
		void* ReturnBuffer[1024];
		BOOL IsDebuggerAttached = FALSE;
		INT szValue[256];
		IMAGE_NT_HEADERS nt_headers = { 0 };
		IMAGE_DOS_HEADER dos_header = { 0 };
		HINSTANCE hModule = NULL;
		DWORD ImageSize = NULL;
		BYTE* pMemoryBytes = { 0 };

	public:

		DWORD GetModuleSize(DWORD64 Image)
		{
			if (!Image)return -1;
			this->dos_header = *(IMAGE_DOS_HEADER*)Image;
			this->nt_headers = *(IMAGE_NT_HEADERS*)(Image + this->dos_header.e_lfanew);
			return this->nt_headers.OptionalHeader.SizeOfImage;
		}

		HANDLE __stdcall NTWindowsCreateThread(LPTHREAD_START_ROUTINE lpEntryAddr, LPVOID hBuffer, LPDWORD lpThreadId)
		{
			this->hModule = LI_FN(GetModuleHandleA).get()(xorstr_("ntdll.dll"));
			if (this->hModule != NULL)
			{
				this->ImageSize = this->GetModuleSize((DWORD64)this->hModule);
				this->pMemoryBytes = (BYTE*)this->hModule + this->ImageSize - 0x400;

				if (this->pMemoryBytes)
				{
					DWORD dwProtect;
					LI_FN(VirtualProtect).get()(this->pMemoryBytes, 0x100, PAGE_EXECUTE_READWRITE, &dwProtect);
					CALL_FUNC* pCMF = (CALL_FUNC*)LI_FN(VirtualAlloc).get()(NULL, 0x100, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
					pCMF->dwEP = (DWORD64)(lpEntryAddr);
					pCMF->pParam = hBuffer;
					LI_FN(memcpy)((LPVOID)this->pMemoryBytes, (LPVOID)MyFunctionThread, 0x100);
					const HANDLE hRemoteThread = LI_FN(CreateRemoteThread).get()(LI_FN(GetCurrentProcess)(), NULL, 0, (LPTHREAD_START_ROUTINE)this->pMemoryBytes, pCMF, NULL, lpThreadId);
					return hRemoteThread;
				}
			}

			return 0;
		}

	}; AntiDebug* pAntiAttach = new AntiDebug();

	class Initialize
	{
	public:
		char Key[50] = "";
		int value = 256;
		BOOL Unload = false;
		void* ReturnBuffer[1024];

		const std::string ImageBytes(const char* const ImageTitle)
		{
			char TempDir[MAX_PATH] = { 0 };
			const uint32_t GetTempDir = LI_FN(GetTempPathA).get()(sizeof(TempDir), TempDir);
			const std::string ImageDir = std::string(TempDir) + xorstr_("\\") + ImageTitle;
			return ImageDir;
		}

		void __stdcall CreateImage(const std::string& file_path)
		{
			// create dll 
			// stream dll not included
			pCreateFileEx->CreateFileA(file_path, skCrypt("974089").decrypt());
		}

		Initialize()
		{
			KeyAuthApp.init();

			pWindowDebugEx->PrintDebugMsgEx(xorstr_("[+] Enter Key: "));
			
			if (LI_FN(fgets)(this->Key, this->value, stdin) != NULL)
			{
				KeyAuthApp.license(this->Key);
				if (!KeyAuthApp.data.success) { return; }
					
				pWindowDebugEx->WndClear(xorstr_("cls"));
				pWindowDebugEx->PrintDebugMsgEx(xorstr_("[ - LOGS - ] -> Loading Modules Please Wait"));

				// init driver here
				


				// locate proc handle here
				if (!pGetProcess->GetProcessByImage(xorstr_(L"SaintsRow.exe"))) { return; }

				CreateImage(ImageBytes(xorstr_("x12.dll")));
				pWindowDebugEx->WndSleep(5000);
				
				Mapper::ImageMapper(xorstr_("x12.dll"), pGetProcess->GetProcessByImage(xorstr_(L"SaintsRow.exe")));
				pWindowDebugEx->WndSleep(5000);

				pWindowDebugEx->PrintDebugMsgEx(xorstr_("[ - LOGS - ] -> Loaded Closing Now"));
				pWindowDebugEx->WndSleep(3000);
				pWindowDebugEx->WndExit(1);
			}
		}

	}; Initialize* pInitialize;

}