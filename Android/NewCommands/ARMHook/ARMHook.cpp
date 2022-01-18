#include "ARMHook.h"
#include <dlfcn.h>
#include <ctype.h>

using namespace std;

uintptr_t ARMHook::GetLibraryAddress(const char* library)
{
    char filename[0xFF] = {0}, buffer[2048] = {0};
    FILE* fp = 0;
    uintptr_t address = 0;

    sprintf(filename, "/proc/%d/maps", getpid());
    fp = fopen(filename, "rt");
    if (fp == 0)
        goto done;
    while (fgets(buffer, sizeof(buffer), fp))
    {
        if (strstr(buffer, library))
        {
            address = (uintptr_t)strtoul(buffer, 0, 16);
            break;
        }
    }

done:
    if (fp)
        fclose(fp);
    return address;
}

uintptr_t ARMHook::GetLibraryLength(const char* library)
{
    char filename[0xFF] = { 0 }, buffer[2048] = { 0 };
    FILE* fp = 0;
    uintptr_t address = 0;

    sprintf(filename, "/proc/%d/maps", getpid());
    fp = fopen(filename, "rt");
    if (fp == 0)
        goto done;
    while (fgets(buffer, sizeof(buffer), fp))
    {
        if (strstr(buffer, library))
        {
            address = (uintptr_t)strtoul(buffer, 0, 16);
            address = (uintptr_t)strtoul(&buffer[9], 0, 16) - address;
            break;
        }
    }

done:
    if (fp)
        fclose(fp);
    return address;
}

uintptr_t ARMHook::GetSymbolAddress(uintptr_t LibAddr, const char* name)
{
    Dl_info info;
    if (dladdr((void*)LibAddr, &info) == 0)
        return 0;
    return (uintptr_t)dlsym(info.dli_fbase, name);
}

void ARMHook::Unprotect(uintptr_t addr, size_t size)
{
    mprotect((void*)(addr & 0xFFFFF000), size, PROT_READ | PROT_WRITE | PROT_EXEC);
}

void ARMHook::WriteMemory(void* addr, void* data, size_t size)
{
    ARMHook::Unprotect((uintptr_t)addr, size);
    memcpy(addr, data, size);
    cacheflush((uintptr_t)addr, (uintptr_t)data + size, 0);
}

void* ARMHook::ReadMemory(void* addr, void* data, size_t size)
{
    ARMHook::Unprotect((uintptr_t)addr, size);
    ARMHook::Unprotect((uintptr_t)data, size);
    memcpy(data, addr, size);
    return data;
}

bool compareData(const uint8_t* data, const bytePattern::byteEntry* pattern, size_t patternlength)
{
    int index = 0;
    for (size_t i = 0; i < patternlength; i++)
    {
        auto byte = *pattern;
        if (!byte.bUnknown && *data != byte.nValue) return false;

        ++data;
        ++pattern;
        ++index;
    }
    return index == patternlength;
}

uintptr_t ARMHook::GetAddressFromPatter(const char* pattern, const char* library)
{
    bytePattern ret;
    const char* input = &pattern[0];
    while (*input)
    {
        bytePattern::byteEntry entry;
        if (isspace(*input)) ++input;
        if (isxdigit(*input))
        {
            entry.bUnknown = false;
            entry.nValue = (uint8_t)std::strtol(input, NULL, 16);
            input += 2;
        }
        else
        {
            entry.bUnknown = true;
            input += 2;
        }
        ret.vBytes.emplace_back(entry);
    }

    auto patternstart = ret.vBytes.data();
    auto length = ret.vBytes.size();

    uintptr_t pMemoryBase = GetLibraryAddress(library);
    size_t nMemorySize = GetLibraryLength(library) - length;

    for (size_t i = 0; i < nMemorySize; i++)
    {
        uintptr_t addr = pMemoryBase + i;
        if (compareData((const uint8_t*)addr, patternstart, length)) return addr;
    }
    return (uintptr_t)0;
}

/*
uintptr_t ARMHook::RedirectBLX(void* addr, void* func, bool vp)
{
    int offset = ((int)func - (int)addr - 4) & 0x007FFFFF;
    int high = offset >> 12;
    int low = (offset & 0x00000FFF) >> 1;
    if (low % 2 != 0) {
        low++;
    }
    int machineCode = ((0xE800 | low) << 16) | (0xF000 | high);
    ARMHook::WriteMemory(addr, &machineCode, 4, vp);
    return machineCode;
}*/