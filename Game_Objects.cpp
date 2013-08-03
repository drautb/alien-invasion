/////////////////////////////////////////////////
// Game_Objects.cpp
// Source file for game objects
/////////////////////////////////////////////////

// Todo list
// TODO: Create powerup system
// TODO: Create powerup graphics
// TODO: Create new AI Routines
// TODO: Cleanup code

// Include everything
#include "Global_Engine.h"

// Collision function
BOOL Collision(int X1, int Y1, int W1, int H1, int X2, int Y2, int W2, int H2)
{
	// Put coords into a rect, check for rect overlap
	RECT R1, R2, R3;

	// First rect
	R1.top = Y1;
	R1.left = X1;
	R1.bottom = R1.top + H1;
	R1.right = R1.left + W1;

	// Second rect
	R2.top = Y2;
	R2.left = X2;
	R2.bottom = R2.top + H2;
	R2.right = R2.left + W2;

	// Intersection?
	return IntersectRect(&R3, &R1, &R2);
}

// cEarth Definitions
void cEarth::Draw(int MenuChoice)
{
	// Get correct y coordinate
	int Y=0;
	if (MenuChoice == 0)
		Y = 240;
	else if (MenuChoice == 1)
		Y = 360;

	// Update animation
	m_AnimCount++;
	if (m_AnimCount > 10)
	{	
		m_AnimCount = 0;
		m_Frame++;
		if (m_Frame > 11)
			m_Frame = 0;
	}

	// Draw it 
	m_TE->DrawTile(0, m_Frame, 345, Y);
}	

/////////////////////////////////////////////////
// Begin defs for in-game object classes
/////////////////////////////////////////////////
// cMessage
void cMessage::Run()
{
	// Update life counter
	m_Life++;

	if (m_Life < m_LifeSpan)
	{
		if (m_Font->Begin())
		{
			m_Font->Print((char*)m_Text.c_str(), m_X, m_Y, 0, 0, m_Color);
			m_Font->End();
		}
	}
}

// cMessageManager
void cMessageManager::AddMsg(cFont *Font, std::string Text, int X, int Y, int Life, int LifeSpan, D3DCOLOR Color)
{
	// Create a new message
	cMessage *Message = new cMessage(Font, Text, X, Y, Life, LifeSpan, Color);

	// Put the message in the vector
	m_MsgList.push_back(Message);
}

void cMessageManager::ClearMsgs()
{
	// Delete all the messages in the vector
	for (unsigned int i=0; i<m_MsgList.size(); i++)
	{
		if (m_MsgList[i] != NULL)
		{
			delete m_MsgList[i];
			m_MsgList[i] = NULL;
		}
	}

	// clear vector
	m_MsgList.clear();
}

void cMessageManager::RunMsgs()
{
	// Go through the vector. run each message, if the message is dead, delete it.
	for (unsigned int i=0; i<m_MsgList.size(); i++)
	{
		m_MsgList[i]->Run();
		if (m_MsgList[i]->GetLife() > m_MsgList[i]->GetLifeSpan())
		{
			delete m_MsgList[i];
			m_MsgList[i] = NULL;
			m_MsgList.erase(m_MsgList.begin() + i);
			i--;
		}
	}
}

// cMovingObject
cMovingObject::cMovingObject(cMovingObject &rhs)
{
	m_Texture = new cTexture();
	m_Texture = rhs.GetTexture();
}

cMovingObject &cMovingObject::operator=(const cMovingObject &rhs)
{
	if (this == &rhs)
		return *this;
	else
	{
		m_Texture = rhs.GetTexture();
		return *this;
	}
}

void cMovingObject::SetAngle(float Angle)
{
	m_Angle = Angle;
	if (m_Angle > 2*PI)
		m_Angle = 0.0f;
	if (m_Angle < 0.0f)
		m_Angle = 2*PI;
}

void cMovingObject::Draw(int ScrollX, int ScrollY)
{
	// Compute rotation center
	float X, Y;
	X = m_Width/2;
	Y = m_Height/2;
	D3DXVECTOR2 RotationCenter(X, Y);

	// Draw it
	m_Texture->Render(m_X-ScrollX, m_Y-ScrollY, NULL, RotationCenter, m_Angle);
}

