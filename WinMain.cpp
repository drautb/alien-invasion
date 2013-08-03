/////////////////////////////////////////////////
// ALIEN INVASION!
// Space dogfighting game created by Ben Draut.
// File: WinMain.cpp
// Created: 8/15/06
// Last Updated: 8/15/06
/////////////////////////////////////////////////

// Include everything
#include "Global_Engine.h"

// Game Constructor
cGame::cGame()
{
	m_Width = SCREEN_WIDTH;
	m_Height = SCREEN_HEIGHT;
	strcpy(m_AppTitle, "Alien Invasion!");
}

// Main game functions, Init, Run, and Shutdown
bool cGame::GameInit()
{
	// Initialize graphics
	if (!m_Graphics.GraphicsInit(GetHWND(), true, m_Width, m_Height))
		Error(true, "Error initializing DirectX Graphics!\nExiting...");

	ShowMouse(false);

	// Initialize tile engine
	if (!m_TE.Create(&m_Graphics, 3))
		Error(true, "Error initializing tile engine!\nExiting...");

	// Init font
	if (!m_Font.Create(&m_Graphics, "Quartz", 24))
		Error(true, "Error initializing font.\n\tExiting...");
	if (!m_SmallFont.Create(&m_Graphics, "Quartz", 20))
		Error(true, "Error initializing small font.\n\tExiting...");

	// Load tile engine textures
	if (!m_TE.Load(0, "Graphics/Earth.bmp", 96, 96, 6, D3DCOLOR_RGBA(255,0,255,255)))
		Error(true, "Error loading Earth texture,\n\tExiting...");
	if (!m_TE.Load(1, "Graphics/STARS.bmp", 200, 200, 1, D3DCOLOR_RGBA(0,0,0,255)))
		Error(true, "Error loading stars texture,\n\tExiting...");
	if (!m_TE.Load(2, "Graphics/Explosion2.bmp", 32, 32, 19))
		Error(true, "Error loading explosion texture,\n\tExiting...");

	// Initialize audio stuff
	if (!m_Audio.InitDirectSound(GetHWND()))
		Error(true, "Error initializing DirectSound.\n\tExiting...");

	// Load sounds
	if (!m_ESG13.LoadWAVFile(&m_Audio, "Audio/EnteringSectorGamma13.wav"))
		Error(true, "Error loading Sector 13 WAV file.\n\tExiting...");
	if (!m_TFC.LoadWAVFile(&m_Audio, "Audio/TheFinalCountdown.wav"))
		Error(true, "Error loading The Final Countdown WAV file.\n\tExiting...");
	if (!m_NWC.LoadWAVFile(&m_Audio, "Audio/NextWaveCommencing.wav"))
		Error(true, "Error loading Next wave commencing WAV file.\n\tExiting...");
	if (!m_Explode.LoadWAVFile(&m_Audio, "Audio/Explode.wav"))
		Error(true, "Error loading Explode WAV file.\n\tExiting...");
	if (!m_SmExp.LoadWAVFile(&m_Audio, "Audio/SmExplode.wav"))
		Error(true, "Error loading Small Explode WAV file.\n\tExiting...");
	if (!m_Laser.LoadWAVFile(&m_Audio, "Audio/Laser.wav"))
		Error(true, "Error loading Laser WAV file.\n\tExiting...");

	// Load textures 
	if (!m_MenuTex.Load(&m_Graphics, "Graphics/Menu.bmp", D3DCOLOR_RGBA(255,0,255,255)))
		Error(true, "Error loading menu texture.\n\tExiting...");
	if (!m_SpaceTex.Load(&m_Graphics, "Graphics/Space.bmp", D3DCOLOR_RGBA(0,0,0,0)))
		Error(true, "Error loading space texture.\n\tExiting...");
	if (!m_RadarTex.Load(&m_Graphics, "Graphics/Radar.bmp", D3DCOLOR_RGBA(0,0,0,255)))
		Error(true, "Error loading radar texture.\n\tExiting...");
	if (!m_GreenBlipTex.Load(&m_Graphics, "Graphics/GreenBlip.bmp", D3DCOLOR_RGBA(0,0,0,0)))
		Error(true, "Error loading green radar blip texture.\n\tExiting...");
	if (!m_RedBlipTex.Load(&m_Graphics, "Graphics/RedBlip.bmp", D3DCOLOR_RGBA(0,0,0,0)))
		Error(true, "Error loading red radar blip texture.\n\tExiting...");
	if (!m_GrayBlipTex.Load(&m_Graphics, "Graphics/GrayBlip.bmp", D3DCOLOR_RGBA(0,0,0,0)))
		Error(true, "Error loading Gray radar blip texture.\n\tExiting...");
	if (!m_PlayerTex.Load(&m_Graphics, "Graphics/Player.bmp", D3DCOLOR_RGBA(0,0,0,255)))
		Error(true, "Error loading player texture.\n\tExiting...");
	if (!m_WPLT.Load(&m_Graphics, "Graphics/WPL.bmp", D3DCOLOR_RGBA(0,0,0,255)))
		Error(true, "Error loading player's laser texture.\n\tExiting...");
	if (!m_NPLT.Load(&m_Graphics, "Graphics/NPL.bmp", D3DCOLOR_RGBA(0,0,0,255)))
		Error(true, "Error loading player's laser texture.\n\tExiting...");
	if (!m_SPLT.Load(&m_Graphics, "Graphics/SPL.bmp", D3DCOLOR_RGBA(0,0,0,255)))
		Error(true, "Error loading player's laser texture.\n\tExiting...");
	if (!m_HealthMeterTex.Load(&m_Graphics, "Graphics/HealthMeter.bmp", D3DCOLOR_RGBA(0,0,0,255)))
		Error(true, "Error loading health meter texture.\n\tExiting...");
	if (!m_HealthBarTex.Load(&m_Graphics, "Graphics/HealthBar.bmp", D3DCOLOR_RGBA(0,0,0,255)))
		Error(true, "Error loading health bar texture.\n\tExiting...");
	if (!m_PlayerLifeTex.Load(&m_Graphics, "Graphics/PlayerLife.bmp", D3DCOLOR_RGBA(0,0,0,255)))
		Error(true, "Error loading player life texture.\n\tExiting...");
	if (!m_DroneTex.Load(&m_Graphics, "Graphics/Drone.bmp", D3DCOLOR_RGBA(255,0,255,255)))
		Error(true, "Error loading drone texture.\n\tExiting...");
	if (!m_DroneLaserTex.Load(&m_Graphics, "Graphics/DroneLaser.bmp", D3DCOLOR_RGBA(0,0,0,255)))
		Error(true, "Error loading drone laser texture.\n\tExiting...");
	if (!m_ClawTex.Load(&m_Graphics, "Graphics/Claw.bmp", D3DCOLOR_RGBA(255,0,255,255)))
		Error(true, "Error loading claw texture.\n\tExiting...");
	if (!m_ViperTex.Load(&m_Graphics, "Graphics/Viper.bmp", D3DCOLOR_RGBA(255,0,255,255)))
		Error(true, "Error loading viper texture.\n\tExiting...");
	if (!m_SmallAsteroidTex.Load(&m_Graphics, "Graphics/SmallAsteroid.bmp", D3DCOLOR_RGBA(0,0,0,255)))
		Error(true, "Error loading Small Asteroid texture.\n\tExiting...");
	if (!m_BigAsteroidTex.Load(&m_Graphics, "Graphics/BigAsteroid.bmp", D3DCOLOR_RGBA(0,0,0,255)))
		Error(true, "Error loading Big Asteroid texture.\n\tExiting...");
	if (!m_GameOverTex.Load(&m_Graphics, "Graphics/GameOver.bmp", D3DCOLOR_RGBA(0,0,0,255)))
		Error(true, "Error loading Game Over texture\n\tExiting...");
	if (!m_LaserUpgradeTex.Load(&m_Graphics, "Graphics/LaserUp.bmp", D3DCOLOR_RGBA(0,0,0,255)))
		Error(true, "Error loading Laser Upgrade texture\n\tExiting...");
	if (!m_HealthUpgradeTex.Load(&m_Graphics, "Graphics/HealthUp.bmp", D3DCOLOR_RGBA(0,0,0,255)))
		Error(true, "Error loading Health Upgrade texture\n\tExiting...");
	if (!m_RocketTex.Load(&m_Graphics, "Graphics/Rocket.bmp", D3DCOLOR_RGBA(0,0,0,255)))
		Error(true, "Error loading Rocket texture\n\tExiting...");
	if (!m_BombTex.Load(&m_Graphics, "Graphics/Bomb.bmp", D3DCOLOR_RGBA(0,0,0,255)))
		Error(true, "Error loading Bomb texture\n\tExiting...");

	// Initialize game menu objects
	m_Earth = new cEarth(&m_TE, 0);

	// Init variables
	m_Wave = 0;
	m_ScrollX = m_ScrollY = 0;
	m_DelayCounter = 0;

	// Start with the menu
	m_SM.Push(GameMenu, this);

	//m_Graphics.GetD3DDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, true);

	return true;
}

