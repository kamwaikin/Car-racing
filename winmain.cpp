#include <windows.h>
#include "diManager.h"
#include "dxManager.h"
#include "dsManager.h"
#define totalNumObj 10

HINSTANCE hInst;				// holds the instance for this app
HWND wndHandle;					// global window handle

LPD3DXMESH mesh1;
LPD3DXMESH mesh2;
LPD3DXMESH mesh3;
LPD3DXMESH mesh4;
LPD3DXMESH mesh5;
LPD3DXMESH mesh6;
LPD3DXMESH mesh7;
LPD3DXMESH mesh8;
LPD3DXMESH mesh9;
LPD3DXMESH mesh10;
LPD3DXMESH mesh11;

float car_angle=0;
float car_frontvelocity = 0;	//current vf
float car_tempfrontvelocity = 0;	//vi
float car_maxfrontvelocity = 100;	//max vf
float car_frontacceleration = 15;	//a
float car_lastfrontacceleration = 0;
D3DXVECTOR3 car_position=D3DXVECTOR3(150.0f, 5.0f, 0.0f);
D3DXVECTOR3 car1_position=D3DXVECTOR3(200.0f, 5.0f, 0.0f);
D3DXVECTOR3 camera_position=D3DXVECTOR3(0, 70,-100);
D3DXVECTOR3 point_position=D3DXVECTOR3(10, 0, 0);
DWORD lastUpdate=timeGetTime();
LPDIRECTSOUNDBUFFER soundBuffer;

bool x;
bool isShooted;
bool isStart;
bool isWin;
typedef struct objectStruct
{
	LPD3DXMESH mesh;
	D3DXVECTOR3 pos;
	D3DXVECTOR3 scale;
	float size;
	bool collidable;
	bool collided;
	int textureSet;
} ;

