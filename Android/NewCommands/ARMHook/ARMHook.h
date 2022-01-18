#pragma once
#include <vector>
#include <sys/mman.h>

struct bytePattern
{
	struct byteEntry
	{
		uint8_t nValue;
		bool bUnknown;
	};
	std::vector<byteEntry> vBytes;
};

class ARMHook
{
public:
	static uintptr_t GetLibraryAddress(const char* library);
	static uintptr_t GetLibraryLength(const char* library);
	static uintptr_t GetSymbolAddress(uintptr_t LibAddr, const char* name);
	static void Unprotect(uintptr_t ptr, size_t size);
	static void WriteMemory(void* addr, void* data, size_t size);
	static void* ReadMemory(void* addr, void* data, size_t size);
	static uintptr_t GetAddressFromPatter(const char* pattern, const char* library);
	//static uintptr_t RedirectBLX(void* addr, void* func, bool vp);
};