bool cGame::GameRun()
{
	// Play Music
	m_TFC.Play();
	
	// Update Locks on keys
	m_KB.UpdateKeys();

	// Clear the screen
	m_Graphics.ClearScreen();

	// Process the current state
	m_SM.Process(this);

	// Present the backbuffer
	m_Graphics.Present();

	return true;
}

bool cGame::GameEnd()
{
	// Free game objects
	if (m_Earth != NULL)
	{
		delete m_Earth;
		m_Earth = NULL;
	}

	// Free textures
	m_MenuTex.Free();
	m_SpaceTex.Free();
	m_RadarTex.Free();
	m_GreenBlipTex.Free();
	m_RedBlipTex.Free();
	m_GrayBlipTex.Free();
	m_PlayerTex.Free();
	m_WPLT.Free();
	m_NPLT.Free();
	m_SPLT.Free();
	m_HealthMeterTex.Free();
	m_HealthBarTex.Free();
	m_PlayerLifeTex.Free();
	m_DroneTex.Free();
	m_DroneLaserTex.Free();
	m_ClawTex.Free();
	m_ViperTex.Free();
	m_SmallAsteroidTex.Free();
	m_BigAsteroidTex.Free();
	m_GameOverTex.Free();
	m_LaserUpgradeTex.Free();
	m_HealthUpgradeTex.Free();
	m_RocketTex.Free();
	m_BombTex.Free();

	// Release tile engine
	m_TE.Free();

	// Release fonts
	m_Font.Free();
	m_SmallFont.Free();

	// Release graphics stuff
	if (!m_Graphics.GraphicsEnd())
		Error(true, "Error shuting down DirectX Graphics!\nExiting");

	return true;
}

