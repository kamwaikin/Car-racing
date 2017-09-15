#pragma once

#include <d3d9.h>
#include <d3dx9tex.h>
#include <string>
#include <vector>

class dxManager
{
public:
	LPDIRECT3DDEVICE9       pd3dDevice;
	dxManager(void);
	~dxManager(void);
	bool init(HWND hwnd);
	void shutdown(void);
	void beginRender(void);
	void drawMesh(LPD3DXMESH pMesh, D3DXVECTOR3 position, float angle, D3DXVECTOR3 scale, int mesh);
	void endRender(void);
	IDirect3DSurface9* getSurfaceFromBitmap(std::string filename, int width, int height);

	IDirect3DSurface9* getBackBuffer(void);
	void blitToSurface(IDirect3DSurface9* srcSurface, const RECT *srcRect, const RECT *destRect);

	LPD3DXMESH loadMesh(std::string filename, int mesh);

	void setCamera(D3DXVECTOR3, D3DXVECTOR3);
	void createCamera(float nearClip, float farClip);
	void moveCamera(D3DXVECTOR3 vec);
	void pointCamera(D3DXVECTOR3 vec);

private:
	HRESULT                 lastResult;
	LPDIRECT3D9             pD3D;	
	std::vector<D3DMATERIAL9*> m_pMeshMaterials;
	std::vector<LPDIRECT3DTEXTURE9*>		g_pTexture;
	std::vector<DWORD>               m_dwNumMaterials;     // Number of materials	

	// screen details
	int screen_width;
	int screen_height;

	// camera details
	D3DXMATRIX matView;
	D3DXMATRIX matProj;
	D3DXVECTOR3 position;
	D3DXVECTOR3 lookPos;
};
