#pragma once
#include"../Protection/global.h"
#include"../Image/Data.h"
#include"Helpers/injection.h"

namespace Mapper
{
	class ImageMapper
	{
	public:

		const std::string GetImage(const char* const ImageTitle)
		{
			char TempDir[MAX_PATH] = { 0 };
			const uint32_t GetTempDir = LI_FN(GetTempPathA).get()(sizeof(TempDir), TempDir);
			const std::string ImageDir = std::string(TempDir) + xorstr_("\\") + ImageTitle;
			return ImageDir;
		}

		ImageMapper(const char* const image, DWORD ProcID)
		{
			if (ProcID != NULL)
			{
				ManualMappingInject(GetImage(image).c_str(), ProcID);
				LI_FN(Sleep).get()(5000);
			}
		}
	}; 

	class ManualMapDriver
	{
	public:
		ManualMapDriver(void* rawData)
		{
			IMAGE_NT_HEADERS nt_headers = { 0 };
			IMAGE_DOS_HEADER dos_header = { 0 };

			dos_header = *(IMAGE_DOS_HEADER*)rawData;
			nt_headers = *(IMAGE_NT_HEADERS*)(reinterpret_cast<BYTE*>(rawData) + dos_header.e_lfanew);
			auto entry_point = reinterpret_cast<void*>(nt_headers.OptionalHeader.AddressOfEntryPoint);

			int count = 0;
			BYTE* pMemoryBytes = (BYTE*)rawData;

			BYTE EntryCode[] = {0x00,0x00,0x00,0x00,0x00,0x00};
			BYTE ShellCodeStart[] = { 0x34, 0x1F };
			BYTE ShellCodeEnd[] = { 0x4c, 0x8f };

			if (entry_point)
			{
				int chunk_of_data[1024];
				char* buffer[45];

				printf(xorstr_("welcome"));
				Sleep(10000);
			}
		}

		bool DoesImageExist(const std::string& path)
		{
			struct stat buffer;
			return (stat(path.c_str(), &buffer) == FALSE);
		}

		void* MapDriver(void* rawData)
		{
			char TempDir[MAX_PATH] = { 0 };
			const uintptr_t GetTempDir = LI_FN(GetTempPathA).get()(sizeof(TempDir), TempDir);
			const char* const driver_image = xorstr_("i64x.sys");
			
			const char* const mapper_image = xorstr_("i64_mapper.exe");
			
			const std::string driver_path = std::string(TempDir) + xorstr_("\\") + driver_image;
			const std::string mapper_path = std::string(TempDir) + xorstr_("\\") + mapper_image;

			if (DoesImageExist(driver_path.c_str()) && DoesImageExist(mapper_path.c_str()))
			{
				//if (!LI_FN(CreateProcessA).get()(mapper_path.c_str(), NULL, NULL, NULL, NULL )
			}

			return nullptr;
		}
	};
}