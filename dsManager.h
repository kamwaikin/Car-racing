#pragma once
#include "dsutil.h"

class dsManager
{
private:
	LPDIRECTSOUND8 g_pDS;
	
public:
	dsManager(void);
	virtual ~dsManager(void);
	bool initDirectSound(HWND hwnd);
	void shutdownDirectSound(void);
	LPDIRECTSOUNDBUFFER LoadWaveToSoundBuffer(LPTSTR wavFilename);
	void playSound(LPDIRECTSOUNDBUFFER whichBuffer, int loop);
};
