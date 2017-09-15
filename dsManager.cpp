#include "dsManager.h"

dsManager::dsManager(void)
{
	g_pDS=NULL;
}

dsManager::~dsManager(void)
{
}

bool dsManager::initDirectSound(HWND hwnd)
{
	if FAILED(DirectSoundCreate8( NULL, &g_pDS, NULL ))
		return FALSE;
	if FAILED(g_pDS->SetCooperativeLevel( hwnd, DSSCL_PRIORITY ))
		return FALSE;
	return TRUE;
}

void dsManager::shutdownDirectSound(void)
{
	if(g_pDS!=NULL){
		g_pDS->Release();	// release the d3d8 object
		g_pDS=NULL;
	}
}

LPDIRECTSOUNDBUFFER dsManager::LoadWaveToSoundBuffer(LPTSTR wavFilename)
{
	LPDIRECTSOUNDBUFFER apDSBuffer = NULL;
	CWaveFile *wavFile=NULL;
	HRESULT hr=NULL;

	wavFile = new CWaveFile();
	if FAILED(wavFile->Open(wavFilename, NULL, WAVEFILE_READ))
		return FALSE;

	DSBUFFERDESC dsbd;
    ZeroMemory( &dsbd, sizeof(DSBUFFERDESC) );
    dsbd.dwSize          = sizeof(DSBUFFERDESC);
    dsbd.dwBufferBytes   = wavFile->GetSize();
    dsbd.guid3DAlgorithm = GUID_NULL;
	dsbd.lpwfxFormat     = wavFile->m_pwfx;

	dsbd.dwFlags		 = DSBCAPS_CTRLVOLUME;

	if FAILED(g_pDS->CreateSoundBuffer( &dsbd, &apDSBuffer, NULL ))
		return FALSE;

	VOID*   pDSLockedBuffer      = NULL; // Pointer to locked buffer memory
    DWORD   dwDSLockedBufferSize = 0;    // Size of the locked DirectSound buffer
    DWORD   dwWavDataRead        = 0;    // Amount of data read from the wav file 

	hr = apDSBuffer->Lock(		// lock the apDSBuffer. The locked buffer
	0, wavFile->GetSize(),		// is renamed to pDSLockedBuffer
	&pDSLockedBuffer, 
	&dwDSLockedBufferSize,		// The new locked buffer size is dwDSLockedBufferSize
	NULL, NULL, 0L );			// note that dwDSLockedBufferSize can be greater than
								// wavFile->GetSize()
	if FAILED(hr)
		return FALSE;

	wavFile->ResetFile();	// sound file cursor back to the beginning

	hr = wavFile->Read((BYTE*) pDSLockedBuffer,
	dwDSLockedBufferSize, &dwWavDataRead );		// Read wave into pDSLockedBuffer
	if FAILED(hr)
		return NULL;

    if( dwWavDataRead == 0 )
    {
        FillMemory( (BYTE*) pDSLockedBuffer, 
                    dwDSLockedBufferSize, 
                    (BYTE)(wavFile->m_pwfx->wBitsPerSample == 8 ? 128 : 0 ) );
    }
    else if( dwWavDataRead < dwDSLockedBufferSize )
    {					// pDSLockedBuffer[dwWavDataRead],
						// the index dwWavDataRead in the array of pDSLockedBuffer
        FillMemory( (BYTE*) pDSLockedBuffer + dwWavDataRead, 
					// 1025 bytes		- 500 bytes
                   dwDSLockedBufferSize - dwWavDataRead,	// number of bytes to fill
                   (BYTE)(wavFile->m_pwfx->wBitsPerSample == 8 ? 128 : 0 ) );
    }
						// unlock the sound buffer with size pDSLockedBuffer
	apDSBuffer->Unlock( pDSLockedBuffer, dwDSLockedBufferSize, NULL, 0 );
	wavFile=NULL;

	return apDSBuffer;
}

void dsManager::playSound(LPDIRECTSOUNDBUFFER whichBuffer, int loop)
{
	if(loop>=0)
		whichBuffer->Play(0, 0,loop);	// number of looping
	else
		whichBuffer->Play(0, 0,DSBPLAY_LOOPING);	// loop indinity
}