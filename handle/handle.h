#pragma once
#include <Windows.h>
#include <iostream>

DWORD GetPID(const std::wstring& processName);
uintptr_t GetModuleBaseAddress(DWORD processId, const std::wstring& moduleName);

// 封装一个更友好的读取内存的函数
template<typename T>
T ReadMemory(HANDLE hProcess, LPVOID address) {
    T value = 0;
    SIZE_T bytesRead;
    if (!ReadProcessMemory(hProcess, address, &value, sizeof(T), &bytesRead)) {
        // 使用FormatMessage获取更详细的错误信息
        DWORD error = GetLastError();
        LPVOID buffer;
        FormatMessage(
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,
            error,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // 默认语言
            (LPWSTR)&buffer,
            0,
            NULL
        );
        std::wcout << L"ReadMemory Failed at address: " << address << L" with error: " << error << L" - " << (LPWSTR)buffer << std::endl;
        LocalFree(buffer); // 释放buffer
        return T(); // 返回默认值
    }
    return value;
}

// 封装一个更友好的写入内存的函数
template<typename T>
bool WriteMemory(HANDLE hProcess, LPVOID address, T value) {
    SIZE_T bytesWritten;
    if (!WriteProcessMemory(hProcess, address, &value, sizeof(T), &bytesWritten)) {
        DWORD error = GetLastError();
        LPVOID buffer;
        FormatMessage(
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,
            error,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // 默认语言
            (LPWSTR)&buffer,
            0,
            NULL
        );
        std::wcout << L"WriteMemory Failed at address: " << address << L" with error: " << error << L" - " << (LPWSTR)buffer << std::endl;
        LocalFree(buffer);
        return false;
    }
    return true;
}