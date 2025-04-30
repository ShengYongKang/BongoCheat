#include "address/address.h"
#include "handle/handle.h"

#include <TlHelp32.h>


//#include <iomanip> // 用于格式化输出

int add_count = 1200;  //每次允许加多少次点击数

int main()
{
    // 1. 获取进程ID
    DWORD processId = GetPID(PROCESS_NAME);
    if (processId == 0)
    {
        std::wcout << L"[ERROR]:Process not found: " << PROCESS_NAME << std::endl;
        return 1;
    }
    // std::wcout << L"[LOG]:Process ID: " << processId << std::endl;

    // 2. 获取进程句柄
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
    if (hProcess == NULL)
    {
        DWORD error = GetLastError();
        std::wcout << L"[ERROR]:Failed to open process with error: " << error << std::endl;
        return 1;
    }
    // std::wcout << L"[LOG]:Process handle obtained." << std::endl;

    // 创建保存内存地址的变量
    uintptr_t addr_rest_clicks = 0;
    uintptr_t addr_total_clicks = 0;
    GetBaseAddressTotalClicks(addr_rest_clicks, processId, hProcess);  //计算内存地址
    addr_total_clicks = addr_rest_clicks + 0x4;  //偏移+4就是总点击次数

    // 4. 读写内存中的 int 值
    int temp_rest_clicks = ReadMemory<int>(hProcess, (LPVOID)addr_rest_clicks);
    int temp_total_clicks = ReadMemory<int>(hProcess, (LPVOID)addr_total_clicks);
    

    if (WriteMemory(hProcess, (LPVOID)addr_rest_clicks, temp_rest_clicks + add_count))
    {
        std::wcout << L"[LOG]:(Rest Clicks) written successfully: " << std::dec << ReadMemory<int>(hProcess, (LPVOID)addr_rest_clicks) << std::endl;
    }
    else
    {
        std::wcout << L"[ERROR]:Failed to write (Rest Clicks)." << std::endl;
    }

    if (WriteMemory(hProcess, (LPVOID)addr_total_clicks, temp_total_clicks + add_count))
    {
        std::wcout << L"[LOG]:(Total Clicks) written successfully:" << std::dec << ReadMemory<int>(hProcess, (LPVOID)addr_total_clicks) << std::endl;
    }
    else
    {
        std::wcout << L"[ERROR]:Failed to write (Total Clicks)." << std::endl;
    }


    // 关闭进程句柄
    CloseHandle(hProcess);
    std::wcout << L"[MISSION COMPLETED]." << std::endl;

    return 0;
}