// cPlayerShip
std::vector<cLaser*> &cPlayerShip::GetLasers()
{
	std::vector<cLaser*> &Lasers = m_Lasers;
	return m_Lasers;
}

void cPlayerShip::Update(int ScrollX, int ScrollY, cExplosionManager *EM)
{
	// Add the x and y velocities to the x and y position taking angle into account
	SetX(GetX() + cos(-GetAngle()) * GetVelocity());
	SetY(GetY() + sin(-GetAngle()) * GetVelocity());

	// Check for boundary collisions
	if (GetX() < 0)
		SetX(WORLD_WIDTH-GetWidth());
	else if (GetX() > WORLD_WIDTH-GetWidth())
		SetX(0);
	
	if (GetY() < 0)
		SetY(WORLD_HEIGHT-GetHeight());
	else if (GetY() > WORLD_HEIGHT-GetHeight())
		SetY(0);

	// Update players lasers
	for (unsigned int i=0; i<m_Lasers.size(); i++)
	{
		m_Lasers[i]->Update();
		if ((m_Lasers[i]->GetX() > (ScrollX + SCREEN_WIDTH)) ||
			(m_Lasers[i]->GetX() < ScrollX)					 || 
			(m_Lasers[i]->GetY() > ScrollY + SCREEN_HEIGHT)  ||
			(m_Lasers[i]->GetY() < ScrollY))
			m_Lasers[i]->SetDead(true);
	}

	// If the players health is <= 0, they are dead
	if (GetHealth() <= 0 && !GetDead())
	{
		SetDead(true);

		// Create explosion for the player
		EM->AddExplosion(GetX()-((128.0f-GetWidth())/2), GetY()-((128.0f-GetHeight())/2),
						 GetVelocity(), GetAngle(), 4.0f);
	}
}

void cPlayerShip::ShootLaser(cTexture *WeakTex, cTexture *NormTex, cTexture *StrongTex)
{
	// Create a new laser
	float X, Y;
	// Set xy position
	X = (GetX()+GetWidth()/2)-9.5f;
	Y = (GetY()+GetHeight()/2)-3.5f;

	switch (m_LaserLevel)
	{
	case WEAK:
		{
			// Make sure laser strength is correct
			SetLaserStrength(20);

			// Create a new laser
			cLaser *Laser = new cLaser(X, Y, 19.0f, 7.0f, GetVelocity()+4.0f, GetAngle(), 
									   WeakTex, GetLaserStrength());

			// Put the new laser in the vector
			m_Lasers.push_back(Laser);
	
			break;
		}
	case NORMAL:
		{
			// Make sure laser strength is correct
			SetLaserStrength(40);

			// Create new Laser
			cLaser *Laser = new cLaser(X, Y, 19.0f, 7.0f, GetVelocity()+4.0f, GetAngle(), 
									   NormTex, GetLaserStrength());
			
			// Put the laser on the vector
			m_Lasers.push_back(Laser);

			break;
		}
	case STRONG:
		{
			// Make sure laser strength is correct
			SetLaserStrength(60);

			// Create a new laser
			cLaser *Laser = new cLaser(X, Y, 19.0f, 7.0f, GetVelocity()+4.0f, GetAngle(), 
									   StrongTex, GetLaserStrength());
			// Put the laser on the vector
			m_Lasers.push_back(Laser);

			break;
		}
	default:
		break;
	}
}

void cPlayerShip::ShootRocket(cTexture *RocketTex)
{
	if (GetRockets() > 0)
	{
		// Set xy position
		float X, Y;
		X = (GetX()+GetWidth()/2)-16.0f;
		Y = (GetY()+GetHeight()/2)-16.0f;
		
		// Create a new laser with rocket picture and damage
		cLaser *Rocket = new cLaser(X, Y, 32.0f, 32.0f, GetVelocity()+4.0f, GetAngle(), RocketTex, 100);
	
		// Add it to the laser vector
		m_Lasers.push_back(Rocket);

		SetRockets(GetRockets() - 1);
	}
}

