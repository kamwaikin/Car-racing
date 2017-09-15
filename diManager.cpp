#include ".\dimanager.h"

extern HINSTANCE hInst;				// holds the instance for this app
char	buffer[256];
#define KEYDOWN(name, key) (name[key] & 0x80) //macro

diManager::diManager(void)
{
	g_lpDI = NULL;
	g_lpDIDevice = NULL;
	g_lpDIKeyboard = NULL;
	
}

diManager::~diManager(void)
{
	shutdown();
}

/*******************************************************************
* initDirectInput
* Initializes DirectInput for use with a mouse device
*******************************************************************/
bool diManager::initDirectInput(HINSTANCE hInst, HWND wndHandle)
{
	HRESULT hr;

	// Create the DirectInput object. 
    hr = DirectInput8Create(hInst, DIRECTINPUT_VERSION, 
                            IID_IDirectInput8, (void**)&g_lpDI, NULL); 

	if FAILED(hr) 
		return FALSE; 

    // Retrieve a pointer to an IDirectInputDevice8 interface 
    hr = g_lpDI->CreateDevice(GUID_SysMouse, &g_lpDIDevice, NULL); 
	if FAILED(hr)
		return FALSE; 
	hr = g_lpDI->CreateDevice(GUID_SysKeyboard, &g_lpDIKeyboard, NULL); 
	if FAILED(hr)
		return FALSE; 

	hr = g_lpDIDevice->SetDataFormat(&c_dfDIMouse); 
	if FAILED(hr)
		return FALSE; 
	hr = g_lpDIKeyboard->SetDataFormat(&c_dfDIKeyboard); 
	if FAILED(hr)
		return FALSE; 
	// Set the cooperative level 
    hr = g_lpDIDevice->SetCooperativeLevel(wndHandle, DISCL_FOREGROUND | DISCL_EXCLUSIVE); 
    if FAILED(hr) 
        return FALSE; 
	// Get access to the input device. 
    hr = g_lpDIDevice->Acquire(); 
    if FAILED(hr) 
        return FALSE; 
	hr = g_lpDIKeyboard->Acquire(); 
    if FAILED(hr) 
        return FALSE; 
	return true;
}

/*******************************************************************
* shutdown
* Shuts down and releases DirectInput
*******************************************************************/
void diManager::shutdown(void)
{
	if (g_lpDI) 
    { 
        if (g_lpDIDevice) 
        { 
			// Always unacquire device before calling Release(). 
            g_lpDIDevice->Unacquire(); 
            g_lpDIDevice->Release();
            g_lpDIDevice = NULL; 
		} 
		if(g_lpDIKeyboard)
		{
			g_lpDIKeyboard->Unacquire();
			g_lpDIKeyboard->Release();
			g_lpDIKeyboard = NULL;
		}
        g_lpDI->Release();
        g_lpDI = NULL; 
    } 
}

/*******************************************************************
* getInput
* Retrieves the current user input
*******************************************************************/
void diManager::getInput(void)
{
	HRESULT hr;

	hr = g_lpDIDevice->GetDeviceState(sizeof(DIMOUSESTATE),(LPVOID)&mouseState); 

	// check the return state to see if the device is still accessible
	if (FAILED ( hr ))
	{
		// try and reacquire the input device
		hr = g_lpDIDevice->Acquire();
		// do a continuous loop until the device is reacquired
	       while( hr == DIERR_INPUTLOST ) 
			hr = g_lpDIDevice->Acquire();
	}	

	hr = g_lpDIKeyboard->GetDeviceState(sizeof(buffer),(LPVOID)&buffer); 

	// check the return state to see if the device is still accessible
	if (FAILED ( hr ))
	{
		// try and reacquire the input device
		hr = g_lpDIKeyboard->Acquire();
		// do a continuous loop until the device is reacquired
	       while( hr == DIERR_INPUTLOST ) 
			hr = g_lpDIKeyboard->Acquire();
	}
}

/*******************************************************************
* isButtonDown
*******************************************************************/
bool diManager::isButtonDown(int which)
{
	return (bool)BUTTONDOWN(mouseState, which);
}

bool diManager::isKeyDown()
{
	return (bool)KEYDOWN(buffer,DIK_SPACE);
}
