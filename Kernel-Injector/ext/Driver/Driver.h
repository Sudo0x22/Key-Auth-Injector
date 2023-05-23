#pragma once
#include"../Pattern/Pattern.h"

extern "C"
{
	namespace driver
	{
		PVOID GetKernelModuleBaseAddress(const wchar_t* const KernelModule, LPVOID lpBaseAddress);
		PVOID HookKernelModuleBase(LPVOID Module);
		BOOL WriteToReadOnlyMemory(const DWORD address, LPVOID lpBaseAddress, LPCVOID SourceAddress);
		BOOL WriteToProcessMemory(const DWORD address, LPVOID lpBaseAddress, LPCVOID SourceAddress);
	}

	PVOID driver::GetKernelModuleBaseAddress(const wchar_t* const KernelModule, LPVOID lpBaseAddress)
	{

		return nullptr;
	}

	PVOID driver::HookKernelModuleBase(LPVOID Module)
	{
		if (!Module)
			return nullptr;

		auto ModuleEntry = reinterpret_cast<DWORD>(Module);
		DWORD lpEntryAddr = pattern::pScanner->AllocateEntryPoint((DWORD64)ModuleEntry);

		if (lpEntryAddr)
		{
			auto kernel_sig = pattern::pScanner->PatternScan(Module, "");
		}

		return Module;
	}

}