void cPlayerShip::DropBomb(cTexture *BombTex)
{
	if (GetBombs() > 0)
	{
		// Set xy position
		float X, Y;
		X = (GetX()+GetWidth()/2)-16.0f;
		Y = (GetY()+GetHeight()/2)-16.0f;
	
		// A bomb is programmed as a laser that goes behind the player, and is an instant kill
		cLaser *Bomb = new cLaser(X, Y, 32.0f, 32.0f, GetVelocity()-GetVelocity()+0.5f, GetAngle()+PI, BombTex, 100);
	
		// add the bomb to the vector
		m_Lasers.push_back(Bomb);

		SetBombs(GetBombs() - 1);
	}
}

void cPlayerShip::ClearLasers()
{
	// Delete any lasers that are there, and clear the vector
	for (unsigned int i=0; i<m_Lasers.size(); i++)
	{
		if (m_Lasers[i] != NULL)
		{
			delete m_Lasers[i];
			m_Lasers[i] = NULL;
		}
	}

	m_Lasers.clear();
}

// Basic enemy ship functions
cEnemyShip::cEnemyShip(cEnemyShip &rhs)
{
	for (unsigned int i=0; i<m_Lasers.size(); i++)
	{
		m_Lasers[i] = new cLaser();
		m_Lasers[i] = rhs.GetLasers()[i];
	}
}

cEnemyShip &cEnemyShip::operator=(cEnemyShip &rhs)
{
	if (this == &rhs)
		return *this;
	else
	{
		for (unsigned int i=0; i<m_Lasers.size(); i++)
			m_Lasers[i] = rhs.GetLasers()[i];
		return *this;
	}
}

void cEnemyShip::UpdateLasers()
{
	// For all its lasers, update them
	for (unsigned int i=0; i<m_Lasers.size(); i++)
		m_Lasers[i]->Update();
}

void cEnemyShip::ClearLasers()
{
	// Go through the vector, delete every laser
	for (unsigned int i=0; i<m_Lasers.size(); i++)
	{
		if (m_Lasers[i] != NULL)
		{
			delete m_Lasers[i];
			m_Lasers[i] = NULL;
		}
	}

	// Clear the vector
	m_Lasers.clear();
}

std::vector<cLaser*> &cEnemyShip::GetLasers()
{
	std::vector<cLaser*> &Lasers = m_Lasers;
	return Lasers;
}

// Update AI defs for enemy ships
void cDrone::UpdateAI(float PlayerX, float PlayerY, cTexture *LaserTex)
{
	// Update position, angle, velocity, etc.	
	// Get the x and y distances between drone and player
	float DistX, DistY;
	DistX = PlayerX - GetX();
	DistY = PlayerY - GetY();

	// Get the vector length between them
	float VectorDist = sqrt(pow(DistX,2) + pow(DistY,2));

	// Normalize distances
	float NormDistX = DistX/VectorDist;

	// Take the arccosine of the normalized distance to get the angle the ship needs to be at
	float ReqAngle = acos(NormDistX);
	
	// if the player is below the alien, we want the mirrored angle
	if (PlayerY > GetY())
	{
		float AngleAddition = (PI - ReqAngle) * 2;
		ReqAngle += AngleAddition;
	}

	// if the ship is not at that angle +- a few degrees, rotate it, if it is, increase velocity
	if ((GetAngle() > ReqAngle-0.5f) && (GetAngle() < ReqAngle+0.5f))
	{
		// Shoot a laser every few secs
		if (GetFireCount() == 0)
			ShootLaser(LaserTex);
	}
	
	// Head off at a random angle
	int Number = rand() % 3;
	switch (Number)
	{
	case 0:
		break;
	case 1:
		SetAngle(GetAngle() + 0.03f);
		break;
	case 2:
		SetAngle(GetAngle() - 0.03f);
		break;
	default:
		break;
	}

	if (GetVelocity() != 2.0f)
		SetVelocity(2.0f);

	SetX(GetX() + cos(-GetAngle()) * GetVelocity());
	SetY(GetY() + sin(-GetAngle()) * GetVelocity());

	// Update laser firing delay
	SetFireCount(GetFireCount()+1);
	if (GetFireCount() > GetFireDelay())
		SetFireCount(0);

	// Update lasers
	UpdateLasers();
}
	
