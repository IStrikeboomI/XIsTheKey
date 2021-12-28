#pragma once
#include <Windows.h>
#include <gdiplus.h>
#include <string>
#pragma comment (lib,"gdiplus.lib")
LRESULT CALLBACK windowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
Gdiplus::Bitmap* img = nullptr;
namespace Splash {
    void LoadImageFromResource()
    {
        IStream* pStream = nullptr;
        HGLOBAL hGlobal = nullptr;

        //125 is the location of the image
        HRSRC hrsrc = FindResourceW(GetModuleHandle(0), MAKEINTRESOURCE(125), L"PNG");     // get the handle to the resource
        if (hrsrc)
        {
            DWORD dwResourceSize = SizeofResource(NULL, hrsrc);
            if (dwResourceSize > 0)
            {
                HGLOBAL hGlobalResource = LoadResource(NULL, hrsrc); // load it
                if (hGlobalResource)
                {
                    void* imagebytes = LockResource(hGlobalResource); // get a pointer to the file bytes

                    // copy image bytes into a real hglobal memory handle
                    hGlobal = ::GlobalAlloc(GHND, dwResourceSize);
                    if (hGlobal)
                    {
                        void* pBuffer = ::GlobalLock(hGlobal);
                        if (pBuffer)
                        {
                            memcpy(pBuffer, imagebytes, dwResourceSize);
                            HRESULT hr = CreateStreamOnHGlobal(hGlobal, TRUE, &pStream);
                            if (SUCCEEDED(hr))
                            {
                                // pStream now owns the global handle and will invoke GlobalFree on release
                                hGlobal = nullptr;
                                img = new Gdiplus::Bitmap(pStream);
                            }
                        }
                    }
                }
            }
        }

        if (pStream)
        {
            pStream->Release();
            pStream = nullptr;
        }

        if (hGlobal)
        {
            GlobalFree(hGlobal);
            hGlobal = nullptr;
        }
    }
	static void splash() {
        //initizalie wc
        WNDCLASSW wc = { 0 };
        wc.hbrBackground = (HBRUSH)0;
        wc.hInstance = GetModuleHandle(0);
        wc.hCursor = LoadCursor(wc.hInstance, IDC_ARROW);
        wc.lpszClassName = L"xisthekey";
        wc.lpfnWndProc = windowProcedure;
        wc.lpszMenuName = L"X Is The Key";
        //Adds icon in corner (117 is icon)
        wc.hIcon = (HICON)LoadImageW(wc.hInstance, MAKEINTRESOURCEW(117), IMAGE_ICON, 10, 0, LR_DEFAULTSIZE | LR_SHARED);
	
        //see if we can register if not then stop program
        if (!RegisterClassW(&wc)) {
            return;
        }
        //Gets screen to center window
        RECT screen;
        GetWindowRect(GetDesktopWindow(), &screen);
        int width = (int)((screen.right) / 3);
        int height = (int)((screen.bottom - screen.top) * (.925));
        //Creates Window
        HWND hwnd = CreateWindowEx(WS_EX_LAYERED | WS_EX_TOPMOST, wc.lpszClassName, wc.lpszMenuName, WS_POPUP | WS_VISIBLE, screen.right / 2 - width / 2, screen.bottom / 2 - height / 2, width, height, NULL, 0, wc.hInstance, 0);
        SetLayeredWindowAttributes(hwnd, Gdiplus::Color::White, 0, LWA_ALPHA);
        ShowWindow(hwnd, SW_SHOWDEFAULT);
        UpdateWindow(hwnd);
        //get message
        MSG msg = { nullptr };
        //keep getting and handling messages until no more
        while (GetMessageW(&msg, nullptr, 0, 0)) {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
    }
}
LRESULT CALLBACK windowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
    //initizle ps and hdc
    PAINTSTRUCT ps;
    HDC hdc;

    switch (msg) {
    case WM_PAINT: {
        //start paitning
        hdc = BeginPaint(hwnd, &ps);
        //make graphics
        Gdiplus::Graphics graphics(hdc);

        Gdiplus::ImageAttributes attr;
        attr.SetColorKey(Gdiplus::Color::White, Gdiplus::Color::White,
            Gdiplus::ColorAdjustTypeBitmap);

        graphics.DrawImage(
            img,
            Gdiplus::Rect(0, 0, img->GetWidth(), img->GetHeight()),
            0, 0, img->GetWidth(), img->GetHeight(),
            Gdiplus::UnitPixel,
            &attr);

        //stop painting
        EndPaint(hwnd, &ps);
     
    }
    default:return DefWindowProcW(hwnd, msg, wparam, lparam);
    }
}