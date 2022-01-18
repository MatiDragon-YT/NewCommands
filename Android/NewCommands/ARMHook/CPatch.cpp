
/*
* Copyright (C) fastman92 <fastman92@gmail.com>, website: http://fastman92.com
* Licensed under the MIT License, see LICENSE at top level directory.
*
*/

#include "CPatch.h"
#include "MemoryPermission\MemoryPermission.h"
#include <sys/mman.h>
#include <errno.h>
#include <unistd.h>
#include <dlfcn.h>

// Patches memory data
void CPatch::PatchMemoryData(uintptr_t dwAddress, const void* bData, int iSize)
{
	WriteDataToMemory((void*)dwAddress, bData, iSize);
}

// Patches UINT8 value
void CPatch::PatchUINT8(uintptr_t dwAddress, uint8_t to)
{		
	WriteDataToMemory((void*)dwAddress, &to, sizeof(to));
}

// Patches UINT16 value
void CPatch::PatchUINT16(uintptr_t dwAddress, uint16_t to)
{
	WriteDataToMemory((void*)dwAddress, &to, sizeof(to));
}

// Patches UINT32 value
void CPatch::PatchUINT32(uintptr_t dwAddress, uint32_t to)
{
	WriteDataToMemory((void*)dwAddress, &to, sizeof(to));
}

// Patches pointer
void CPatch::PatchPointer(uintptr_t dwAddress, const void* to)
{
	WriteDataToMemory((void*)dwAddress, &to, sizeof(to));
}

// Patches float value
void CPatch::PatchFloat(uintptr_t dwAddress, float to)
{
	WriteDataToMemory((void*)dwAddress, &to, sizeof(to));
}

// NOPs instruction (0x90)
void CPatch::NOPinstructions(eInstructionSet sourceInstructionSet, uintptr_t dwAddress, int iSize)
{
	tMemoryPermissionChangeRequest request;
	request.lpAddress = (void*)dwAddress;
	request.dwSize = iSize;
	request.flNewProtect = GetNativeNewProtect(F92_MEM_PAGE_EXECUTE_READWRITE);

	SetMemoryPermission(&request);
	if (sourceInstructionSet == INSTRUCTION_SET_THUMB)
	{
		for (int i = 0; i < iSize; i += 2)
		{
			*((unsigned char*)dwAddress + i + 0) = 0x00;
			*((unsigned char*)dwAddress + i + 1) = 0xBF;
		}
	}
	else if (sourceInstructionSet == INSTRUCTION_SET_ARM)
	{
		for (int i = 0; i < iSize; i += 4)
		{
			*((unsigned char*)dwAddress + i + 0) = 0x00;
			*((unsigned char*)dwAddress + i + 1) = 0xF0;
			*((unsigned char*)dwAddress + i + 2) = 0x20;
			*((unsigned char*)dwAddress + i + 3) = 0xE3;
		}
	}
	// Restore old permission
	if (request.bIsOldProtectSet)
	{
		request.flNewProtect = request.lpflOldProtect;
		SetMemoryPermission(&request);
	}
}

void CPatch::RedirectCodeEx(eInstructionSet sourceInstructionSet, uintptr_t dwAddress, uintptr_t to)
{
	return CPatch::RedirectCodeEx(sourceInstructionSet, dwAddress, (const void*)to);
}

void CPatch::RedirectCodeEx(eInstructionSet sourceInstructionSet, uintptr_t dwAddress, const void* to)
{
	// Thumb trampoline may take 8 bytes (if address is aligned to value of 4) or 10 bytes.
	if (sourceInstructionSet == INSTRUCTION_SET_THUMB)
	{
		char code[12];
		unsigned int sizeOfData = 0;

		if (dwAddress % 4 == 0)
		{
			*(uint32_t*)(code + 0) = 0xF000F8DF;	// // LDR.W             R0, [PC, #0]
			*(const void**)(code + 4) = to;	// pointer, where to jump
			sizeOfData = 8;
		}
		else
		{
			*(uint32_t*)(code + 0) = 0xBF00;	// NOP
			*(uint32_t*)(code + 2) = 0xF000F8DF;	// // LDR.W             R0, [PC, #0]
			
			*(const void**)(code + 6) = to;	// pointer, where to jump
			sizeOfData = 10;
		}

		CPatch::PatchMemoryData(dwAddress, code, sizeOfData);
	}
	else if (sourceInstructionSet == INSTRUCTION_SET_ARM)
	{
		char code[8];

		*(uint32_t*)(code + 0) = 0xE51FF004;	// // LDR             R0, [PC, #0]
		*(const void**)(code + 4) = to;	// pointer, where to jump
		CPatch::PatchMemoryData(dwAddress, code, sizeof(code));
	}
}

void CPatch::RedirectFunction(uintptr_t functionJumpAddress, void* to)
{
	CPatch::RedirectCodeEx(
		GET_INSTRUCTION_SET_FROM_ADDRESS(functionJumpAddress),
		GET_CODE_START(functionJumpAddress),
		to
	);
}

// Makes a function return
void CPatch::DisableFunctionByName(const char* name)
{
	uintptr_t address = (uintptr_t)dlsym(NULL, name);
	if (!address)
		return;

	if (address % 2)	// is THUMB?
	{
		address = address & (~1);
		CPatch::PatchUINT16(address, 0x4770);	// BX LR
	}
}

// Writes data to memory
// For memory, which is marked as read-only
// The code sections are marked read-only
// So if you want to change the code, use this function
// dwAddress - memory address, where to write the data
// bData - pointer to data
// iSize - size of data in bytes
void CPatch::WriteDataToMemory(void* dwAddress, const void* bData, int iSize)
{
	tMemoryPermissionChangeRequest request;
	request.lpAddress = dwAddress;
	request.dwSize = iSize;
	request.flNewProtect = GetNativeNewProtect(F92_MEM_PAGE_EXECUTE_READWRITE);

	SetMemoryPermission(&request);
	
	memcpy(dwAddress, bData, iSize);

	// Restore old permission
	if (request.bIsOldProtectSet)
	{
		request.flNewProtect = request.lpflOldProtect;
		SetMemoryPermission(&request);
	}
}