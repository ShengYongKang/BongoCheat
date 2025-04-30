#pragma once
#include <Windows.h>
#include <iostream>


// 定义一些常量，方便后续修改
const std::wstring PROCESS_NAME = L"BongoCat.exe"; // 进程名，注意这里使用宽字符串

uintptr_t GetBaseAddressTotalClicks(uintptr_t& addr_rest_clicks, DWORD pid, HANDLE hProcess);
