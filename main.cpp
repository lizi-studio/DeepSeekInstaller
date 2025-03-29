#include <windows.h>
#include <shellapi.h>
#include <stdio.h>
#include <string>
#include <wtsapi32.h>
#include <userenv.h>

using namespace std;

#pragma comment(lib, "Userenv.lib")
#pragma comment(lib, "Wtsapi32.lib")

#define ID_BUTTON_1 101
#define ID_BUTTON_2 102
#define ID_BUTTON_3 103
#define ID_BUTTON_4 104
#define ID_BUTTON_5 105
#define ID_BUTTON_6 106
#define ID_BUTTON_7 107
#define ID_BUTTON_8 108
#define ID_BUTTON_9 109

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void CheckOllamaInstallation(HWND hwnd);
void RunCommand(const char* cmd);
bool MyCreateProcessAsUser(const wstring& path, const wstring& cmd, const bool bShowWnd, wstring& strError);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow) {
    static TCHAR szAppName[] = TEXT("Ollama Deployer");
    HWND hwnd;
    MSG msg;
    WNDCLASS wndclass;

    wndclass.style = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc = WndProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = szAppName;

    if (!RegisterClass(&wndclass)) {
        MessageBox(NULL, TEXT("This program requires Windows NT!"), szAppName, MB_ICONERROR);
        return 0;
    }

    hwnd = CreateWindow(szAppName, TEXT("Ollama Deployer"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

    ShowWindow(hwnd, iCmdShow);
    UpdateWindow(hwnd);

    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    wstring strError;
    switch (message) {
    case WM_CREATE:
        CheckOllamaInstallation(hwnd);
        break;
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case ID_BUTTON_1:
            //RunCommand("cmd.exe /k echo Running 1.5b model");
            //MyCreateProcessAsUser(L"", L"cmd.exe /k echo Running 1.5b model", true, strError);
            system("cmd.exe /k ollama run deepseek-r1:1.5b");
            break;
        case ID_BUTTON_2:
            system("cmd.exe /k ollama run deepseek-r1:7b");
            break;
        case ID_BUTTON_3:
            system("cmd.exe /k ollama run deepseek-r1:8b");
            break;
        case ID_BUTTON_4:
            system("cmd.exe /k ollama run deepseek-r1:14b");
            break;
        case ID_BUTTON_5:
            system("cmd.exe /k ollama run deepseek-r1:32b");
            break;
        case ID_BUTTON_6:
            system("cmd.exe /k ollama run deepseek-r1:70b");
            break;
        case ID_BUTTON_7:
            system("cmd.exe /k ollama run deepseek-r1:671b");
            break;
        case ID_BUTTON_8:
            system("cmd.exe /k echo 你确定要删除所有模型吗（此过程不可逆）");
            system("cmd.exe /k ollama rm deepseek-r1:1.5b");
            system("cmd.exe /k ollama rm deepseek-r1:7b");
            system("cmd.exe /k ollama rm deepseek-r1:8b");
            system("cmd.exe /k ollama rm deepseek-r1:14b");
            system("cmd.exe /k ollama rm deepseek-r1:32b");
            system("cmd.exe /k ollama rm deepseek-r1:70b");
            system("cmd.exe /k ollama rm deepseek-r1:671b");
            break;
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, message, wParam, lParam);
    }
    return 0;
}

void CheckOllamaInstallation(HWND hwnd) {
    // Simulate checking if Ollama is installed
    //HKEY_CURRENT_USER\SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\{44E83376-CE68-45EB-8FC1-393500EB558C}_is1
    bool isInstalled = false;
    wstring strSubkey = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\{44E83376-CE68-45EB-8FC1-393500EB558C}_is1";
    bool bRet = true;
    HKEY hSubKey;

    LSTATUS lStatus = ::RegOpenKeyExW(HKEY_CURRENT_USER, strSubkey.c_str(), 0, KEY_READ , &hSubKey);
    if (ERROR_SUCCESS == lStatus)
    {
        isInstalled = true;
    }

    if (!isInstalled) {
        int result = MessageBox(hwnd, TEXT("没有安装过ollama，是否安装？"), TEXT("安装提示"), MB_YESNO);
        if (result == IDYES) {
            ShellExecuteA(NULL, "open", "llama.exe", NULL, NULL, SW_SHOWNORMAL);
            MessageBox(hwnd, TEXT("请按要求完成安装后重启软件和电脑"), TEXT("安装提示"), MB_OK);
            isInstalled = 1;
            ExitProcess(0);
        }
        else {
            MessageBox(hwnd, TEXT("未安装ollama，部署失败"), TEXT("安装提示"), MB_OK);
            Sleep(3000);
            ExitProcess(0);
        }
    }
    else {
        // Create buttons for model selection
        CreateWindow(TEXT("BUTTON"), TEXT("1.5b"), WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 10, 10, 100, 30, hwnd, (HMENU)ID_BUTTON_1, NULL, NULL);
        CreateWindow(TEXT("BUTTON"), TEXT("7b"), WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 120, 10, 100, 30, hwnd, (HMENU)ID_BUTTON_2, NULL, NULL);
        CreateWindow(TEXT("BUTTON"), TEXT("8b"), WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 230, 10, 100, 30, hwnd, (HMENU)ID_BUTTON_3, NULL, NULL);
        CreateWindow(TEXT("BUTTON"), TEXT("14b"), WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 10, 50, 100, 30, hwnd, (HMENU)ID_BUTTON_4, NULL, NULL);
        CreateWindow(TEXT("BUTTON"), TEXT("32b"), WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 120, 50, 100, 30, hwnd, (HMENU)ID_BUTTON_5, NULL, NULL);
        CreateWindow(TEXT("BUTTON"), TEXT("70b"), WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 230, 50, 100, 30, hwnd, (HMENU)ID_BUTTON_6, NULL, NULL);
        CreateWindow(TEXT("BUTTON"), TEXT("671b"), WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 10, 90, 100, 30, hwnd, (HMENU)ID_BUTTON_7, NULL, NULL);
        CreateWindow(TEXT("BUTTON"), TEXT("DEL ALL(all of them will del)"), WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 120, 90, 210, 30, hwnd, (HMENU)ID_BUTTON_8, NULL, NULL);
        CreateWindow(TEXT("BUTTON"), TEXT("提示：按下DEL ALL按钮会删除所有已经安装的模型|||请根据电脑配置安装模型|||小心不要点到DEL ALL按钮，这会导致所有模型被删除（无法恢复），如果真的不小心点到了就用任务管理器结束这个窗口|||"), WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 10, 130, 1400, 60, hwnd, (HMENU)ID_BUTTON_9, NULL, NULL);
    }
}

void RunCommand(const char* cmd) {
    ShellExecuteA(NULL, "open", cmd, NULL, NULL, SW_SHOWNORMAL);
}

bool MyCreateProcessAsUser(const wstring& path, const wstring& cmd, const bool bShowWnd, wstring& strError)
{
    HANDLE hToken = NULL;

    DWORD dwSessId = WTSGetActiveConsoleSessionId();
    if (!WTSQueryUserToken(dwSessId, &hToken))
    {
        DWORD dwError = GetLastError();
        wchar_t buff[MAX_PATH] = { 0 };
        wsprintfW(buff, L"WTSQueryUserToken error:%d", dwError);
        strError = buff;
        return false;
    }

    PROCESS_INFORMATION pi = { 0 };

    STARTUPINFOW si = { 0 };
    si.cb = sizeof(STARTUPINFO);
    si.lpDesktop = (LPWSTR)L"winsta0\\default";
    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = bShowWnd ? 1 : 0;

    wchar_t* szCmd = new wchar_t[cmd.length() + 1];
    if (szCmd == NULL)
    {
        return false;
    }
    memset(szCmd, 0, cmd.length() + 1);
    wsprintfW(szCmd, L"%s", cmd.c_str());

    // 不传递Env会导致启动的程序取不到用户文件夹，如：SHGetFolderPath失败
    LPVOID pEnv = NULL;
    if (!CreateEnvironmentBlock(&pEnv, hToken, FALSE))
    {
        DWORD dwError = GetLastError();
        wchar_t buff[MAX_PATH] = { 0 };
        wsprintfW(buff, L"WTSQueryUserToken error:%d", dwError);
        strError = buff;
        return false;
    }

    bool bRet = true;
    if (false == CreateProcessAsUserW(hToken,
        path.c_str(),
        szCmd,
        NULL,
        NULL,
        FALSE,
        NORMAL_PRIORITY_CLASS | CREATE_UNICODE_ENVIRONMENT,
        pEnv,
        NULL,
        &si,
        &pi))
    {
        DWORD dwError = GetLastError();
        wchar_t buff[MAX_PATH] = { 0 };
        wsprintfW(buff, L"WTSQueryUserToken error:%d", dwError);
        strError = buff;
        bRet = false;
    }

    DestroyEnvironmentBlock(pEnv);

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    CloseHandle(hToken);
    if (szCmd)
    {
        delete[] szCmd;
        szCmd = NULL;
    }

    return bRet;
}