/////////////////////////////////////////////////
// Game State Defintions
/////////////////////////////////////////////////
void cGame::GameMenu(void *DataPtr, long Purpose)
{
	cGame *Game = (cGame*)DataPtr;

	if (Purpose == INIT)
	{
		Game->m_MenuChoice = 0;
	}
	if (Purpose == FRAME)
	{
		// Update logic here
		Game->UpdateMenu();	

		// Draw stuff here
		if (Game->m_Graphics.BeginScene())
		{
			Game->DrawMenu();

			Game->m_Graphics.EndScene();
		}
	}
	if (Purpose == SHUTDOWN)
	{

	}
}

// MainGame state function
void cGame::MainGame(void *DataPtr, long Purpose)
{
	cGame *Game = (cGame*)DataPtr;

	if (Purpose == INIT)
	{
		// Init game
		Game->InitGame();
	}
	if (Purpose == FRAME)
	{
		// Update Game Logic
		Game->UpdateGame();

		// Draw Game stuff
		if (Game->m_Graphics.BeginScene())
		{
			// Draw everything
			Game->DrawGame();

			Game->m_Graphics.EndScene();
		}
		
		// Delete anything dead
		Game->DeleteDeadObjects();

		// Handle all the messages
		Game->m_MM.RunMsgs();

		if (Game->m_KB.IsKeyDown(VK_ESCAPE))
			Game->m_SM.Pop(Game);
	}
	if (Purpose == SHUTDOWN)
	{
		// Shutdown Game
		Game->ShutdownGame();
	}
}

// GameOver function
void cGame::GameOver(void *DataPtr, long Purpose)
{
	cGame *Game = (cGame*)DataPtr;

	if (Purpose == INIT)
	{

	}
	if (Purpose == FRAME)
	{
		// Render stuff
		if (Game->m_Graphics.BeginScene())
		{
			// Draw stuff
			// Draw space background
			Game->m_SpaceTex.Render(0, 0, NULL, D3DXVECTOR2(0.0f, 0.0f), NULL, 1.56f, 1.17f);
	
			// Draw game over screen
			Game->m_GameOverTex.Render(0.0f, 0.0f);

			// Show final score and wave
			if (Game->m_Font.Begin())
			{	
				Game->m_Font.Print("FINAL SCORE:", 150, 200, 0, 0, D3DCOLOR_RGBA(0,255,0,255));
				Game->m_Font.Print("FINAL WAVE:", 450, 200, 0, 0, D3DCOLOR_RGBA(0,255,0,255));
				char Temp[32];
				char Temp2[8];
				itoa(Game->m_FinalScore, Temp, 10);
				itoa(Game->m_FinalWave, Temp2, 10);
				Game->m_Font.Print(Temp, 295, 200, 0, 0, D3DCOLOR_RGBA(255,0,0,255));
				Game->m_Font.Print(Temp2, 585, 200, 0, 0, D3DCOLOR_RGBA(255,0,0,255));

				Game->m_Font.End();
			}

			// End scene
			Game->m_Graphics.EndScene();
		}
		
		// Wait for Escape Key
		if (Game->m_KB.IsKeyDown(VK_ESCAPE))
			Game->m_SM.Pop(Game);
	}
	if (Purpose == SHUTDOWN)
	{

	}
}

void cGame::GamePause(void *DataPtr, long Purpose)
{
	cGame *Game = (cGame*)DataPtr;

	if (Purpose == INIT)
	{

	}
	if (Purpose == FRAME)
	{
		// Pause state, just display "PAUSED" until user hits p key again
		if (Game->m_Graphics.BeginScene())
		{
			if (Game->m_Font.Begin())
			{
				Game->DrawGame();
				Game->m_Font.Print("*----------* PAUSED *----------*", 200, 300, 0, 0, D3DCOLOR_RGBA(255,0,0,255));

				Game->m_Font.End();
			}

			Game->m_Graphics.EndScene();
		}
	
		if (Game->m_KB.IsKeyDown(0x50) && !Game->m_KB.GetLock(0x50)) // P Key
		{
			Game->m_KB.SetLock(0x50);
			Game->m_SM.Pop(Game);
		}
	}
	if (Purpose == SHUTDOWN)
	{

	}
}

