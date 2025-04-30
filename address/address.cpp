#include "address.h"
#include "../handle/handle.h"


uintptr_t GetBaseAddressTotalClicks(uintptr_t& addr_rest_clicks, DWORD pid, HANDLE hProcess)
{
    uintptr_t baseAddress = GetModuleBaseAddress(pid, L"UnityPlayer.dll");
    if (baseAddress == 0)
    {
        std::wcout << L"[ERROR]:Failed getting TotalClicks address." << std::endl;
        return 1;
    }

    // 加上基址偏移
    uintptr_t currentAddress = baseAddress + 0x01EFDE50;

    // 逐级指针偏移
    currentAddress = ReadMemory<uintptr_t>(hProcess, (LPVOID)currentAddress) + 0x38;
    currentAddress = ReadMemory<uintptr_t>(hProcess, (LPVOID)currentAddress) + 0x18;
    currentAddress = ReadMemory<uintptr_t>(hProcess, (LPVOID)currentAddress) + 0x0;
    currentAddress = ReadMemory<uintptr_t>(hProcess, (LPVOID)currentAddress) + 0x20;
    currentAddress = ReadMemory<uintptr_t>(hProcess, (LPVOID)currentAddress) + 0x40;
    currentAddress = ReadMemory<uintptr_t>(hProcess, (LPVOID)currentAddress) + 0x38;
    addr_rest_clicks = currentAddress;
    // 输出最终地址
    // std::wcout << L"[LOG]:TotalClicks address: 0x" << std::hex << addr_rest_clicks << std::endl;
}