void cDrone::ShootLaser(cTexture *Texture)
{
	// Computer correct X,Y
	float X, Y;
	X = GetX()+(GetWidth()/2)-6.5f;
	Y = GetY()+(GetHeight()/2)-6.5f;

	// Create a new laser
	cLaser *Laser = new cLaser(X, Y, 13.0f, 13.0f, GetVelocity()+3.0f, GetAngle(), 
							   Texture, GetLaserStrength());

	// Put it in the vector
	GetLasers().push_back(Laser);
}

// cClaw stuff
void cClaw::UpdateAI(float PlayerX, float PlayerY, cTexture *LaserTex)
{
	// Update position, angle, velocity, etc.	
	// Get the x and y distances between drone and player
	float DistX, DistY;
	DistX = PlayerX - GetX();
	DistY = PlayerY - GetY();

	// Get the vector length between them
	float VectorDist = sqrt(pow(DistX,2) + pow(DistY,2));

	// Normalize distances
	float NormDistX = DistX/VectorDist;

	// Take the arccosine of the normalized distance to get the angle the ship needs to be at
	float ReqAngle = acos(NormDistX);
	
	// if the player is below the alien, we want the mirrored angle
	if (PlayerY > GetY())
	{
		float AngleAddition = (PI - ReqAngle) * 2;
		ReqAngle += AngleAddition;
	}

	// if the ship is not at that angle +- a few degrees, rotate it, if it is, increase velocity
	if ((GetAngle() > ReqAngle-0.05f) && (GetAngle() < ReqAngle+0.05f))
	{
		// Shoot a laser every few secs
		if (GetFireCount() == 0)
			ShootLaser(LaserTex);

		// Increase velocity
		SetVelocity(GetVelocity() + 0.01f);
	}
	else
	{
		float AngleDiff = abs(ReqAngle - GetAngle());
		if (ReqAngle < GetAngle() && AngleDiff < PI)
			SetAngle(GetAngle() - 0.01f);
		else if (ReqAngle < GetAngle() && AngleDiff > PI)
			SetAngle(GetAngle() + 0.01f);
		else if (ReqAngle > GetAngle() && AngleDiff < PI)
			SetAngle(GetAngle() + 0.01f);
		else if (ReqAngle > GetAngle() && AngleDiff > PI)
			SetAngle(GetAngle() - 0.01f);
	}

	if (GetVelocity() != 2.0f)
		SetVelocity(2.0f);

	SetX(GetX() + cos(-GetAngle()) * GetVelocity());
	SetY(GetY() + sin(-GetAngle()) * GetVelocity());

	// Update laser firing delay
	SetFireCount(GetFireCount()+1);
	if (GetFireCount() > GetFireDelay())
		SetFireCount(0);

	// Update lasers
	UpdateLasers();
}

void cClaw::ShootLaser(cTexture *Texture)
{
	// Computer correct X,Y
	float X, Y;
	X = GetX()+(GetWidth()/2)-6.5f;
	Y = GetY()+(GetHeight()/2)-6.5f;

	// Create a new laser
	cLaser *Laser = new cLaser(X, Y, 13.0f, 13.0f, GetVelocity()+3.0f, GetAngle(), 
							   Texture, GetLaserStrength());

	// Put it in the vector
	GetLasers().push_back(Laser);
}