/////////////////////////////////////////////////
// General Game functions
/////////////////////////////////////////////////
void cGame::HandleInput()
{
	// Handle input for player
	if (m_KB.IsKeyDown(VK_UP))
		m_Player->SetVelocity(m_Player->GetVelocity()+0.05f);
	else if (m_KB.IsKeyDown(VK_DOWN))
		m_Player->SetVelocity(m_Player->GetVelocity()-0.05f);

	if (m_KB.IsKeyDown(VK_LEFT))
		m_Player->SetAngle(m_Player->GetAngle()+0.03f);
	else if (m_KB.IsKeyDown(VK_RIGHT))
		m_Player->SetAngle(m_Player->GetAngle()-0.03f);

	if (m_KB.IsKeyDown(VK_SPACE) && !m_KB.GetLock(VK_SPACE))
	{
		m_KB.SetLock(VK_SPACE);
		m_Player->ShootLaser(&m_WPLT, &m_NPLT, &m_SPLT);
		m_Laser.Play();
	}

	if (m_KB.IsKeyDown(VK_CONTROL) && !m_KB.GetLock(VK_CONTROL))		
	{
		m_KB.SetLock(VK_CONTROL);
		m_Player->ShootRocket(&m_RocketTex);
	}

	if (m_KB.IsKeyDown(VK_SHIFT) && !m_KB.GetLock(VK_SHIFT))		
	{
		m_KB.SetLock(VK_SHIFT);
		m_Player->DropBomb(&m_BombTex);
	}
	
	if (m_KB.IsKeyDown(0x50) && !m_KB.GetLock(0x50))
	{
		m_KB.SetLock(0x50);
		m_SM.Push(GamePause, this);
	}

	// DEBUG_AID: Press W for weak lasers, N for normal, S for strong
	/*if (m_KB.IsKeyDown(0x57))
		m_Player->SetLaserLevel(WEAK);
	else if (m_KB.IsKeyDown(0x4E))
		m_Player->SetLaserLevel(NORMAL);
	else if (m_KB.IsKeyDown(0x53))
		m_Player->SetLaserLevel(STRONG);*/

	if (m_Player->GetVelocity() > 3.00f)
		m_Player->SetVelocity(3.00f);
	else if (m_Player->GetVelocity() < -3.00f)
		m_Player->SetVelocity(-3.00f);
}

void cGame::DeleteDeadObjects()
{
	// First, delete any dead player lasers
	for (unsigned int i=0; i<m_Player->GetLasers().size(); i++)
	{
		if (m_Player->GetLasers()[i]->GetDead())
		{
			delete m_Player->GetLasers()[i];
			m_Player->GetLasers()[i] = NULL;
			m_Player->GetLasers().erase(m_Player->GetLasers().begin() + i);
			i--;
		}
	}	

	// Go through the enemy manager, if a ships lasers are dead, delete them, if the ship is dead, 
	// clear its lasers then delete it.
	for (unsigned int i=0; i<m_EnemyManager->GetShips().size(); i++)
	{
		// Delete any dead lasers
		for (unsigned int j=0; j<m_EnemyManager->GetShips()[i]->GetLasers().size(); j++)
		{
			if (m_EnemyManager->GetShips()[i]->GetLasers()[j]->GetDead())
			{
				delete m_EnemyManager->GetShips()[i]->GetLasers()[j];
				m_EnemyManager->GetShips()[i]->GetLasers()[j] = NULL;
				m_EnemyManager->GetShips()[i]->GetLasers().erase(m_EnemyManager->GetShips()[i]->GetLasers().begin() + j);
				j--;
			}
		}

		// Delete any dead ships
		if (m_EnemyManager->GetShips()[i]->GetDead())
		{
			// Play an explosion
			m_Explode.Play();
			CreatePowerup(m_EnemyManager->GetShips()[i]->GetX(), m_EnemyManager->GetShips()[i]->GetY());
			m_EnemyManager->GetShips()[i]->ClearLasers();
			delete m_EnemyManager->GetShips()[i];
			m_EnemyManager->GetShips()[i] = NULL;
			m_EnemyManager->GetShips().erase(m_EnemyManager->GetShips().begin() + i);
			i--;
		}
	}

	// Delete dead asteroids
	for (unsigned int i=0; i<m_AM->GetAsteroids().size(); i++)
	{
		if (m_AM->GetAsteroids()[i]->GetDead())
		{
			m_Explode.Play();
			CreatePowerup(m_AM->GetAsteroids()[i]->GetX(), m_AM->GetAsteroids()[i]->GetY());
			delete m_AM->GetAsteroids()[i];
			m_AM->GetAsteroids()[i] = NULL;
			m_AM->GetAsteroids().erase(m_AM->GetAsteroids().begin() + i);
			i--;
		}
	}

	// If the player is dead, reset them
	if (m_Player->GetDead())
		ResetPlayer();
}

