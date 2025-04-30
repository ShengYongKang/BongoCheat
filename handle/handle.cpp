#include "handle.h"
#include <TlHelp32.h>


// 获取进程ID的函数，使用宽字符
DWORD GetPID(const std::wstring& processName) {
    DWORD processId = 0;
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot != INVALID_HANDLE_VALUE) {
        PROCESSENTRY32W entry; // 使用PROCESSENTRY32W
        entry.dwSize = sizeof(PROCESSENTRY32W);
        if (Process32FirstW(snapshot, &entry)) { // 使用Process32FirstW
            do {
                if (processName == entry.szExeFile) { // 比较宽字符串
                    processId = entry.th32ProcessID;
                    break;
                }
            } while (Process32NextW(snapshot, &entry)); // 使用Process32NextW
        }
        CloseHandle(snapshot);
    }
    return processId;
}

// 获取模块基址
uintptr_t GetModuleBaseAddress(DWORD processId, const std::wstring& moduleName) {
    uintptr_t baseAddress = 0;
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, processId);
    if (snapshot != INVALID_HANDLE_VALUE) {
        MODULEENTRY32W entry; // 使用MODULEENTRY32W
        entry.dwSize = sizeof(MODULEENTRY32W);
        if (Module32FirstW(snapshot, &entry)) { // 使用Module32FirstW
            do {
                if (moduleName == entry.szModule) { // 比较宽字符串
                    baseAddress = (uintptr_t)entry.modBaseAddr;
                    break;
                }
            } while (Module32NextW(snapshot, &entry)); // 使用Module32NextW
        }
        CloseHandle(snapshot);
    }
    return baseAddress;
}