// cViper stuff
void cViper::UpdateAI(float PlayerX, float PlayerY, cTexture *LaserTex)
{
	// Update position, angle, velocity, etc.	
	// Get the x and y distances between drone and player
	float DistX, DistY;
	DistX = PlayerX - GetX();
	DistY = PlayerY - GetY();

	// Get the vector length between them
	float VectorDist = sqrt(pow(DistX,2) + pow(DistY,2));

	// Normalize distances
	float NormDistX = DistX/VectorDist;

	// Take the arccosine of the normalized distance to get the angle the ship needs to be at
	float ReqAngle = acos(NormDistX);
	
	// if the player is below the alien, we want the mirrored angle
	if (PlayerY > GetY())
	{
		float AngleAddition = (PI - ReqAngle) * 2;
		ReqAngle += AngleAddition;
	}

	// if the ship is not at that angle +- a few degrees, rotate it, if it is, increase velocity
	if ((GetAngle() > ReqAngle-0.05f) && (GetAngle() < ReqAngle+0.05f))
	{
		// Shoot a laser every few secs
		if (GetFireCount() == 0)
			ShootLaser(LaserTex);

		// Increase velocity
		SetVelocity(GetVelocity() + 0.01f);
	}
	else
	{
		float AngleDiff = abs(ReqAngle - GetAngle());
		if (ReqAngle < GetAngle() && AngleDiff < PI)
			SetAngle(GetAngle() - 0.02f);
		else if (ReqAngle < GetAngle() && AngleDiff > PI)
			SetAngle(GetAngle() + 0.02f);
		else if (ReqAngle > GetAngle() && AngleDiff < PI)
			SetAngle(GetAngle() + 0.02f);
		else if (ReqAngle > GetAngle() && AngleDiff > PI)
			SetAngle(GetAngle() - 0.02f);
	}

	if (GetVelocity() != 2.0f)
		SetVelocity(2.0f);

	SetX(GetX() + cos(-GetAngle()) * GetVelocity());
	SetY(GetY() + sin(-GetAngle()) * GetVelocity());

	// Update laser firing delay
	SetFireCount(GetFireCount()+1);
	if (GetFireCount() > GetFireDelay())
		SetFireCount(0);

	// Update lasers
	UpdateLasers();
}

void cViper::ShootLaser(cTexture *Texture)
{
	// Computer correct X,Y
	float X, Y;
	X = GetX()+(GetWidth()/2)-6.5f;
	Y = GetY()+(GetHeight()/2)-6.5f;

	// Create a new laser
	cLaser *Laser = new cLaser(X, Y, 13.0f, 13.0f, GetVelocity()+3.0f, GetAngle(), 
							   Texture, GetLaserStrength());

	// Put it in the vector
	GetLasers().push_back(Laser);
}

// cEnemyManager stuff
void cEnemyManager::ClearShips()
{
	// For each ship in the vector, delete it
	for (unsigned int i=0; i<m_Ships.size(); i++)
	{
		if (m_Ships[i] != NULL)
		{
			delete m_Ships[i];
			m_Ships[i] = NULL;
		}
	}

	// Clear vector
	m_Ships.clear();
}