void cGame::StartNewWave()
{
	// Print the new wave message
	m_MM.AddMsg(&m_Font, "NEXT WAVE COMMENCING", 300, 200, 0, 1000, COLOR_GREEN);

	if (m_Wave != 1)
	{
		// Play the sound
		m_NWC.Play();	
	}

	// Create enemy ships for the wave
	for (int i=0; i<m_Wave; i++)
	{
		// The number of wave = # of drones	
		m_EnemyManager->AddDrone(7000.0f/m_Wave*i, 500.0f, DRONE_WIDTH, DRONE_HEIGHT, 0.0f, 3.0f*PI/2.0f, &m_DroneTex,
								 30, 5, 60);
		
		// Wave number/3 = # of claws
		if (i % 3 == 0)
			m_EnemyManager->AddClaw(7000.0f/m_Wave*i, 250.0f, CLAW_WIDTH, CLAW_HEIGHT, 0.0f, 0.0f, &m_ClawTex,
									60, 10, 45);

		// Wave number/6 = # of Vipers
		if (i % 6 == 0)
			m_EnemyManager->AddViper(7000.0f/m_Wave*i, 0.0f, VIPER_WIDTH, VIPER_HEIGHT, 0.0f, 0.0f, &m_ViperTex,
									 100, 20, 30);
	}
}

void cGame::ResetPlayer()
{
	m_DelayCounter++;
	m_Player->SetVelocity(0.0f);
	if (m_DelayCounter > 200)
	{
		m_DelayCounter = 0;
		m_Player->SetLives(m_Player->GetLives() - 1);
		if (m_Player->GetLives() < 0)
		{
			m_SM.Pop(this);
			m_SM.Push(GameOver, this);
		}
		else
		{
			m_Player->SetX(4000.0f);
			m_Player->SetY(5000.0f);
			m_Player->SetHealth(100);
			m_Player->SetAngle(PI/2.0f);
			m_Player->SetVelocity(0.5f);
			m_Player->SetLaserLevel(WEAK);
			m_Player->SetDead(false);
		}
	}
}

void cGame::CreatePowerup(float X, float Y)
{
	int Number = rand()%3;
	if (Number == 1)
	{
		Number = rand()%12;
		if (Number <= 2)
			m_PM->AddPowerup(X, Y, 0, 1000, &m_LaserUpgradeTex, LASER);
		else if (Number >=3 && Number <= 5)
			m_PM->AddPowerup(X, Y, 0, 1000, &m_HealthUpgradeTex, HEALTH);
		else if (Number >= 6 && Number <= 8)
			m_PM->AddPowerup(X, Y, 0, 1000, &m_RocketTex, ROCKET);
		else if (Number == 9 || Number == 10)
			m_PM->AddPowerup(X, Y, 0, 1000, &m_BombTex, BOMB);
		else if (Number == 11)
			m_PM->AddPowerup(X, Y, 0, 1000, &m_PlayerLifeTex, LIFE);
	}
}

void cGame::InitGame()
{
	// seed random number generator
	srand(GetTickCount());
	
	// Reset wave counter 
	m_Wave = 0;

	// Create Game objects
	m_Player = new cPlayerShip(4000.0f, 5000.0f, 54.0f, 68.0f, 1.0f, PI/2.0f,
									 &m_PlayerTex, 100, 20, 3, 3, 1); 

	// Initialize Enemy Manager
	m_EnemyManager = new cEnemyManager();

	// Initialize explosion manager
	m_EM = new cExplosionManager(&m_TE);

	// Initialize asteroid Manager
	m_AM = new cAsteroidManager();
	InitAsteroids();
	
	// Initialize the powerup manager
	m_PM = new cPowerupManager();

	// Add "ENTERING SECTOR GAMMA_13 to the Msg List
	m_MM.AddMsg(&m_Font, "ENTERING SECTOR GAMMA-13", 260, 370, 0, 500, COLOR_GREEN);

	// Play sound
	m_ESG13.Play();
}

void cGame::InitAsteroids()
{
	for (int i=0; i<5; i++)
		m_AM->AddAsteroid(false, (float)(rand()%7000), i*3000.0f, BIG_ASTEROID_WIDTH, BIG_ASTEROID_HEIGHT,
						  0.5f, i*1.2f, &m_BigAsteroidTex);

	for (int i=0; i<10; i++)
		m_AM->AddAsteroid(true, (float)(rand()%8000), i*1000.0f, SMALL_ASTEROID_WIDTH, SMALL_ASTEROID_HEIGHT,
						  1.5f, i*1.2f, &m_SmallAsteroidTex);
}

void cGame::ShutdownGame()
{
	// delete game objects
	if (m_Player != NULL)
	{
		m_FinalScore = m_Player->GetScore();
		m_FinalWave = m_Wave;
		delete m_Player;
		m_Player = NULL;
	}
	if (m_EnemyManager != NULL)
	{
		delete m_EnemyManager;
		m_EnemyManager = NULL;
	}
	if (m_EM != NULL)
	{
		delete m_EM;
		m_EM = NULL;
	}
	if (m_AM != NULL)
	{
		delete m_AM;
		m_AM = NULL;
	}
	if (m_PM != NULL)
	{
		delete m_PM;
		m_PM = NULL;
	}
}

