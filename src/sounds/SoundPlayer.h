#pragma once
#include <Windows.h>
#pragma comment (lib,"Winmm.lib")
namespace SoundPlayer {
	static void playThud() {
        //Find the WAVE resource.
		//124 is the resource of the thud sound
        HRSRC hResInfo = FindResource(NULL, MAKEINTRESOURCE(124), TEXT("IDR_WAVE1"));
        //If it doesn't exist then stop
        if (hResInfo == NULL) {
            return;
        }
        // Load the WAVE resource. 
        HANDLE hRes = LoadResource(NULL, hResInfo);
        //If it doesn't exist then stop
        if (hRes == NULL) {
            return;
        }
        // Lock the WAVE resource and play it. 
        const char* lpRes = (const char*)LockResource(hRes);
        if (lpRes != NULL) {
            PlaySound((LPCUWSTR)lpRes, NULL, SND_MEMORY | SND_ASYNC);
            UnlockResource(hRes);
        }
        // Free the WAVE resource
        FreeResource(hRes);
        
	}
}