void cEnemyManager::UpdateShips(cPlayerShip *Player, cTexture *LaserTex, cExplosionManager *EM, 
								cAsteroidManager *AM)
{
	// Computer scroll values
	int ScrollX = (int)Player->GetX() - 373;
	int ScrollY = (int)Player->GetY() - 266;

	// For each ship in the vector, call its update function
	for (unsigned int i=0; i<m_Ships.size(); i++)
	{
		// Update the ship and its lasers
		m_Ships[i]->UpdateAI(Player->GetX(), Player->GetY(), LaserTex);

		// Update against screen edges
		if (m_Ships[i]->GetX() > 8000.0f)
			m_Ships[i]->SetX(0.0f);
		else if (m_Ships[i]->GetX() < -m_Ships[i]->GetWidth())
			m_Ships[i]->SetX(8000.0f);
		
		if (m_Ships[i]->GetY() > 6000.0f)
			m_Ships[i]->SetY(0.0f);
		else if (m_Ships[i]->GetY() < -m_Ships[i]->GetHeight())
			m_Ships[i]->SetY(6000.0f);
		
		// Check for collisions between alien lasers and player
		for (unsigned int j=0; j<m_Ships[i]->GetLasers().size(); j++)
		{
			// If there is a collision, handle it
			if (Collision((int)Player->GetX(), (int)Player->GetY(), 
						  (int)Player->GetWidth(), (int)Player->GetHeight(),
						  (int)m_Ships[i]->GetLasers()[j]->GetX(), 
						  (int)m_Ships[i]->GetLasers()[j]->GetY(),
						  (int)m_Ships[i]->GetLasers()[j]->GetWidth(), 
						  (int)m_Ships[i]->GetLasers()[j]->GetHeight()))
			{
				// Lower Health
				Player->SetHealth(Player->GetHealth() - m_Ships[i]->GetLasers()[j]->GetDamage());
				
				// Kill the laser
				m_Ships[i]->GetLasers()[j]->SetDead(true);

				// Create an explosion
				EM->AddExplosion(m_Ships[i]->GetLasers()[j]->GetX(), m_Ships[i]->GetLasers()[j]->GetY(), 
								 Player->GetVelocity(), Player->GetAngle(),
								 1.0f);
			}
			
			// If the laser goes offscreen, delete it
			else if ((m_Ships[i]->GetLasers()[j]->GetX()-ScrollX < 0)	||
					 (m_Ships[i]->GetLasers()[j]->GetX()-ScrollX > 800) || 
					 (m_Ships[i]->GetLasers()[j]->GetY()-ScrollY < 0)	||
					 (m_Ships[i]->GetLasers()[j]->GetY()-ScrollY > 600))
			{
				// kill the laser
				m_Ships[i]->GetLasers()[j]->SetDead(true);
			}
		}

		// Collisions with asteroids and ships
		for (unsigned int n=0; n<AM->GetAsteroids().size(); n++)
		{
			if (Collision((int)m_Ships[i]->GetX(), (int)m_Ships[i]->GetY(), 
						  (int)m_Ships[i]->GetWidth(), (int)m_Ships[i]->GetHeight(),
						  (int)AM->GetAsteroids()[n]->GetX(), (int)AM->GetAsteroids()[n]->GetY(),
						  (int)AM->GetAsteroids()[n]->GetWidth(), (int)AM->GetAsteroids()[n]->GetHeight()))
			{
				// Create an explosion, Mark the ship as dead
				EM->AddExplosion(m_Ships[i]->GetX(), m_Ships[i]->GetY(), m_Ships[i]->GetVelocity(), m_Ships[i]->GetAngle(),
								 3.0f);

				m_Ships[i]->SetDead(true);

				// If the asteroid is destructible, create and explosion and mark it as dead as well
				if (AM->GetAsteroids()[n]->GetDestructible())
				{
					EM->AddExplosion(AM->GetAsteroids()[n]->GetX(), AM->GetAsteroids()[n]->GetY(), 
									 AM->GetAsteroids()[n]->GetVelocity(), AM->GetAsteroids()[n]->GetAngle(),
									 3.0f);

					AM->GetAsteroids()[n]->SetDead(true);
				}
			}

			// collisions with asteroids and enemy lasers
			for (unsigned int j=0; j<m_Ships[i]->GetLasers().size(); j++)
			{
				if (Collision((int)m_Ships[i]->GetLasers()[j]->GetX(), (int)m_Ships[i]->GetLasers()[j]->GetY(), 
							  (int)m_Ships[i]->GetLasers()[j]->GetWidth(), (int)m_Ships[i]->GetLasers()[j]->GetHeight(),
							  (int)AM->GetAsteroids()[n]->GetX(), (int)AM->GetAsteroids()[n]->GetY(),
							  (int)AM->GetAsteroids()[n]->GetWidth(), (int)AM->GetAsteroids()[n]->GetHeight()))
				{
					// Create an explosion at the laser and mark the laser as dead
					EM->AddExplosion(m_Ships[i]->GetLasers()[j]->GetX()-((32.0f-m_Ships[i]->GetLasers()[j]->GetWidth())/2), 
									 m_Ships[i]->GetLasers()[j]->GetY()-((32.0f-m_Ships[i]->GetLasers()[j]->GetHeight())/2), 
									 AM->GetAsteroids()[n]->GetVelocity(), AM->GetAsteroids()[n]->GetAngle(),
									 1.0f);

					m_Ships[i]->GetLasers()[j]->SetDead(true);

					// If the asteroid is destructible, destroy it
					if (AM->GetAsteroids()[n]->GetDestructible())
					{
						EM->AddExplosion(AM->GetAsteroids()[n]->GetX()-((64.0f-AM->GetAsteroids()[n]->GetWidth())/2), 
										 AM->GetAsteroids()[n]->GetY()-((64.0f-AM->GetAsteroids()[n]->GetHeight())/2), 
										 AM->GetAsteroids()[n]->GetVelocity(), AM->GetAsteroids()[n]->GetAngle(),
										 2.0f);

						AM->GetAsteroids()[n]->SetDead(true);
					}
				}
			}
		}	
	}
}