void cGame::UpdateMenu()
{
	if (m_KB.IsKeyDown(VK_UP))
		m_MenuChoice = 0;
	else if (m_KB.IsKeyDown(VK_DOWN))
		m_MenuChoice = 1;
	
	if (m_KB.IsKeyDown(VK_RETURN))
	{
		if (m_MenuChoice == 0)
			m_SM.Push(MainGame, this);
		else if (m_MenuChoice == 1)
			PostQuitMessage(0);
	}
}

void cGame::UpdateGame()
{
	// Start a new wave if neccessary
	if (m_EnemyManager->GetShips().empty())
	{
		m_Wave++;
		StartNewWave();
	}

	// Get Input
	if (!m_Player->GetDead())
		HandleInput();
	
	// Update Player
	UpdatePlayer();

	// Update aliens
	m_EnemyManager->UpdateShips(m_Player, &m_DroneLaserTex, m_EM, m_AM);

	// Update asteroids
	m_AM->UpdateAsteroids();

	// Update powerups
	m_PM->UpdatePowerups();

	// Update explosions
	m_EM->UpdateExplosions();

	// Update scrolling stuff
	UpdateScroll();
}

void cGame::UpdateScroll()
{
	m_ScrollX = (int)m_Player->GetX() - 373;
	m_ScrollY = (int)m_Player->GetY() - 266;

	if (m_ScrollX > 7200)
		m_ScrollX = 7200;
	else if (m_ScrollX < 0)
		m_ScrollX = 0;

	if (m_ScrollY > 5400)
		m_ScrollY = 5400;
	else if (m_ScrollY < 0)
		m_ScrollY = 0;
}

