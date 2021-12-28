#pragma once
#include <Windows.h>
#pragma comment (lib,"Winmm.lib")
namespace SoundPlayer {
    HANDLE hRes;
    char* lpRes;
    static void init() {
        //Find the WAVE resource.
        //124 is the resource of the thud sound
        HRSRC hResInfo = FindResource(NULL, MAKEINTRESOURCE(124), TEXT("IDR_WAVE1"));
        //If it doesn't exist then stop
        if (hResInfo == NULL) {
            return;
        }
        // Load the WAVE resource. 
        hRes = LoadResource(NULL, hResInfo);
        //If it doesn't exist then stop
        if (hRes == NULL) {
            return;
        }
        // Lock the WAVE resource and play it. 
        lpRes = (char*)LockResource(hRes);
    }
    static void deInit() {
        if (lpRes != NULL) {
            UnlockResource(hRes);
        }
        // Free the WAVE resource
        FreeResource(hRes);
    }
	static void playThud() {
        if (lpRes != NULL) {
            PlaySound((LPCUWSTR)lpRes, NULL, SND_MEMORY | SND_ASYNC);
        }
	}
}