typedef struct ammoStruct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 dir;
};
/////////////////////////////////////////////w/ forward declarations
bool    initWindow(HINSTANCE hInstance);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	// call our function to init and create our window
	if (!initWindow(hInstance))
	{
		MessageBox(NULL, "Unable to create window", "ERROR", MB_OK);
		return false;
	}

	dxManager *dxMgr = new dxManager();
	if(!dxMgr->init(wndHandle))
	{
		MessageBox(NULL, "Unable to init DirectX","ERROR", MB_OK);
		return false;
	}
	diManager *diMgr = new diManager();
	if(!diMgr->initDirectInput(hInstance, wndHandle))
	{
		MessageBox(NULL, "Unable to init DirectInput","ERROR", MB_OK);
		return false;
	}
	dsManager *dsMgr = new dsManager();
	if(!dsMgr->initDirectSound(wndHandle))
	{
		MessageBox(NULL, "Unable to init DirectSound","ERROR", MB_OK);
		return false;
	}
	dxMgr->init(wndHandle);
	diMgr->initDirectInput(hInstance, wndHandle);
	dsMgr->initDirectSound(wndHandle);
	soundBuffer = dsMgr->LoadWaveToSoundBuffer("forest2.wav");
	soundBuffer->SetVolume(-1300);
	dsMgr->playSound(soundBuffer, -1);
	std::vector<ammoStruct> ammos;
		
	objectStruct obj[totalNumObj];
		//filename // 1=set of material
	//mesh1 = dxMgr->loadMesh("floor.x", 1);
	obj[0].mesh= dxMgr->loadMesh("tree.x", 1);
	obj[0].textureSet=1;
	obj[0].collidable=true;
	obj[0].collided=false;
	obj[0].scale=D3DXVECTOR3(1.0f, rand()%2+1, 1.0f);
	obj[0].pos=D3DXVECTOR3(rand()%360-180, 0.0f, rand()%360-180);
	obj[0].size=9*2*9*2;
	obj[1].mesh= dxMgr->loadMesh("tree.x", 1);
	obj[1].textureSet=1;
	obj[1].collidable=true;
	obj[1].collided=false;
	obj[1].scale=D3DXVECTOR3(1.0f, rand()%2+1, 1.0f);
	obj[1].pos=D3DXVECTOR3(rand()%360-180, 0.0f, rand()%360-180);
	obj[1].size=9*2*9*2;
	obj[2].mesh= dxMgr->loadMesh("tree.x", 1);
	obj[2].textureSet=1;
	obj[2].collidable=true;
	obj[2].collided=false;
	obj[2].scale=D3DXVECTOR3(1.0f, rand()%2+1, 1.0f);
	obj[2].pos=D3DXVECTOR3(rand()%360-180, 0.0f, rand()%360-180);
	obj[2].size=9*2*9*2;
	obj[3].mesh= dxMgr->loadMesh("tree.x", 1);
	obj[3].textureSet=1;
	obj[3].collidable=true;
	obj[3].collided=false;
	obj[3].scale=D3DXVECTOR3(1.0f, rand()%2+1, 1.0f);
	obj[3].pos=D3DXVECTOR3(rand()%360-180, 0.0f, rand()%360-180);
	obj[3].size=9*2*9*2;
	obj[4].mesh= dxMgr->loadMesh("tree.x", 1);
	obj[4].textureSet=1;
	obj[4].collidable=true;
	obj[4].collided=false;
	obj[4].scale=D3DXVECTOR3(1.0f, rand()%2+1, 1.0f);
	obj[4].pos=D3DXVECTOR3(rand()%360-180, 0.0f, rand()%360-180);
	obj[4].size=9*2*9*2;

	obj[5].mesh= dxMgr->loadMesh("rock.x", 2);
	obj[5].textureSet=2;
	obj[5].collidable=true;
	obj[5].collided=false;
	obj[5].scale=D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	obj[5].pos=D3DXVECTOR3(rand()%360-180, 0.0f, rand()%360-180);
	obj[5].size=10*2*10*2;
	obj[6].mesh= dxMgr->loadMesh("rock.x", 2);
	obj[6].textureSet=2;
	obj[6].collidable=true;
	obj[6].collided=false;
	obj[6].scale=D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	obj[6].pos=D3DXVECTOR3(rand()%360-180, 0.0f, rand()%360-180);
	obj[6].size=10*2*10*2;
	obj[7].mesh= dxMgr->loadMesh("rock.x", 2);
	obj[7].textureSet=2;
	obj[7].collidable=true;
	obj[7].collided=false;
	obj[7].scale=D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	obj[7].pos=D3DXVECTOR3(rand()%360-180, 0.0f, rand()%360-180);
	obj[7].size=10*2*10*2;
	obj[8].mesh= dxMgr->loadMesh("rock.x", 2);
	obj[8].textureSet=2;
	obj[8].collidable=true;
	obj[8].collided=false;
	obj[8].scale=D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	obj[8].pos=D3DXVECTOR3(rand()%360-180, 0.0f, rand()%360-180);
	obj[8].size=10*2*10*2;
	obj[9].mesh= dxMgr->loadMesh("rock.x", 2);
	obj[9].textureSet=2;
	obj[9].collidable=true;
	obj[9].collided=false;
	obj[9].scale=D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	obj[9].pos=D3DXVECTOR3(rand()%360-180, 0.0f, rand()%360-180);
	obj[9].size=10*2*10*2;
	//mesh2 = dxMgr->loadMesh("road.x", 2);
	mesh3 = dxMgr->loadMesh("road.x", 3);
	mesh4 = dxMgr->loadMesh("pond.x", 4);
	mesh5 = dxMgr->loadMesh("floor.x", 5);
	mesh6 = dxMgr->loadMesh("car.x", 6);
	mesh7 = dxMgr->loadMesh("bullet.x", 7);
	mesh8 = dxMgr->loadMesh("flag.x", 8);
	mesh9 = dxMgr->loadMesh("movie.x", 9);
	mesh10 = dxMgr->loadMesh("movie1.x", 10);
	mesh11 = dxMgr->loadMesh("car1.x", 11);
	//									2
	if (!obj[0].mesh)
	{
		MessageBox(wndHandle, "can't load xfile", "ERROR", MB_OK);
		return false;
	}
	// Main message loop:
	// Enter the message loop
    MSG msg; 
    ZeroMemory( &msg, sizeof(msg) );
    while( msg.message!=WM_QUIT )
    {
		// check for messages
		if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
        {
			TranslateMessage( &msg );
            DispatchMessage( &msg );
        }
		// this is called when no messages are pending
		else
		{
			DWORD timeNow=timeGetTime();
			float time = (timeNow-lastUpdate) / 1000.0;
			car_frontvelocity = car_tempfrontvelocity + car_frontacceleration*time;
			if(car_frontvelocity>car_maxfrontvelocity) 	car_frontvelocity=car_maxfrontvelocity;
			float step = car_tempfrontvelocity*time + (car_frontacceleration*time*time)/2;

			diMgr->getInput();
			if(!x){
				if(diMgr->getCurMouseX() < 0){  //gragged left
					car_angle-=step;
					point_position.z -= step;
				}
				if(diMgr->getCurMouseX() > 0){  //gragged right
					car_angle+=step;
					point_position.z += step;
				}
			}
			float Xstep = step*sin(car_angle*D3DX_PI/180.0);
			float Zstep = step*cos(car_angle*D3DX_PI/180.0);

			if(diMgr->isButtonDown(0)){   //left click
				car_position.z += Zstep;
				car_position.x += Xstep;
				if(!x){
					car1_position.z += Zstep;
					car1_position.x += Xstep;}
				isStart=true;
			}
			if(car_position.x < -150) {car_position.x=-150;}
			if(car_position.x > 150) {car_position.x=150;}
			if(car_position.z < -150) {car_position.z=-150;}
			if(car_position.z > 150) {car_position.z=150;}

			if(car1_position.x < -150) {car1_position.x=-150;}
			if(car1_position.x > 150) {car1_position.x=150;}
			if(car1_position.z < -150) {car1_position.z=-150;}
			if(car1_position.z > 150) {car1_position.z=150;}

			if(!(car_position.x + 19/2 < 0 - 80) &&
					!(car_position.x - 19/2 > 0 + 80) &&
					!(car_position.z + 48/2 < 0 - 70) &&
					!(car_position.z - 48/2 > 0 + 70))
				{
					car_maxfrontvelocity=15;
				}
			else car_maxfrontvelocity=100;

			if(!(car1_position.x + 19/2 < 150 - 10) &&
					!(car1_position.x - 19/2> 150 + 10) &&
					!(car1_position.z + 48/2 < -35 - 10) &&
					!(car1_position.z - 48/2 > -35 + 10))
				{
					isWin=true;
				}
			if(!(car_position.x + 19/2 < 120 - 10) &&
					!(car_position.x - 19/2> 120 + 10) &&
					!(car_position.z + 48/2 < -100 - 10) &&
					!(car_position.z - 48/2 > -100 + 10))
				{
					x=true;
					car_frontacceleration=1000;
					car_maxfrontvelocity=10000;
				}
			
			/*
			if(camera_position.x < -150) {camera_position.x=-150;}
			if(camera_position.x > 150) {camera_position.x=150;}
			if(camera_position.z < -150) {camera_position.z=-150;}
			if(camera_position.z > 150) {camera_position.z=150;}*/
			/*for(int i=0;i<totalNumObj;i++)
			{
				if(!(car_position.x + 19/2 < obj[i].pos.x * obj[i].scale.x - sqrt(obj[i].size)/2) &&
					!(car_position.x - 19/2 > obj[i].pos.x * obj[i].scale.x + sqrt(obj[i].size)/2) &&
					!(car_position.z + 41/2 < obj[i].pos.z * obj[i].scale.z - sqrt(obj[i].size)/2) &&
					!(car_position.z - 41/2 > obj[i].pos.z * obj[i].scale.z + sqrt(obj[i].size)/2))
				{
					obj[i].collided=true;
				}
			}*/
			for(int i=0;i<totalNumObj;i++)
			{
				if(!(car_position.x + 19/2 < obj[i].pos.x * obj[i].scale.x - sqrt(obj[i].size)/2) &&
					!(car_position.x - 19/2 > obj[i].pos.x * obj[i].scale.x + sqrt(obj[i].size)/2) &&
					!(car_position.z + 48/2 < obj[i].pos.z * obj[i].scale.z - sqrt(obj[i].size)/2) &&
					!(car_position.z - 48/2 > obj[i].pos.z * obj[i].scale.z + sqrt(obj[i].size)/2))
				{
					obj[i].collided=true;
				}
			}
			if(!diMgr->isKeyDown()) isShooted=false;
			if(diMgr->isKeyDown() && !isShooted)
			{
				isShooted=true;
				ammoStruct newAmmo;
				newAmmo.pos = car_position;
				newAmmo.pos.x = car_position.x+Xstep*10;
				newAmmo.pos.z = car_position.z+Zstep*10;
				newAmmo.dir.y = 0;
				newAmmo.dir.x = step*sin(car_angle*D3DX_PI/180.0)*3;
				newAmmo.dir.z = step*cos(car_angle*D3DX_PI/180.0)*3;
				ammos.push_back (newAmmo);
			}	
			for(std::vector<ammoStruct>::iterator i = ammos.begin(); i != ammos.end();)
			{
				ammoStruct *ammo = &(*i);
				if(ammo->pos.x<-150 || ammo->pos.x>150 || ammo->pos.z<-150 || ammo->pos.z>150)
				{
					ammos.erase(i++);
					i = ammos.begin();
				}
				else 
					++i;
			}

			for(std::vector<ammoStruct>::iterator i = ammos.begin(); i != ammos.end(); i++)
			{
				ammoStruct *ammo = &(*i);
				ammo->pos.x += ammo->dir.x;
				ammo->pos.z += ammo->dir.z;
				
				for(int i=0;i<totalNumObj;i++)
					{
					if(!(ammo->pos.x + 1 < obj[i].pos.x * obj[i].scale.x - sqrt(obj[i].size)/2) &&
						!(ammo->pos.x - 1 > obj[i].pos.x * obj[i].scale.x + sqrt(obj[i].size)/2) &&
						!(ammo->pos.z + 1 < obj[i].pos.z * obj[i].scale.z - sqrt(obj[i].size)/2) &&
						!(ammo->pos.z - 1 > obj[i].pos.z * obj[i].scale.z + sqrt(obj[i].size)/2))
					{
						obj[i].collided=true;
					}
				}
			}
			//car_position.x += step*car_speed;
			// call our render function
			dxMgr->beginRender();
			// Set the camera
			dxMgr->setCamera(camera_position + car_position, car_position);
			if(x)
			{
				dxMgr->setCamera(D3DXVECTOR3(300.0f, 70.0f, 0.0f),  D3DXVECTOR3(180.0f, 0.0f, 0.0f));
				car_position.x=150;
				car_position.z+=3;
				car_angle=0;
				car1_position.x=150;
				car1_position.z+=1;
			}
			//dxMgr->pointCamera(point_position);
			for(int i=0;i<totalNumObj;i++)
			{
				if(obj[i].collidable && !obj[i].collided)
					dxMgr->drawMesh(obj[i].mesh, obj[i].pos, 0, obj[i].scale, obj[i].textureSet);
			}
			// Draw ground	mesh	position						scale					set
			//dxMgr->drawMesh(obj[0].mesh, obj[0].pos, 0, obj[0].scale, obj[0].textureSet);
			//dxMgr->drawMesh(mesh2, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, D3DXVECTOR3(1.0f, 1.0f, 1.0f), 2);
			dxMgr->drawMesh(mesh3, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, D3DXVECTOR3(1.0f, 1.0f, 1.0f), 3);
			dxMgr->drawMesh(mesh4, D3DXVECTOR3(20.0f, 0.0f, 0.0f), 0, D3DXVECTOR3(1.0f, 1.0f, 1.0f),4 );
			dxMgr->drawMesh(mesh5, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0,D3DXVECTOR3(1.0f, 1.0f, 1.0f), 5);
			dxMgr->drawMesh(mesh6, car_position, car_angle, D3DXVECTOR3(1.0f, 1.0f, 1.0f), 6);
			dxMgr->drawMesh(mesh8, D3DXVECTOR3(180.0f, 0.0f, 0.0f), 0, D3DXVECTOR3(4.0f, 4.0f, 4.0f), 8);
			dxMgr->drawMesh(mesh8, D3DXVECTOR3(130.0f, 0.0f, 0.0f), 40, D3DXVECTOR3(4.0f, 4.0f, 4.0f), 8);
			dxMgr->drawMesh(mesh11, car1_position + D3DXVECTOR3(30.0f, 0.0f, 0.0f), car_angle, D3DXVECTOR3(1.0f, 1.0f, 1.0f), 11);
			if(!isStart)
				dxMgr->drawMesh(mesh9, car_position - D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, D3DXVECTOR3(1.0f, 1.0f, 1.0f), 9);
			if(isWin)
			{
				dxMgr->drawMesh(mesh10, car_position - D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, D3DXVECTOR3(1.0f, 1.0f, 1.0f), 10);
				//car_position=D3DXVECTOR3(150.0f, 20.0f, 0.0f);
				//camera_position=D3DXVECTOR3(0, 70,-100);
				x=false;
			}
			for(std::vector<ammoStruct>::iterator i = ammos.begin(); i != ammos.end(); i++)
			{
				ammoStruct ammo = *i;
				dxMgr->drawMesh(mesh7, ammo.pos, 0, D3DXVECTOR3(1.0f, 1.0f, 1.0f), 7);
			}
			lastUpdate=timeGetTime();
			car_tempfrontvelocity=car_frontvelocity;
			dxMgr->endRender();
		}
    }

	// shutdown the directx manager
	dxMgr->shutdown();
	diMgr->shutdown();
	soundBuffer->Release();
	dsMgr->shutdownDirectSound();
	return (int) msg.wParam;
}

/*************************************************************************
* initWindow
* initalize the window

*************************************************************************/
bool initWindow(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= 0;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= "DirectXExample";
	wcex.hIconSm		= 0;
	RegisterClassEx(&wcex);

	wndHandle = CreateWindow("DirectXExample", 
							 "DirectXExample", 
							 WS_OVERLAPPEDWINDOW,
							 CW_USEDEFAULT, 
							 CW_USEDEFAULT, 
							 640, 
							 480, 
							 NULL, 
							 NULL, 
							 hInstance, 
							 NULL);
   if (!wndHandle)
      return false;
   
   ShowWindow(wndHandle, SW_SHOW);
   UpdateWindow(wndHandle);

   return true;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) 
	{
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}