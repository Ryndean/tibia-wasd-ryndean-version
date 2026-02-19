#include <windows.h>
#include <time.h>

HMODULE origLibrary;
WNDPROC wndProc;
time_t lastToggleTime = 0;
bool wsadActive = false;

typedef void (*_PushLetter) (int Letter);
_PushLetter PushLetter;

bool isOnline() {
    return (*(DWORD*) 0x0071C588) == 8;
}

void SetNumLock(bool enable) {
    BYTE keyState[256];
    GetKeyboardState(keyState);
    if ((keyState[VK_NUMLOCK] & 1) != enable) {
        keybd_event(VK_NUMLOCK, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0);
        keybd_event(VK_NUMLOCK, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
    }
}

LRESULT CALLBACK HookedMessageDispatcher(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    if (!isOnline()) {
        return CallWindowProc(wndProc, hWnd, uMsg, wParam, lParam);
    }

    // Toggle WASD med ENTER
    if (uMsg == WM_KEYDOWN && wParam == VK_RETURN) {
        if (lastToggleTime + 300 < clock()) {
            wsadActive = !wsadActive;
            if (wsadActive) {
                SetNumLock(false); 
            }
        }
        lastToggleTime = clock();
        return CallWindowProc(wndProc, hWnd, uMsg, wParam, lParam);
    }

    if (uMsg == WM_KEYDOWN && wsadActive) {
        switch (wParam) {
            case 'W': wParam = VK_UP; break;
            case 'S': wParam = VK_DOWN; break;
            case 'A': wParam = VK_LEFT; break;
            case 'D': wParam = VK_RIGHT; break;
            case 'Q': wParam = VK_HOME; break;
            case 'E': wParam = VK_PRIOR; break;
            case 'Z': wParam = VK_END; break;
            case 'C': wParam = VK_NEXT; break;
        }
    }

    return CallWindowProc(wndProc, hWnd, uMsg, wParam, lParam);
}

HWND WINAPI HookedCreateWindowEx(DWORD dwExStyle, LPCTSTR lpClassName, LPCTSTR lpWindowName, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam) {
    HWND m_hWnd = CreateWindowEx(dwExStyle, lpClassName, lpWindowName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
    wndProc = (WNDPROC) GetWindowLongPtr(m_hWnd, GWL_WNDPROC);
    SetWindowLongPtr(m_hWnd, GWL_WNDPROC, (LONG_PTR) HookedMessageDispatcher);
    return m_hWnd;
}

// Prevents movement keys from being typed into the game chat
void _stdcall HookedPushLetter(int Letter) {
    if (!isOnline() || !wsadActive) {
        PushLetter(Letter);
        return;
    }

    // Filter out WASD keys
    if (Letter == 'W' || Letter == 'w') return;
    if (Letter == 'S' || Letter == 's') return;
    if (Letter == 'A' || Letter == 'a') return;
    if (Letter == 'D' || Letter == 'd') return;

    // Filter out diagonal keys (QEZC)
    if (Letter == 'Q' || Letter == 'q') return;
    if (Letter == 'E' || Letter == 'e') return;
    if (Letter == 'Z' || Letter == 'z') return;
    if (Letter == 'C' || Letter == 'c') return;

    PushLetter(Letter);
}

void HookCall(DWORD dwCallAddress, DWORD dwNewAddress) {
    DWORD dwOldProtect, dwNewProtect, dwNewCall;
    BYTE call[4];

    dwNewCall = dwNewAddress - dwCallAddress - 5;
    *(DWORD*) call = dwNewCall;

    VirtualProtect((LPVOID) (dwCallAddress + 1), 4, PAGE_EXECUTE_WRITECOPY, &dwOldProtect);
    dwCallAddress += 1;
    *(DWORD*) dwCallAddress = *(DWORD*) & call;
    VirtualProtect((LPVOID) (dwCallAddress), 5, dwOldProtect, &dwNewProtect);
}

static int InitMain() {
    char systemDirectory[MAX_PATH];
    GetSystemDirectory(systemDirectory, MAX_PATH);
    lstrcat(systemDirectory, "\\ddraw.dll");
    origLibrary = LoadLibrary(systemDirectory);

    if (!origLibrary) exit(-1);

    PushLetter = (_PushLetter) 0x00447E60;
    HookCall(0x004AB834, (DWORD) & HookedPushLetter);

    DWORD dwOldProtect, dwNewProtect, funcAddress, origAddress;
    funcAddress = (DWORD) & HookedCreateWindowEx;
    origAddress = (DWORD) ((int*) 0x0055B580);
    VirtualProtect((LPVOID) origAddress, 4, PAGE_READWRITE, &dwOldProtect);
    memcpy((LPVOID) origAddress, &funcAddress, 4);
    VirtualProtect((LPVOID) origAddress, 4, dwOldProtect, &dwNewProtect);

    return 1;
}

extern "C" {
    __declspec (dllexport) HRESULT WINAPI DirectDrawCreate(void* lpGUID, void* lplp, void* pUnkOuter) {
        FARPROC proc = GetProcAddress(origLibrary, "DirectDrawCreate");
        if (!proc) return E_INVALIDARG;
        return ((HRESULT(WINAPI *)(void*, void*, void*))(DWORD) (proc))(lpGUID, lplp, pUnkOuter);
    }

    BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved) {
        if (dwReason == DLL_PROCESS_ATTACH) return InitMain();
        return 1;
    }
}