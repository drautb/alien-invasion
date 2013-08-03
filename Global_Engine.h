/////////////////////////////////////////////////
// Global_Engine.h 
// Global Header File For Game Engine
// Developed By: Ben Draut
// Last Updated: 3/1/06
/////////////////////////////////////////////////

#ifndef GLOBAL_ENGINE_H
#define GLOBAL_ENGINE_H

/////////////////////////////////////////////////
// Misc. Stuff
/////////////////////////////////////////////////
#define WIN32_LEAN_AND_MEAN
#define ReleaseCOM(x) if(x){x->Release(); x = NULL;}
// DirectInput Version 
#define DIRECTINPUT_VERSION 0x0800

// common color
#define COLOR_GREEN D3DCOLOR_RGBA(0,255,0,255)

/////////////////////////////////////////////////
// Includes and Constants
/////////////////////////////////////////////////

// Windows Includes 
#include <windows.h>
#include <windowsx.h>

// Standard Inlcudes
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <time.h>
#include <assert.h>

// DirectX Includes 
#include <d3d9.h>
#include <d3dx9.h>
#include <dsutil.h>

// Game Engine Includes 
#include "Main_Engine.h"
#include "Graphics_Engine.h"
#include "Input_Engine.h"
#include "Audio_Engine.h"

// Other includes
#include "Game_Objects.h"

// Constants  
#define PI 3.141592654f
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define WORLD_WIDTH 8000
#define WORLD_HEIGHT 6000
#define STAR_TILE_SIZE 200
#define DRONE_WIDTH 69.0f
#define DRONE_HEIGHT 69.0f
#define CLAW_WIDTH 58.0f
#define CLAW_HEIGHT 54.0f
#define VIPER_WIDTH 70.0f
#define VIPER_HEIGHT 94.0f
#define SMALL_ASTEROID_WIDTH 41.0f
#define SMALL_ASTEROID_HEIGHT 48.0f
#define BIG_ASTEROID_WIDTH 205.0f
#define BIG_ASTEROID_HEIGHT 250.0f

/////////////////////////////////////////////////
// Derived Main Game Class
/////////////////////////////////////////////////
class cGame : public cBaseGame
{
private:
	// Engine Graphics Objects 
	cGraphics m_Graphics;

	// Tile Engine object
	cTile m_TE;

	// Font objects
	cFont m_Font;
	cFont m_SmallFont;

	// Audio object and sounds
	cAudio m_Audio;

	// Sounds
	cWAVSound m_ESG13;
	cWAVSound m_TFC;
	cWAVSound m_NWC;
	cWAVSound m_Explode;
	cWAVSound m_SmExp;
	cWAVSound m_Laser;

	// State Machine Manager object
	cStateMachine m_SM;

	// Game Message manager
	cMessageManager m_MM; 

	// Game states
	static void GameMenu(void *DataPtr, long Purpose);
	static void MainGame(void *DataPtr, long Purpose);
	static void GameOver(void *DataPtr, long Purpose);
	static void GamePause(void *DataPtr, long Purpose);

	// Input engine object
	cKeyboard m_KB;

	// Textures
	cTexture m_MenuTex;
	cTexture m_SpaceTex;
	cTexture m_RadarTex;
	cTexture m_GreenBlipTex;
	cTexture m_RedBlipTex;
	cTexture m_GrayBlipTex;
	cTexture m_PlayerTex;
	cTexture m_WPLT;
	cTexture m_NPLT;
	cTexture m_SPLT;
	cTexture m_HealthMeterTex;
	cTexture m_HealthBarTex;
	cTexture m_PlayerLifeTex;
	cTexture m_DroneTex;
	cTexture m_DroneLaserTex;
	cTexture m_ClawTex;
	cTexture m_ViperTex;
	cTexture m_SmallAsteroidTex;
	cTexture m_BigAsteroidTex;
	cTexture m_GameOverTex;
	cTexture m_LaserUpgradeTex;
	cTexture m_HealthUpgradeTex;
	cTexture m_RocketTex;
	cTexture m_BombTex;

public:
	// Constructor/Destructor 
	cGame();
	~cGame(){}

	// Overloaded Main Game Functions 
	bool GameInit();
	bool GameRun();
	bool GameEnd();

	// Other Game Functions
	void HandleInput();
	void DeleteDeadObjects();
	void StartNewWave();
	void ResetPlayer();
	void CreatePowerup(float X, float Y);

	// Initializers
	void InitGame();
	void InitAsteroids();

	// Shutdowners
	void ShutdownGame();

	// Update-ers
	void UpdateMenu();
	void UpdateGame();
	void UpdateScroll();
	void UpdatePlayer();

	// Draw-ers
	void DrawMenu();
	void DrawGame();
	void DrawStars();
	void DrawRadar();
	void DrawStats();
	
	// General Game objects
	cEarth *m_Earth;
	cPlayerShip *m_Player;
	cEnemyManager *m_EnemyManager;
	cExplosionManager *m_EM;
	cAsteroidManager *m_AM;
	cPowerupManager *m_PM;

	// Game Variables
	int m_Wave;						// Current wave of alien ships
	int m_MenuChoice;				// Current Menu choice
	int m_ScrollX, m_ScrollY;		// Scrolling variables
	int m_DelayCounter;
	int m_FinalScore, m_FinalWave;
};

#endif 


