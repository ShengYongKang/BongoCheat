#include "address.h"
#include "../handle/handle.h"


uintptr_t GetBaseAddressTotalClicks(uintptr_t& addr_rest_clicks, DWORD pid, HANDLE hProcess)
{
    uintptr_t baseAddress = GetModuleBaseAddress(pid, L"mono-2.0-bdwgc.dll");
    if (baseAddress == 0)
    {
        std::wcout << L"[ERROR]:Failed getting TotalClicks address." << std::endl;
        return 1;
    }

    // 加上基址偏移
    uintptr_t currentAddress = baseAddress + 0x0073CBC0;

    // 逐级指针偏移
    currentAddress = ReadMemory<uintptr_t>(hProcess, (LPVOID)currentAddress) + 0xA0;
    currentAddress = ReadMemory<uintptr_t>(hProcess, (LPVOID)currentAddress) + 0x80;
    currentAddress = ReadMemory<uintptr_t>(hProcess, (LPVOID)currentAddress) + 0x38;
    currentAddress = ReadMemory<uintptr_t>(hProcess, (LPVOID)currentAddress) + 0x20;
    currentAddress = ReadMemory<uintptr_t>(hProcess, (LPVOID)currentAddress) + 0x38;
    addr_rest_clicks = currentAddress;
    // 输出最终地址
    // std::wcout << L"[LOG]:TotalClicks address: 0x" << std::hex << addr_rest_clicks << std::endl;
}
