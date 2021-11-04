#pragma once
#include "NewCommands.h"

class CPools
{
public:
	static void* GetPed(int handle)
	{
		return CMemoryCall::Function<void*, int>(reinterpret_cast<int>(cleo->GetMainLibrarySymbol("_ZN6CPools6GetPedEi")), handle);
	}
	static void* GetVehicle(int handle)
	{
		return CMemoryCall::Function<void*, int>(reinterpret_cast<int>(cleo->GetMainLibrarySymbol("_ZN6CPools10GetVehicleEi")), handle);
	}
	static void* GetObject(int handle)
	{
		return CMemoryCall::Function<void*, int>(reinterpret_cast<int>(cleo->GetMainLibrarySymbol("_ZN6CPools9GetObjectEi")), handle);
	}
};