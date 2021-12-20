#pragma once
#include "Windows.h"
#include <memory>
//the virtual key code for X key
static const int X_KEY = 0x58;
namespace KeyboardHook {
    LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
    {
        //return immidentialy if ncode is less than 0
        if (nCode < 0) {
            return CallNextHookEx(NULL, nCode, wParam, lParam);
        }
        //check for a key down
        if (wParam == WM_KEYDOWN) {
            KBDLLHOOKSTRUCT pKeyBoard = *reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);
            //check for X key
            if (pKeyBoard.vkCode == X_KEY) {
                
            }
        }
        return CallNextHookEx(0, nCode, wParam, lParam);
    }
    static void init()
    {
        //boiler plate to set up a keyboard hook
        HHOOK hook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, GetModuleHandle(0), 0);
        MSG msg;
        while (GetMessage(&msg, NULL, 0, 0) > 0)
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        UnhookWindowsHookEx(hook);
    }
}