void cGame::UpdatePlayer()
{
	// Update player position and lasers against boundaries, etc.
	m_Player->Update(m_ScrollX, m_ScrollY, m_EM);

	// Update lasers against other ships
	for (unsigned int i=0; i<m_Player->GetLasers().size(); i++)
	{
		for (unsigned int j=0; j<m_EnemyManager->GetShips().size(); j++)
		{
			if (Collision((int)m_Player->GetLasers()[i]->GetX(), (int)m_Player->GetLasers()[i]->GetY(),
					      (int)m_Player->GetLasers()[i]->GetWidth(), (int)m_Player->GetLasers()[i]->GetHeight(),
						  (int)m_EnemyManager->GetShips()[j]->GetX(), (int)m_EnemyManager->GetShips()[j]->GetY(),
						  (int)m_EnemyManager->GetShips()[j]->GetWidth(), (int)m_EnemyManager->GetShips()[j]->GetHeight()))
			{
				// Take health away from the enemy ship
				m_EnemyManager->GetShips()[j]->SetHealth(m_EnemyManager->GetShips()[j]->GetHealth() - m_Player->GetLasers()[i]->GetDamage());
				
				// Delete the laser that hit the ship
				m_Player->GetLasers()[i]->SetDead(true);

				// Put an explosion there
				m_EM->AddExplosion(m_Player->GetLasers()[i]->GetX()-((32.0f-m_Player->GetLasers()[i]->GetWidth())/2),
								   m_Player->GetLasers()[i]->GetY()-((32.0f-m_Player->GetLasers()[i]->GetHeight())/2),
								   m_Player->GetVelocity(),
								   m_Player->GetAngle(),
								   1.0f);

				// Play a small exploding sound
				m_SmExp.Play();

				// If the health is below zero, destroy the ship
				if (m_EnemyManager->GetShips()[j]->GetHealth() <= 0)
				{
					m_EM->AddExplosion(m_EnemyManager->GetShips()[j]->GetX()-((128.0f-m_EnemyManager->GetShips()[j]->GetWidth())/2), 
									   m_EnemyManager->GetShips()[j]->GetY()-((128.0f-m_EnemyManager->GetShips()[j]->GetHeight())/2),
									   m_EnemyManager->GetShips()[j]->GetVelocity(),
									   m_EnemyManager->GetShips()[j]->GetAngle(),
									   4.0f);
					m_EnemyManager->GetShips()[j]->SetDead(true);

					m_Player->SetScore(m_Player->GetScore() + 5);
				}
			}
		}

		// Update against asteroids
		for (unsigned int n=0; n<m_AM->GetAsteroids().size(); n++)
		{
			if (Collision((int)m_Player->GetLasers()[i]->GetX(), (int)m_Player->GetLasers()[i]->GetY(),
					      (int)m_Player->GetLasers()[i]->GetWidth(), (int)m_Player->GetLasers()[i]->GetHeight(),
						  (int)m_AM->GetAsteroids()[n]->GetX()+10, (int)m_AM->GetAsteroids()[n]->GetY()+10,
						  (int)m_AM->GetAsteroids()[n]->GetWidth()-10, (int)m_AM->GetAsteroids()[n]->GetHeight()-10))
			{
				// Kill the laser
				m_Player->GetLasers()[i]->SetDead(true);
				
				// Create an explosion
				m_EM->AddExplosion(m_Player->GetLasers()[i]->GetX()-((32.0f-m_Player->GetWidth())/2),
								   m_Player->GetLasers()[i]->GetY()-((32.0f-m_Player->GetHeight())/2),
								   m_Player->GetVelocity(), m_Player->GetAngle(), 1.0f);

				// Play a small exploding sound
				m_SmExp.Play();

				// If the asteroid is destructible, Mark it as dead and blow it up
				if (m_AM->GetAsteroids()[n]->GetDestructible())
				{
					// Mark as dead
					m_AM->GetAsteroids()[n]->SetDead(true);
					
					// New explosion
					m_EM->AddExplosion(m_AM->GetAsteroids()[n]->GetX()-((96.0f-m_AM->GetAsteroids()[n]->GetWidth())/2), 
									   m_AM->GetAsteroids()[n]->GetY()-((96.0f-m_AM->GetAsteroids()[n]->GetHeight())/2),
									   m_AM->GetAsteroids()[n]->GetVelocity(), m_AM->GetAsteroids()[n]->GetAngle(),
									   3.0f);
				}
			}
		}	
	}

	for (unsigned int j=0; j<m_EnemyManager->GetShips().size(); j++)
	{
		if (Collision((int)m_Player->GetX(), (int)m_Player->GetY(),
				      (int)m_Player->GetWidth(), (int)m_Player->GetHeight(),
					  (int)m_EnemyManager->GetShips()[j]->GetX(), (int)m_EnemyManager->GetShips()[j]->GetY(),
					  (int)m_EnemyManager->GetShips()[j]->GetWidth(), (int)m_EnemyManager->GetShips()[j]->GetHeight()) &&
					  m_Player->GetDead() != true)
		{
			// Create a big explosion for the enemy
			m_EM->AddExplosion(m_EnemyManager->GetShips()[j]->GetX()-((128.0f-m_EnemyManager->GetShips()[j]->GetWidth())/2),
							   m_EnemyManager->GetShips()[j]->GetY()-((128.0f-m_EnemyManager->GetShips()[j]->GetHeight())/1),
							   m_EnemyManager->GetShips()[j]->GetVelocity(),
							   m_EnemyManager->GetShips()[j]->GetAngle(),
							   4.0f);

			// Kill Player
			m_Explode.Play();
			m_Player->SetHealth(0);

			// Kill Alien ship
			m_EnemyManager->GetShips()[j]->SetDead(true);
		}
	}

	// Collisions between player and asteroids
	for (unsigned int n=0; n<m_AM->GetAsteroids().size(); n++)
	{
		if (Collision((int)m_Player->GetX(), (int)m_Player->GetY(),
				      (int)m_Player->GetWidth(), (int)m_Player->GetHeight(),
					  (int)m_AM->GetAsteroids()[n]->GetX()+10, (int)m_AM->GetAsteroids()[n]->GetY()+10,
					  (int)m_AM->GetAsteroids()[n]->GetWidth()-10, (int)m_AM->GetAsteroids()[n]->GetHeight()-10))
		{
			// Kill the laser
			m_Player->SetHealth(0);
			
			// If the asteroid is destructible, Mark it as dead and blow it up
			if (m_AM->GetAsteroids()[n]->GetDestructible())
			{
				// Mark as dead
				m_AM->GetAsteroids()[n]->SetDead(true);
				
				// New explosion
				m_EM->AddExplosion(m_AM->GetAsteroids()[n]->GetX()-((96.0f-m_AM->GetAsteroids()[n]->GetWidth())/2), 
								   m_AM->GetAsteroids()[n]->GetY()-((96.0f-m_AM->GetAsteroids()[n]->GetHeight())/2),
								   m_AM->GetAsteroids()[n]->GetVelocity(), m_AM->GetAsteroids()[n]->GetAngle(),
								   3.0f);
			}
		}
	}	

	// Collisions between player and powerups
	for (unsigned int i=0; i<m_PM->GetPowerups().size(); i++)
	{
		if (Collision((int)m_Player->GetX(), (int)m_Player->GetY(), 
					  (int)m_Player->GetWidth(), (int)m_Player->GetHeight(),
					  (int)m_PM->GetPowerups()[i]->GetX(), (int)m_PM->GetPowerups()[i]->GetY(), 32, 32))
		{
			switch (m_PM->GetPowerups()[i]->GetType())
			{
			case LASER:
				if (m_Player->GetLaserLevel() == WEAK)
					m_Player->SetLaserLevel(NORMAL);
				else if (m_Player->GetLaserLevel() == NORMAL)
					m_Player->SetLaserLevel(STRONG);
				break;

			case HEALTH:
				m_Player->SetHealth(m_Player->GetHealth() + 25);
				if (m_Player->GetHealth() > 100)
					m_Player->SetHealth(100);
				break;
				
			case LIFE:
				if (m_Player->GetLives() < 3)
					m_Player->SetLives(m_Player->GetLives() + 1);
				break;

			case ROCKET:
				if (m_Player->GetRockets() < 3)
					m_Player->SetRockets(m_Player->GetRockets() + 1);
				break;

			case BOMB:
				if (m_Player->GetBombs() < 3)
					m_Player->SetBombs(m_Player->GetBombs() + 1);
				break;

			default:
				break;
			}

			// delete the powerup
			delete m_PM->GetPowerups()[i];
			m_PM->GetPowerups()[i] = NULL;
			m_PM->GetPowerups().erase(m_PM->GetPowerups().begin() + i);
			i--;
		}
	}
}

