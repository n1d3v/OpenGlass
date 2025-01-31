// ==WindhawkMod==
// @id              dwm-tweaker
// @name            DWM Tweaker
// @description     Choose what Aero Glass framework you want.
// @version         1.1
// @author          Tech Stuff
// @github          https://github.com/teknixstuff
// @homepage        https://teknixstuff.com/
// @architecture    x86-64
// @include         dwm.exe
// @include         winlogon.exe
// @include         explorer.exe
// @compilerOptions -lntdll -ldwmapi
// ==/WindhawkMod==

// ==WindhawkModReadme==
/*
# DWM Tweaker
A simple tool created by Tech Stuff to switch what Aero framework you want to use.

Modified by AeroDoggy to support other frameworks, and also this one to only have the option for the forked OpenGlass mod!

## Warning!!
If you didn't read the installation guide and is wondering why the hell isn't OpenGlass working? You may have not enabled `DisableGlassInjection` in `HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\DWM` which you can do in `regedit.exe`. You will need to create this DWORD.

### Credits
- ALTaleX531: OpenGlass (Original) creator, and mostly did the hard work for the fork.
- patricktbp: OpenGlass (Optimized) creator, did the optimizations for the fork.
- Tech Stuff: Made the optimized fork have Windhawk support and discovered the original reflection optimization. Also made DWM Tweaker.
- AeroDoggy: Made a fork of DWM Tweaker to support other frameworks, and made this one for this fork of OpenGlass.
*/
// ==/WindhawkModReadme==

// ==WindhawkModSettings==
/*
- legacyoptimized: true
  $name: Use Optimized OpenGlass Legacy (V1.4)
  $description: Use a forked version of OpenGlass Legacy optimized thanks to patricktbp and Tech Stuff's efforts.
*/
// ==/WindhawkModSettings==

#include <windhawk_api.h>
#include <windhawk_utils.h>
#include <winver.h>
#include <errhandlingapi.h>
#include <libloaderapi.h>
#include <minwinbase.h>
#include <processenv.h>
#include <processthreadsapi.h>
#include <synchapi.h>
#include <windhawk_api.h>
#include <windows.h>
#include <winreg.h>
#include <wtsapi32.h>
#include <string>
#include <minwindef.h>
#include <windows.h>
#include <winerror.h>
#include <winreg.h>
#include <string>
#include <dwmapi.h>

extern "C" __declspec(dllimport) void WINAPI RtlGetNtVersionNumbers(
    ULONG* pNtMajorVersion,
    ULONG* pNtMinorVersion,
    ULONG* pNtBuildNumber
); DWORD WinBuild;

BOOL Wh_ModSettingsChanged(BOOL* bReload)
{
    Wh_Log(L"Settings changed");

    *bReload = TRUE;
    if (GetModuleHandle(L"dwm.exe")) {
        TerminateProcess(GetCurrentProcess(), -1);
    }
    return TRUE;
}

typedef LONG NTSTATUS;

#ifndef STATUS_SUCCESS
#define STATUS_SUCCESS ((NTSTATUS)0x00000000L)
#endif

#ifndef STATUS_BUFFER_TOO_SMALL
#define STATUS_BUFFER_TOO_SMALL ((NTSTATUS)0xC0000023L)
#endif

