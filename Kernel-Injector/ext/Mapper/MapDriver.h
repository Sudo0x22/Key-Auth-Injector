#pragma once
#include"MapImage.h"

#define _WIN64

namespace Mapper
{
	class MapDriver
	{
	public:
		void* driver_buffer[1024];
		int value = 256;
		const char* service_title = NULL;
		HANDLE hServiceThread = NULL;

	public:
		__declspec() void* call_driver(void* rawData)
		{
			PIMAGE_NT_HEADERS nt_headers = { 0 };
			PIMAGE_DOS_HEADER dos_header = { 0 };

			auto raw_bytes = reinterpret_cast<DWORD>(rawData);

			dos_header = *(PIMAGE_DOS_HEADER*)rawData;
			nt_headers = *(PIMAGE_NT_HEADERS*)(raw_bytes + dos_header->e_lfanew);

			// allocS


		}
	private:
		__declspec() void service_handler(void* buffer, int result, const char* service, HANDLE hService)
		{
			buffer = this->driver_buffer;
			result = this->value;
			service = this->service_title;
		}

	}; MapDriver* pMapper;
}

