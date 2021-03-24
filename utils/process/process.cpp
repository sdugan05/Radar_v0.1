//
// Created by Saul on 3/23/2021.
//

#include "process.hpp"


#include <iostream>
#include <TlHelp32.h>
#include <cstdio>
#include <wchar.h>
#include <tchar.h>


auto Process::attach() -> bool
{
    process = OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION, FALSE, process_id);
    if (!process)
        return false;

    return true;
}

auto Process::detach() -> void
{
    if (process)
        CloseHandle(process);

    process = nullptr;
    process_id = NULL;
    csgo = { NULL, NULL };
    engine = { NULL, NULL };
    client = { NULL, NULL };
}

auto Process::write(DWORD dw_address, LPCVOID lpc_buffer, DWORD dw_size) -> bool
{
    return (WriteProcessMemory(process, reinterpret_cast<LPVOID>(dw_address), lpc_buffer, dw_size, nullptr) == TRUE);
}

auto Process::read(DWORD dw_address, LPVOID lp_buffer, DWORD dw_size) -> bool
{
    return (ReadProcessMemory(process, reinterpret_cast<LPCVOID>(dw_address), lp_buffer, dw_size, nullptr) == TRUE);
}

bool DataCompare(const BYTE* pData, const BYTE* pMask, const char* pszMask)
{
    for (; *pszMask; ++pszMask, ++pData, ++pMask)
    {
        if (*pszMask == 'x' && *pData != *pMask)
        {
            return false;
        }
    }

    return (*pszMask == NULL);
}

auto Process::scan(DWORD start, DWORD size, const char* signature, const char* mask) -> DWORD
{
    const auto remote = new BYTE[size];

    if (!read(start, remote, size))
    {
        delete[] remote;
        return NULL;
    }

    for (size_t i = 0; i < size; i++)
    {
        if (DataCompare(static_cast<const BYTE*>(remote + i), reinterpret_cast<const BYTE*>(signature), mask))
        {
            delete[] remote;
            return start + i;
        }
    }
    delete[] remote;

    return NULL;
}

auto Process::get_module_base_address(const char* str_module_name) -> std::array<ULONG64, 2>
{
    HANDLE h_snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, process_id);
    if (h_snapshot == INVALID_HANDLE_VALUE)
        return { NULL, NULL };

    MODULEENTRY32 module_entry;
    module_entry.dwSize = sizeof(MODULEENTRY32);

    if (Module32First(h_snapshot, &module_entry))
    {
        if (!strcmp(str_module_name, module_entry.szModule)) {
            CloseHandle(h_snapshot);
            return { reinterpret_cast<ULONG64>(module_entry.modBaseAddr), module_entry.modBaseSize };
        }
    }
    while (Module32Next(h_snapshot, &module_entry))
    {
        if (!strcmp(str_module_name, module_entry.szModule)) {
            CloseHandle(h_snapshot);
            return { reinterpret_cast<ULONG64>(module_entry.modBaseAddr), module_entry.modBaseSize };
        }
    }
    CloseHandle(h_snapshot);
    return { NULL, NULL };
}

std::array<ULONG64, 2> Process::GetModuleBaseAddress(const char* modName)
{
    uintptr_t modBaseAddr = 0;
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, process_id);
    MODULEENTRY32 modEntry;
    if (hSnap != INVALID_HANDLE_VALUE)
    {
        modEntry.dwSize = sizeof(modEntry);
        if (Module32First(hSnap, &modEntry))
        {
            do
            {
                if (!strcmp(modEntry.szModule, modName))
                {
                    modBaseAddr = (uintptr_t)modEntry.modBaseAddr;
                    break;
                }
            } while (Module32Next(hSnap, &modEntry));
        }
    }
    CloseHandle(hSnap);
    return { reinterpret_cast<ULONG64>(modBaseAddr), modEntry.modBaseSize };
}