std::wstring GetKeyPathFromHKEY(HKEY key)
{
    std::wstring keyPath;
    if (key != NULL)
    {
        HMODULE dll = LoadLibrary(L"ntdll.dll");
        if (dll != NULL) {
            typedef DWORD(__stdcall* NtQueryKeyType)(
                HANDLE  KeyHandle,
                int KeyInformationClass,
                PVOID  KeyInformation,
                ULONG  Length,
                PULONG  ResultLength);

            NtQueryKeyType func = reinterpret_cast<NtQueryKeyType>(::GetProcAddress(dll, "NtQueryKey"));

            if (func != NULL) {
                DWORD size = 0;
                DWORD result = 0;
                result = func(key, 3, 0, 0, &size);
                if (result == (DWORD)STATUS_BUFFER_TOO_SMALL)
                {
                    size = size + 2;
                    wchar_t* buffer = new (std::nothrow) wchar_t[size / sizeof(wchar_t)]; // size is in bytes
                    if (buffer != NULL)
                    {
                        result = func(key, 3, buffer, size, &size);
                        if (result == STATUS_SUCCESS)
                        {
                            buffer[size / sizeof(wchar_t)] = L'\0';
                            keyPath = std::wstring(buffer + 2);
                        }

                        delete[] buffer;
                    }
                }
            }

            FreeLibrary(dll);
        }
    }

    if (keyPath.substr(0, 17) == L"\\REGISTRY\\MACHINE") {
        keyPath = keyPath.substr(17).insert(0, L"HKEY_LOCAL_MACHINE");
    }

    if (keyPath.substr(0, 23) == L"\\REGISTRY\\USER\\.DEFAULT") {
        keyPath = keyPath.substr(23).insert(0, L"HKEY_CURRENT_USER");
    }

    if (keyPath.substr(0, 18) == L"\\REGISTRY\\USER\\S-1") {
        size_t SIDEndPos = keyPath.find(L"\\", 18);
        if (SIDEndPos == std::wstring::npos) {
            SIDEndPos = keyPath.length();
        }
        if (keyPath.substr(SIDEndPos - 8, 8) == L"_CLASSES") {
            keyPath = keyPath.substr(SIDEndPos).insert(0, L"HKEY_CURRENT_USER\\Software\\Classes");
        }
        else {
            keyPath = keyPath.substr(SIDEndPos).insert(0, L"HKEY_CURRENT_USER");
        }
    }

    return keyPath;
}