void cEnemyManager::DrawShips(int ScrollX, int ScrollY)
{
	for (unsigned int i=0; i<m_Ships.size(); i++)
	{
		for (unsigned int j=0; j<m_Ships[i]->GetLasers().size(); j++)
			m_Ships[i]->GetLasers()[j]->Draw(ScrollX, ScrollY);
		m_Ships[i]->Draw(ScrollX, ScrollY);
	}
}

void cEnemyManager::AddDrone(float X, float Y, float Width, float Height, float Velocity, float Angle,
							 cTexture *Texture, int Health, int LaserStrength, int FireDelay)
{
	// Create a new ship
	cEnemyShip *Ship = new cDrone(X, Y, Width, Height, Velocity, Angle, Texture, 
								  Health, LaserStrength, FireDelay);

	// Add it to the vector of ships
	m_Ships.push_back(Ship);
}

void cEnemyManager::AddClaw(float X, float Y, float Width, float Height, float Velocity, float Angle,
							cTexture *Texture, int Health, int LaserStrength, int FireDelay)
{
	// Create the ship
	cEnemyShip *Ship = new cClaw(X, Y, Width, Height, Velocity, Angle, Texture, 
								 Health, LaserStrength, FireDelay);

	// Add it to the vector of ships
	m_Ships.push_back(Ship);
}

void cEnemyManager::AddViper(float X, float Y, float Width, float Height, float Velocity, float Angle,
							 cTexture *Texture, int Health, int LaserStrength, int FireDelay)
{
	// Create the ship
	cEnemyShip *Ship = new cViper(X, Y, Width, Height, Velocity, Angle, Texture, 
								 Health, LaserStrength, FireDelay);

	// Add it to the vector of ships
	m_Ships.push_back(Ship);
}

std::vector<cEnemyShip*> &cEnemyManager::GetShips()
{
	std::vector<cEnemyShip*> &Ships = m_Ships;
	return Ships;
}

// cLaser stuff
void cLaser::Update()
{
	// Update position
	SetX(GetX() + cos(-GetAngle()) * GetVelocity());
	SetY(GetY() + sin(-GetAngle()) * GetVelocity());
}

// cExplosion stuff
void cExplosionManager::ClearExplosions()
{
	for (unsigned int i=0; i<m_Explosions.size(); i++)
	{
		if (m_Explosions[i] != NULL)
		{
			delete m_Explosions[i];
			m_Explosions[i] = NULL; 
			m_Explosions.erase(m_Explosions.begin() + i);
			i--;
		}
	}

	m_Explosions.clear();
}

void cExplosionManager::AddExplosion(float X, float Y, float Velocity, float Angle, float Size)
{
	// Create a new explosion
	cExplosion *Exp = new cExplosion(X, Y, Velocity, Angle, Size);

	// Put it on the vector
	m_Explosions.push_back(Exp);
}

void cExplosionManager::UpdateExplosions()
{
	// Update position 
	for (unsigned int i=0; i<m_Explosions.size(); i++)
	{
		m_Explosions[i]->SetX(m_Explosions[i]->GetX() + cos(-m_Explosions[i]->GetAngle()) * m_Explosions[i]->GetVelocity());
		m_Explosions[i]->SetY(m_Explosions[i]->GetY() + sin(-m_Explosions[i]->GetAngle()) * m_Explosions[i]->GetVelocity());
	}
	
	// Update animation timer
	m_AnimTimer++;
	if (m_AnimTimer > 5)
	{
		m_AnimTimer = 0;
		for (unsigned int i=0; i<m_Explosions.size(); i++)
		{
			m_Explosions[i]->SetFrame(m_Explosions[i]->GetFrame() + 1);
			if (m_Explosions[i]->GetFrame() > 18)
			{
				delete m_Explosions[i];
				m_Explosions[i] = NULL;
				m_Explosions.erase(m_Explosions.begin() + i);
				i--;
			}
		}
	}
}

