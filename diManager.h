#pragma once
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#define BUTTONDOWN(name, key) (name.rgbButtons[key] & 0x80) 

class diManager
{
public:
	diManager(void);
	~diManager(void);

	// singleton stuff
	static diManager& getInstance() { static diManager pInstance; return pInstance;}

	bool initDirectInput(HINSTANCE hInst, HWND wndHandle);
	void shutdown(void);

	void getInput(void);

	bool isKeyDown();
	bool isButtonDown(int which);	//0=L 1=R
				//-ve = Left  +ve = Right
	inline int getCurMouseX(void) { return mouseState.lX; }
	inline int getCurMouseY(void) { return mouseState.lY; }

private:

	LPDIRECTINPUT8        g_lpDI;		// the direct input object
	LPDIRECTINPUTDEVICE8  g_lpDIDevice; // the direct input device
	LPDIRECTINPUTDEVICE8  g_lpDIKeyboard;
	DIMOUSESTATE mouseState;			// The current state of the mouse device 
};