void cGame::DrawMenu()
{
	// Draw space background
	m_SpaceTex.Render(0, 0, NULL, D3DXVECTOR2(0.0f, 0.0f), NULL, 1.56f, 1.17f);
	
	// Draw Menu texture
	m_MenuTex.Render(0, 0);
	
	// Draw earth selector
	m_Earth->Draw(m_MenuChoice);
}

void cGame::DrawGame()
{
	// Draw stars background
	DrawStars();

	// Draw the player's lasers
	for (unsigned int i=0; i<m_Player->GetLasers().size(); i++)
		m_Player->GetLasers()[i]->Draw(m_ScrollX, m_ScrollY);

	// Draw the player's ship
	if (!m_Player->GetDead())
		m_Player->Draw(m_ScrollX, m_ScrollY);

	// Draw aliens
	m_EnemyManager->DrawShips(m_ScrollX, m_ScrollY);

	// Draw Asteroids
	m_AM->DrawAsteroids(m_ScrollX, m_ScrollY);

	// Draw powerups
	m_PM->DrawPowerups(m_ScrollX, m_ScrollY);

	// Draw Explosions
	m_EM->DrawExplosions(m_ScrollX, m_ScrollY);
	
	// Draw radar system
	DrawRadar();

	// Draw stats
	DrawStats();
}

void cGame::DrawStars()
{
	for (int i=0; i<40; i++)
	{
		for (int j=0; j<30; j++)
			m_TE.DrawTile(1, 0, i*200-m_ScrollX, j*200-m_ScrollY);
	}
}

void cGame::DrawRadar()
{
	// Draw radar frame
	m_RadarTex.Render(635.0f, 5.0f);

	// Draw radar blips
	// First draw players blip
	m_GreenBlipTex.Render(m_Player->GetX()/50+635, m_Player->GetY()/50+5);

	// Draw enemy ships
	for (unsigned int i=0; i<m_EnemyManager->GetShips().size(); i++)
		m_RedBlipTex.Render(m_EnemyManager->GetShips()[i]->GetX()/50+635, 
							m_EnemyManager->GetShips()[i]->GetY()/50+5);

	// Draw asteroids
	for (unsigned int i=0; i<m_AM->GetAsteroids().size(); i++)
		m_GrayBlipTex.Render(m_AM->GetAsteroids()[i]->GetX()/50+635, m_AM->GetAsteroids()[i]->GetY()/50+5);
}

void cGame::DrawStats()
{
	// Draw health stuff first, then score and wave number
	// Draw health meter
	m_HealthMeterTex.Render(5, 5);

	// Draw health inside health meter
	for (int i=0; i<m_Player->GetHealth(); i++)
		m_HealthBarTex.Render((float)i+8, (float)8);

	// Draw lives next to the health bar
	for (int i=0; i<m_Player->GetLives(); i++)
		m_PlayerLifeTex.Render((float)i*40+120, (float)5);

	// Draw Rockets and bombs in the bottom left corner
	for (int i=0; i<m_Player->GetRockets(); i++)
		m_RocketTex.Render((float)i*40+10, (float)536, NULL, D3DXVECTOR2(0.0f, 0.0f), NULL, 0.5f, 0.5f);

	for (int i=0; i<m_Player->GetBombs(); i++)
		m_BombTex.Render((float)i*40+10, (float)564, NULL, D3DXVECTOR2(0.0f, 0.0f), NULL, 0.5f, 0.5f); 

	char Temp[16]; // Temporary char buffer

	// Convert player stats to chars, and print them
	if (m_SmallFont.Begin())
	{
		// Player's score
		itoa(m_Player->GetScore(), Temp, 10);
		m_SmallFont.Print("SCORE:", 5, 30, 0, 0, COLOR_GREEN);
		m_SmallFont.Print(Temp, 70, 30, 0, 0, COLOR_GREEN);

		// Wave number
		itoa(m_Wave, Temp, 10);
		m_SmallFont.Print("WAVE:", 5, 55, 0, 0, COLOR_GREEN);
		m_SmallFont.Print(Temp, 60, 55, 0, 0, COLOR_GREEN);

		m_SmallFont.End();
	}
}

// Main windows loop, game entry point
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR CmdLine, int nCmdShow)
{
	cGame Game;
	Game.BaseRun();
}