#include "initializers.h"
#include "../hook/KeyboardHook.h"
#include <Windows.h>
#include <gdiplus.h>

#pragma comment (lib,"gdiplus.lib")

static ULONG_PTR gdiplusToken;

void initializers::init()
{
	//Initilzing Gdi+
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	//start off by initalizing the keyboard hook for incoming key presses
	KeyboardHook::init();
}

void initializers::deinit()
{
	//Shutdown Gdi+
	Gdiplus::GdiplusShutdown(gdiplusToken);
}