LSTATUS(WINAPI* pOriginalRegQueryValueExW)(HKEY hKey, LPCWSTR lpValueName, LPDWORD lpReserved, LPDWORD lpType, LPBYTE lpData, LPDWORD lpcbData);
LSTATUS WINAPI RegQueryValueExWHook(HKEY hKey, LPCWSTR lpValueName, LPDWORD lpReserved, LPDWORD lpType, LPBYTE lpData, LPDWORD lpcbData) {
    if (_wcsicmp(GetKeyPathFromHKEY(hKey).c_str(), L"HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\DWM") == 0) {
        HKEY hKey2;
        RegOpenKeyExW(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows\\DWM", 0, KEY_READ, &hKey2);
        LSTATUS ret;
        if (pOriginalRegQueryValueExW(hKey, lpValueName, NULL, NULL, NULL, NULL) == ERROR_FILE_NOT_FOUND) {
            Wh_Log(L"Using redirected value for %s", lpValueName);
            ret = pOriginalRegQueryValueExW(hKey2, lpValueName, lpReserved, lpType, lpData, lpcbData);
        }
        else {
            Wh_Log(L"Using original value for %s", lpValueName);
            ret = pOriginalRegQueryValueExW(hKey, lpValueName, lpReserved, lpType, lpData, lpcbData);
        }
        CloseHandle(hKey2);
        return ret;
    }
    return pOriginalRegQueryValueExW(hKey, lpValueName, lpReserved, lpType, lpData, lpcbData);
}

struct ACCENTPOLICY
{
    int nAccentState;
    int nFlags;
    int nColor;
    int nAnimationId;
};

enum WINDOWCOMPOSITIONATTRIB
{
    WCA_ACCENT_POLICY = 19
};

struct WINDOWCOMPOSITIONATTRIBDATA
{
    WINDOWCOMPOSITIONATTRIB Attrib;
    PVOID pvData;
    SIZE_T cbData;
};

struct ACCENTPOLICY policy = {
    0,
    0,
    0,
    0
};

using fnSetWindowCompositionAttribute = BOOL(WINAPI*)(HWND hWnd, WINDOWCOMPOSITIONATTRIBDATA*);

fnSetWindowCompositionAttribute _SetWindowCompositionAttribute = nullptr;

void EnableBlurBehind() {

    HWND Start = FindWindowA("DV2ControlHost", NULL);
    HWND TaskBand = FindWindowA("Shell_TrayWnd", NULL);
    HWND TaskBand2 = FindWindowA("Shell_SecondaryTrayWnd", NULL);


    WINDOWCOMPOSITIONATTRIBDATA data = { WCA_ACCENT_POLICY, &policy, sizeof(policy) };
    _SetWindowCompositionAttribute(Start, &data);
    _SetWindowCompositionAttribute(TaskBand, &data);
    _SetWindowCompositionAttribute(TaskBand2, &data);

    // Create and populate the Blur Behind structure
    DWM_BLURBEHIND bb = { 0 };

    // Enable Blur Behind and apply to the entire client area
    bb.dwFlags = DWM_BB_ENABLE;
    bb.fEnable = true;
    bb.hRgnBlur = NULL;

    // Apply Blur Behind
    DwmEnableBlurBehindWindow(Start, &bb);
    DwmEnableBlurBehindWindow(TaskBand, &bb);
    DwmEnableBlurBehindWindow(TaskBand2, &bb);
}

//Hook window creation so that blur survives a restart of explorer
using CreateWindowExW_t = decltype(&CreateWindowExW);
CreateWindowExW_t pOriginalCreateWindowExW;
HWND WINAPI CreateWindowExWHook(
    DWORD dwExStyle,
    LPCWSTR lpClassName,
    LPCWSTR lpWindowName,
    DWORD dwStyle,
    int X,
    int Y,
    int nWidth,
    int nHeight,
    HWND hWndParent,
    HMENU hMenu,
    HINSTANCE hInstance,
    LPVOID lpParam)
{
    HWND hWnd = pOriginalCreateWindowExW(
        dwExStyle,
        lpClassName,
        lpWindowName,
        dwStyle,
        X,
        Y,
        nWidth,
        nHeight,
        hWndParent,
        hMenu,
        hInstance,
        lpParam
    );

    if (!hWnd)
        return hWnd;

    EnableBlurBehind();

    return hWnd;
}

HMODULE hLibGlass;

HANDLE hStyleCheckThread;
HANDLE hOpenGlassHostProc;

BOOL Wh_ModInit() {
    Wh_Log(L"Init");

    RtlGetNtVersionNumbers(NULL, NULL, &WinBuild);
    WinBuild &= ~0xF0000000;

    if (GetModuleHandle(L"dwm.exe")) {
        WH_FIND_SYMBOL_OPTIONS symbolOptions;
        symbolOptions.optionsSize = sizeof(symbolOptions);
        symbolOptions.symbolServer = NULL;
        WH_FIND_SYMBOL symbol;
        HANDLE hSymbol;
        Wh_Log(L"Getting udwm.dll symbol");
        hSymbol = Wh_FindFirstSymbol(LoadLibrary(L"udwm.dll"), &symbolOptions, &symbol);
        Wh_FindCloseSymbol(hSymbol);
        Wh_Log(L"Getting dwmcore.dll symbol");
        hSymbol = Wh_FindFirstSymbol(LoadLibrary(L"dwmcore.dll"), &symbolOptions, &symbol);
        Wh_FindCloseSymbol(hSymbol);
        Wh_SetFunctionHook((void*)GetProcAddress(LoadLibrary(L"kernelbase.dll"), "RegQueryValueExW"), (void*)RegQueryValueExWHook, (void**)&pOriginalRegQueryValueExW);
        if (Wh_GetIntSetting(L"legacyoptimized")) {
            hLibGlass = LoadLibrary(L"C:\\programdata\\Windhawk\\Engine\\ModsWritable\\OpenGlassOpt.dll");
        }
    }
    if (GetModuleHandle(L"winlogon.exe")) {
        if (Wh_GetIntSetting(L"legacyoptimized")) {
            STARTUPINFO Si;
            ZeroMemory(&Si, sizeof(Si));
            Si.cb = sizeof(Si);
            PROCESS_INFORMATION Pi;
            ZeroMemory(&Pi, sizeof(Pi));
            WCHAR cmdline[] = L"rundll32 C:\\programdata\\Windhawk\\Engine\\ModsWritable\\OpenGlassOpt.dll,StartupService";
            CreateProcess(NULL, cmdline, NULL, NULL, FALSE, 0, NULL, NULL, &Si, &Pi);
        }
    }

    return TRUE;
}

void Wh_ModUninit() {
    Wh_Log(L"Uninit");
    if (GetModuleHandle(L"dwm.exe")) {
        TerminateThread(hStyleCheckThread, 0);
        FreeLibrary(hLibGlass);
    }
    if (GetModuleHandle(L"winlogon.exe")) {
        TerminateProcess(hOpenGlassHostProc, 0);
    }
}