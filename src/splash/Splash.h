#pragma once
#include <Windows.h>
#include <gdiplus.h>
#include <string>

#pragma comment (lib,"gdiplus.lib")

static const int CLOSE_TIMER = 11;
//window amount to keep track of which window because we need unique class name to distinguish
unsigned int windowAmount = 0;
LRESULT CALLBACK windowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
//the X image
Gdiplus::Bitmap* img = nullptr;
namespace Splash {
    //the image always stays the same so we only have to load it once
    static void LoadImageFromResource()
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
                                //set the image
                                img = new Gdiplus::Bitmap(pStream);
                            }
                        }
                    }
                }
            }
        }

        //CLEAN UP!
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
        //add to window amount
        windowAmount++;
        //initizalie wc
        WNDCLASSEX wc = { 0 };
        //set size
        wc.cbSize = sizeof(WNDCLASSEX);
        //background cant be all white because if it is, the gui copies the background making it look bad when you move
        wc.hbrBackground = CreateSolidBrush(RGB(255, 0, 0));
        //i don't need winmain if i have GetModuleHandle!
        wc.hInstance = GetModuleHandle(0);
        //default cursor
        wc.hCursor = LoadCursor(wc.hInstance, IDC_ARROW);
        //we add which window we are to class name so we can differnetiate
        wc.lpszClassName = L"xisthekey" + windowAmount;
        wc.lpfnWndProc = windowProcedure;
        wc.lpszMenuName = L"X Is The Key";
        //no style
        wc.style = 0;
        //Adds icon in corner (117 is icon)
        wc.hIcon = (HICON)LoadImageW(wc.hInstance, MAKEINTRESOURCEW(117), IMAGE_ICON, 10, 0, LR_DEFAULTSIZE | LR_SHARED);
	
        //see if we can register if not then stop program
        if (!RegisterClassEx(&wc)) {
            return;
        }
        //Gets screen to center window
        RECT screen;
        GetWindowRect(GetDesktopWindow(), &screen);
        int width = img->GetWidth();
        int height = img->GetHeight();

        //Creates (Layered) Window
        HWND hwnd = CreateWindowEx(WS_EX_LAYERED | WS_EX_TOPMOST | WS_EX_TRANSPARENT, wc.lpszClassName, wc.lpszMenuName, WS_POPUP, screen.right / 2 - width / 2, screen.bottom / 2 - height / 2, width, height, NULL, 0, wc.hInstance, 0);
        //see if we can create if not then stop program
        if (!hwnd) {
            return;
        }
        //make all red pixels transparent 
        //this function has to be called for a layered window
        SetLayeredWindowAttributes(hwnd, RGB(255, 0, 0), 0, LWA_COLORKEY);

        //make window visible
        ShowWindow(hwnd, SW_NORMAL);
        UpdateWindow(hwnd);

        SetTimer(hwnd, CLOSE_TIMER, 1000, (TIMERPROC)NULL);

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
    case WM_TIMER: {
        switch (wparam) {
            case CLOSE_TIMER: {
                //AnimateWindow(hwnd, 500, AW_HIDE);
                DestroyWindow(hwnd);
                KillTimer(hwnd, CLOSE_TIMER);
                return 0;
            }
        }
    }
    case WM_PAINT: {
        //start paitning
        hdc = BeginPaint(hwnd, &ps);

        //make graphics
        Gdiplus::Graphics graphics(hdc);

        //use image attributes to make it draw a transparent
        Gdiplus::ImageAttributes attr;
        //all the white pixels go transparent
        attr.SetColorKey(Gdiplus::Color::White, Gdiplus::Color::White,
            Gdiplus::ColorAdjustTypeBitmap);

        //draw the iamge
        graphics.DrawImage(img,Gdiplus::Rect(0, 0, img->GetWidth(), img->GetHeight()),0, 0, img->GetWidth(), img->GetHeight(),Gdiplus::UnitPixel,&attr);

        //stop painting
        EndPaint(hwnd, &ps);
     
    }
    default:return DefWindowProcW(hwnd, msg, wparam, lparam);
    }
}