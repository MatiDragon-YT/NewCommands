/*
* Copyright (C) fastman92 <fastman92@gmail.com>, website: http://fastman92.com
* Licensed under the MIT License, see LICENSE at top level directory.
*
*/

#pragma once
#include "Assembly/eInstructionSet.h"
#include "Assembly/UsefulMacros.h"
#include <stdint.h>
#include <vector>

////////////// Patching functions //////////////////

class CPatch
{
public:

	// Writes data to memory
	static void WriteDataToMemory(void* dwAddress, const void* bData, int iSize);
	
	// Patches memory data
	static void PatchMemoryData(uintptr_t dwAddress, const void* bData, int iSize);

	// Patches UINT8 value
	static void PatchUINT8(uintptr_t dwAddress, uint8_t to);

	// Patches UINT16 value
	static void PatchUINT16(uintptr_t dwAddress, uint16_t to);

	// Patches UINT32 value
	static void PatchUINT32(uintptr_t dwAddress, uint32_t to);

	// Patches pointer
	static void PatchPointer(uintptr_t dwAddress, const void* to);

	// Patches float value
	static void PatchFloat(uintptr_t dwAddress, float to);

	// NOP instructions
	static void NOPinstructions(eInstructionSet sourceInstructionSet, uintptr_t dwAddress, int iSize);

	static void RedirectCodeEx(eInstructionSet sourceInstructionSet, uintptr_t dwAddress, uintptr_t to);

	static void RedirectCodeEx(eInstructionSet sourceInstructionSet, uintptr_t dwAddress, const void* to);

	// Redirects a method
	template<typename T> static void RedirectMethod(uintptr_t dwAddress, T method)
	{
		RedirectCode(dwAddress, GetMethodAsVoidPointer(method));
	}

	static void RedirectFunction(uintptr_t functionJumpAddress, void* to);

	// Makes a function return
	static void DisableFunctionByName(const char* name);
};