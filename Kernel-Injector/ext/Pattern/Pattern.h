#pragma once
#include"../Protection/global.h"

namespace pattern
{
	class Scanner
	{
	public:
		uint8_t* PatternScan(void* Module, const char* sig);
		PIMAGE_NT_HEADERS GetModuleHeader(void* Module);
		DWORD AllocateEntryPoint(DWORD64 Module);
	}; Scanner* pScanner;

	inline uint8_t* Scanner::PatternScan(void* Module, const char* sig)
	{
		return 0;
	}

	DWORD Scanner::AllocateEntryPoint(DWORD64 Module)
	{
		if (!Module)
			return NULL;

		IMAGE_NT_HEADERS nt_headers = { 0 };
		IMAGE_DOS_HEADER dos_header = { 0 };
		dos_header = *(IMAGE_DOS_HEADER*)Module;
		nt_headers = *(IMAGE_NT_HEADERS*)(Module + dos_header.e_lfanew);

		return nt_headers.OptionalHeader.AddressOfEntryPoint;
	}
}