void cExplosionManager::DrawExplosions(int ScrollX, int ScrollY)
{
	for (unsigned int i=0; i<m_Explosions.size(); i++)
	{
		m_TE->DrawTile(2, m_Explosions[i]->GetFrame(), (short)m_Explosions[i]->GetX()-ScrollX, 
					   (short)m_Explosions[i]->GetY()-ScrollY,
					   m_Explosions[i]->GetSize(), m_Explosions[i]->GetSize());
	}
}

// Asteroids stuff
void cAsteroid::Update()
{
	SetX(GetX() + cos(-GetAngle()*GetVelocity()));
	SetY(GetY() + sin(-GetAngle()*GetVelocity()));

	if (GetX() > 8000.0f)
		SetX(0.0f);
	else if (GetX() < -BIG_ASTEROID_WIDTH)
		SetX(8000.0f);
	
	if (GetY() > 6000.0f)
		SetY(0.0f);
	else if (GetY() < -BIG_ASTEROID_HEIGHT)
		SetY(6000.0f);
}

void cAsteroidManager::ClearAsteroids()
{
	for (unsigned int i=0; i<m_Asteroids.size(); i++)
	{
		if (m_Asteroids[i] != NULL)
		{
			delete m_Asteroids[i];
			m_Asteroids[i] = NULL;
			m_Asteroids.erase(m_Asteroids.begin() + i);
			i--;
		}
	}

	m_Asteroids.clear();
}

void cAsteroidManager::UpdateAsteroids()
{
	for (unsigned int i=0; i<m_Asteroids.size(); i++)
		m_Asteroids[i]->Update();
}

void cAsteroidManager::DrawAsteroids(int ScrollX, int ScrollY)
{
	for (unsigned int i=0; i<m_Asteroids.size(); i++)	
		m_Asteroids[i]->Draw(ScrollX, ScrollY);
}

void cAsteroidManager::AddAsteroid(bool Destructible, float X, float Y, float Width, float Height, 
								   float Velocity, float Angle, cTexture *Texture)
{
	// Create a new asteroid
	cAsteroid *Asteroid = new cAsteroid(Destructible, X, Y, Width, Height, Velocity, Angle, Texture);

	// Put it in the vector
	m_Asteroids.push_back(Asteroid);
}

std::vector<cAsteroid*> &cAsteroidManager::GetAsteroids()
{
	std::vector<cAsteroid*> &Asteroids = m_Asteroids;
	return m_Asteroids;
}

// Powerups stuff
void cPowerupManager::ClearPowerups()
{
	// Delete all the powerups
	for (unsigned int i=0; i<m_Powerups.size(); i++)
	{
		if (m_Powerups[i] != NULL)
		{
			delete m_Powerups[i];
			m_Powerups[i] = NULL;
			m_Powerups.erase(m_Powerups.begin() + i);
			i--;
		}
	}

	// Clear the vector
	m_Powerups.clear();
}

void cPowerupManager::UpdatePowerups()
{
	for (unsigned int i=0; i<m_Powerups.size(); i++)
	{
		m_Powerups[i]->Update();
		if (m_Powerups[i]->GetLife() > m_Powerups[i]->GetLifespan())
		{
			delete m_Powerups[i];
			m_Powerups[i] = NULL;
			m_Powerups.erase(m_Powerups.begin() + i);
			i--;
		}
	}
}

void cPowerupManager::DrawPowerups(int ScrollX, int ScrollY)
{
	for (unsigned int i=0; i<m_Powerups.size(); i++)
		m_Powerups[i]->GetTexture()->Render(m_Powerups[i]->GetX()-ScrollX, m_Powerups[i]->GetY()-ScrollY);
}

void cPowerupManager::AddPowerup(float X, float Y, int Life, int Lifespan, 
								 cTexture *Texture, POWERUP_TYPE Type)
{
	// Create a new powerup
	cPowerup *Powerup = new cPowerup(X, Y, Life, Lifespan, Texture, Type);

	// Put it in the vector
	m_Powerups.push_back(Powerup);
}

std::vector<cPowerup*> &cPowerupManager::GetPowerups()
{
	std::vector<cPowerup*> &Powerups = m_Powerups;
	return Powerups;
}


