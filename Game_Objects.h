/////////////////////////////////////////////////
// Game_Objects.h
// Header file for game objects
/////////////////////////////////////////////////

// Laser level enumeration
enum LASER_LEVEL 
{
	WEAK=0,
	NORMAL=1,
	STRONG=2
};

// Enumeration for powerup type
enum POWERUP_TYPE
{
	LASER,
	HEALTH,
	LIFE,
	ROCKET,
	BOMB,
	EMPTY
};

// Forward Decs
class cMovingObject;
class cLaser;
class cExplosionManager;
class cAsteroidManager;

// Collision function prototype
extern BOOL Collision(int X1, int Y1, int W1, int H1, int X2, int Y2, int W2, int H2);

// Earth menu cursor class
class cEarth
{
private:
	// Data members
	cTile *m_TE;		// Parent tile engine
	int m_Frame;		// Current frame
	int m_AnimCount;	// Animation timing counter

public:
	// Constructor/Destructor
	cEarth(cTile *TE=NULL, int Frame=0){m_TE = TE; m_Frame = Frame; m_AnimCount = 0;}
	~cEarth(){}

	// Member methods
	void Draw(int MenuChoice);

	// Accessors
	void SetTE(cTile *TE){m_TE = TE;}
	void SetFrame(int Frame){m_Frame = Frame;}

	cTile *GetTE()const{return m_TE;}
	int GetFrame()const{return m_Frame;}
};

/////////////////////////////////////////////////
// Begin classes for in-game objects
/////////////////////////////////////////////////
// cMessage Class
class cMessage
{
private:
	// Data members
	cFont *m_Font;
	std::string m_Text;
	int m_X, m_Y;
	int m_Life, m_LifeSpan;	// Life and Lifespan in frames
	D3DCOLOR m_Color;

public:
	// Constructor/Destructor
	cMessage(cFont *Font=NULL, std::string Text="Text", int X=0, int Y=0, 
			 int Life=0, int LifeSpan=1, D3DCOLOR Color=D3DCOLOR_RGBA(255,255,255,255)):m_Font(Font),
																						m_Text(Text),
																						m_X(X), m_Y(Y),
																						m_Life(Life),
																						m_LifeSpan(LifeSpan),
																						m_Color(Color)
																						{}
	~cMessage(){}

	// Member methods
	void Run();			// Updates life counter, if it is alive, draw it

	// Accessor methods
	cFont *GetFont()const{return m_Font;}
	std::string GetText()const{return m_Text;}
	int GetX()const{return m_X;}
	int GetY()const{return m_Y;}
	int GetLife()const{return m_Life;}
	int GetLifeSpan()const{return m_LifeSpan;}
	D3DCOLOR GetColor()const{return m_Color;}

	void SetFont(cFont *Font){m_Font = Font;}
	void SetText(std::string Text){m_Text = Text;}
	void SetX(int X){m_X = X;}
	void SetY(int Y){m_Y = Y;}
	void SetLife(int Life){m_Life = Life;}
	void SetLifeSpan(int LifeSpan){m_LifeSpan = LifeSpan;}
	void SetColor(D3DCOLOR Color){m_Color = Color;}
};

// cMessageManager class
// class that manages all the messages that need to be shown
class cMessageManager
{
private:
	// Data members
	std::vector<cMessage*> m_MsgList; // vector of messages

public:
	// Constructor/Destructor
	cMessageManager(){}
	~cMessageManager(){}

	// Member Methods
	void AddMsg(cFont *Font=NULL, std::string Text="Text", int X=0, int Y=0, 
				int Life=0, int LifeSpan=1, D3DCOLOR Color=D3DCOLOR_RGBA(255,255,255,255));
	void ClearMsgs();
	void RunMsgs();

	// Accessor Methods
	std::vector<cMessage*> GetMsgList()const{return m_MsgList;}

	void SetMsgList(std::vector<cMessage*> MsgList){m_MsgList = MsgList;}
};

// cMovingObject class
// Basic class for all moving objects in the game
class cMovingObject
{
private:
	// Data members
	float m_X, m_Y;
	float m_Width, m_Height;
	float m_Velocity;
	float m_Angle;
	bool m_Dead;
	cTexture *m_Texture;

public:
	// Constructor/Copy Constructor/Overloaded = operator/Destructor
	cMovingObject(float X=0.0f, float Y=0.0f, float Width=0.0f, float Height=0.0f,
				  float Velocity=0.0f, float Angle=0.0f, cTexture *Texture=NULL):m_X(X), m_Y(Y),
																			     m_Width(Width), m_Height(Height),
																				 m_Velocity(Velocity),
																				 m_Angle(Angle),
																				 m_Texture(Texture){m_Dead=false;}
	cMovingObject(cMovingObject &rhs);
	cMovingObject &operator=(const cMovingObject &rhs);
	~cMovingObject(){}

	// Member Method
	void Draw(int ScrollX, int ScrollY);

	// Accessor methods
	float GetX()const{return m_X;}
	float GetY()const{return m_Y;}
	float GetWidth()const{return m_Width;}
	float GetHeight()const{return m_Height;}
	float GetVelocity()const{return m_Velocity;}
	float GetAngle()const{return m_Angle;}
	bool GetDead()const{return m_Dead;}
	cTexture *GetTexture()const{return m_Texture;}

	void SetX(float X){m_X = X;}
	void SetY(float Y){m_Y = Y;}
	void SetWidth(float Width){m_Width = Width;}
	void SetHeight(float Height){m_Height = Height;}
	void SetVelocity(float Velocity){m_Velocity = Velocity;}
	void SetAngle(float Angle);
	void SetDead(bool Dead){m_Dead = Dead;}
	void SetTexture(cTexture *Texture){m_Texture = Texture;}
};

// cShip class
// Base class for all ships in the game
class cShip : public cMovingObject
{
private:
	// Data members
	int m_Health;			// Health
	int m_LaserStrength;	// Damage that lasers do

public:
	// Constructor/Copy Constructor/Overloaded = op/Destructor
	cShip(float X=0.0f, float Y=0.0f, float Width=0.0f, float Height=0.0f, 
		  float Velocity=0.0f, float Angle=0.0f, cTexture *Texture=NULL, 
		  int Health=0, int LaserStrength=0):cMovingObject(X, Y, Width, Height, Velocity, Angle, Texture),
											 m_Health(Health),
											 m_LaserStrength(LaserStrength){}
	cShip(cShip &rhs):cMovingObject(rhs){}
	cShip &operator=(const cShip &rhs){cMovingObject::operator=(rhs);}
	~cShip(){}

	// Accessor methods
	int GetHealth()const{return m_Health;}
	int GetLaserStrength()const{return m_LaserStrength;}

	void SetHealth(int Health){m_Health = Health;}
	void SetLaserStrength(int LaserStrength){m_LaserStrength = LaserStrength;}
};

// cPlayerShip class
// Player's ship class, very specific
class cPlayerShip : public cShip
{
private:
	// Data Members
	int m_Lives;					// Number of Lives
	int m_Rockets, m_Bombs;			// Number of Rockets and Bombs
	int m_Score;					// Player's score
	std::vector<cLaser*> m_Lasers;	// Vector of Lasers
	LASER_LEVEL m_LaserLevel;		// Level of laser power
	

public:
	// Constructor/Destructor
	cPlayerShip(float X=0.0f, float Y=0.0f, float Width=0.0f, float Height=0.0f,
				float Velocity=0.0f, float Angle=0.0f, cTexture *Texture=NULL,
				int Health=0, int LaserStrength=0,
				int Lives=0, int Rockets=0, int Bombs=0):cShip(X, Y, Width, Height, Velocity, Angle, Texture,
														  Health, LaserStrength),
														  m_Lives(Lives), m_Rockets(Rockets), m_Bombs(Bombs)
														  {
															  ClearLasers();
															  m_LaserLevel = WEAK;
															  m_Score = 0;
														  }
												
	~cPlayerShip(){ClearLasers();}

	// Member methods
	void Update(int ScrollX, int ScrollY, cExplosionManager *EM);
	void ShootLaser(cTexture *WeakTex, cTexture *NormTex, cTexture *StrongTex);
	void ShootRocket(cTexture *RocketTex);
	void DropBomb(cTexture *BombTex);
	void ClearLasers();

	// Accessor methods
	int GetLives()const{return m_Lives;}
	int GetRockets()const{return m_Rockets;}
	int GetBombs()const{return m_Bombs;}
	int GetScore()const{return m_Score;}
	std::vector<cLaser*> &GetLasers();
	LASER_LEVEL GetLaserLevel()const{return m_LaserLevel;}

	void SetLives(int Lives){m_Lives = Lives;}
	void SetRockets(int Rockets){m_Rockets = Rockets;}
	void SetBombs(int Bombs){m_Bombs = Bombs;}
	void SetScore(int Score){m_Score = Score;}
	void SetLaserLevel(LASER_LEVEL LaserLevel){m_LaserLevel = LaserLevel;}
};

// cEnemyShip class
// Base class for enemy ships
class cEnemyShip : public cShip
{
private:
	// Data members
	std::vector<cLaser*> m_Lasers;
	int m_FireDelay, m_FireCount;

public:
	// Constructor/Destructor
	cEnemyShip(float X=0.0f, float Y=0.0f, float Width=0.0f, float Height=0.0f, 
			   float Velocity=0.0f, float Angle=0.0f, cTexture *Texture=NULL, 
			   int Health=0, int LaserStrength=0, int FireDelay=0):cShip(X, Y, Width, Height, Velocity, 
																         Angle, Texture, Health, 
																		 LaserStrength),
																		 m_FireDelay(FireDelay),
																		 m_FireCount(0)
																		 {ClearLasers();}
	cEnemyShip(cEnemyShip &rhs);
	cEnemyShip &operator=(cEnemyShip &rhs);
	virtual ~cEnemyShip(){ClearLasers();}

	// Member Methods
	virtual void UpdateAI(float PlayerX, float PlayerY, cTexture *LaserTex){}
	virtual void ShootLaser(cTexture *Texture){}
	void UpdateLasers();
	void ClearLasers();

	// Accessor Methods
	std::vector<cLaser*> &GetLasers();
	int GetFireCount()const{return m_FireCount;}
	int GetFireDelay()const{return m_FireDelay;}

	void SetFireCount(int FireCount){m_FireCount = FireCount;}
	void SetFireDelay(int FireDelay){m_FireDelay = FireDelay;}
};

// Begin specific enemy ship classes
class cDrone : public cEnemyShip
{
private:
	// No new data members

public:
	// Constructor/Copy Constructor/Destructor
	cDrone(float X=0.0f, float Y=0.0f, float Width=0.0f, float Height=0.0f, 
		   float Velocity=0.0f, float Angle=0.0f, cTexture *Texture=NULL, 
		   int Health=0, int LaserStrength=0, int FireDelay=0):cEnemyShip(X, Y, Width, Height, Velocity, 
																		  Angle, Texture, Health, 
																		  LaserStrength, FireDelay){}
	cDrone(cDrone &rhs):cEnemyShip(rhs){}
	cDrone &operator=(cDrone &rhs){cEnemyShip::operator=(rhs);}
	virtual ~cDrone(){}

	// Member methods
	virtual void UpdateAI(float PlayerX, float PlayerY, cTexture *LaserTex);
	virtual void ShootLaser(cTexture *Texture);
};

class cClaw : public cEnemyShip
{
private:
	// No new data members

public:
	// Constructor/Copy Constructor/Destructor
	cClaw(float X=0.0f, float Y=0.0f, float Width=0.0f, float Height=0.0f, 
		  float Velocity=0.0f, float Angle=0.0f, cTexture *Texture=NULL, 
		  int Health=0, int LaserStrength=0, int FireDelay=0):cEnemyShip(X, Y, Width, Height, Velocity, 
																		  Angle, Texture, Health, 
																		  LaserStrength, FireDelay){}
	cClaw(cClaw &rhs):cEnemyShip(rhs){}
	cClaw &operator=(cClaw &rhs){cEnemyShip::operator=(rhs);}
	virtual ~cClaw(){}

	// Member methods
	virtual void UpdateAI(float PlayerX, float PlayerY, cTexture *LaserTex);
	virtual void ShootLaser(cTexture *Texture);
};

class cViper : public cEnemyShip
{
private:
	// No new data members

public:
	// Constructor/Copy Constructor/Destructor
	cViper(float X=0.0f, float Y=0.0f, float Width=0.0f, float Height=0.0f, 
		  float Velocity=0.0f, float Angle=0.0f, cTexture *Texture=NULL, 
		  int Health=0, int LaserStrength=0, int FireDelay=0):cEnemyShip(X, Y, Width, Height, Velocity, 
																		 Angle, Texture, Health, 
																		 LaserStrength, FireDelay){}
	cViper(cViper &rhs):cEnemyShip(rhs){}
	cViper &operator=(cViper &rhs){cEnemyShip::operator=(rhs);}
	virtual ~cViper(){}

	// Member methods
	virtual void UpdateAI(float PlayerX, float PlayerY, cTexture *LaserTex);
	virtual void ShootLaser(cTexture *Texture);
};

// cEnemyManager Class
// class that holds a vector of all enemy ships and updates everything about them
class cEnemyManager
{
private:
	// Data Members
	std::vector<cEnemyShip*> m_Ships;

public:
	// Constructor/Destructor
	cEnemyManager(){ClearShips();}
	~cEnemyManager(){ClearShips();}

	// Member Methods
	void ClearShips();
	void UpdateShips(cPlayerShip *Player, cTexture *LaserTex, cExplosionManager *EM, cAsteroidManager *AM);
	void DrawShips(int ScrollX, int ScrollY);
	void AddDrone(float X=0.0f, float Y=0.0f, float Width=0.0f, float Height=0.0f, 
				  float Velocity=0.0f, float Angle=0.0f, cTexture *Texture=NULL, 
				  int Health=0, int LaserStrength=0, int FireDelay=0);
	void AddClaw(float X=0.0f, float Y=0.0f, float Width=0.0f, float Height=0.0f, 
				 float Velocity=0.0f, float Angle=0.0f, cTexture *Texture=NULL, 
				 int Health=0, int LaserStrength=0, int FireDelay=0);
	void AddViper(float X=0.0f, float Y=0.0f, float Width=0.0f, float Height=0.0f, 
				  float Velocity=0.0f, float Angle=0.0f, cTexture *Texture=NULL, 
				  int Health=0, int LaserStrength=0, int FireDelay=0);
	

	// Accessor Methods
	std::vector<cEnemyShip*> &GetShips();
};

// cLaser Class
// class for all lasers
class cLaser : public cMovingObject
{
private:
	// Data Members
	int m_Damage;

	
public:
	// Constructor/Destructor
	cLaser(float X=0.0f, float Y=0.0f, float Width=0.0f, float Height=0.0f, float Velocity=0.0f, 
		   float Angle=0.0f, cTexture *Texture=NULL, int Damage=0):cMovingObject(X, Y, Width, Height,
																				 Velocity, Angle, Texture),
																			     m_Damage(Damage){}
	cLaser(cLaser &rhs):cMovingObject(rhs){}
	cLaser &operator=(cLaser &rhs){cMovingObject::operator=(rhs);}
	~cLaser(){}

	// Member methods
	void Update();

	// Accessor Method
	int GetDamage()const{return m_Damage;}

	void SetDamage(int Damage){m_Damage = Damage;}
};

// cExplosion Manager stuff
class cExplosion
{
private:
	// Data members
	float m_X, m_Y;
	float m_Velocity, m_Angle;	// Velocity and angle
	float m_Size;				// Size of the explosion (the sprite is scaled by this number)
	int m_Frame;				// Current frame, all explosions have firstframe 0 and last frame 7 (8 Total)
	
public:
	// Constructor/Destructor
	cExplosion(float X=0.0f, float Y=0.0f, float Velocity=0.0f, 
			   float Angle=0.0f, float Size=1.0f):m_X(X),
				   								  m_Y(Y), 
												  m_Velocity(Velocity),
												  m_Angle(Angle),
												  m_Size(Size), 
												  m_Frame(0){}
	~cExplosion(){}

	// Member methods
	// None needed, the explosion manager will update the frame and everything and delete the 
	// Explosions when they expire

	// Accessor Methods
	float GetX()const{return m_X;}
	float GetY()const{return m_Y;}
	float GetVelocity()const{return m_Velocity;}
	float GetAngle()const{return m_Angle;}
	float GetSize()const{return m_Size;}
	int GetFrame()const{return m_Frame;}

	void SetX(float X){m_X = X;}
	void SetY(float Y){m_Y = Y;}
	void SetVelocity(float Velocity){m_Velocity = Velocity;}
	void SetAngle(float Angle){m_Angle = Angle;}
	void SetSize(float Size){m_Size = Size;}
	void SetFrame(int Frame){m_Frame = Frame;}
};

class cExplosionManager
{
private:
	// Data Members
	int m_AnimTimer;						// Timer for explosion animation
	cTile *m_TE;							// Parent Tile engine
	std::vector<cExplosion*> m_Explosions;	// Vector of explosions

public:
	// Constructor/Destructor
	cExplosionManager(cTile *TE=NULL):m_TE(TE){ClearExplosions(); m_AnimTimer=0;}
	~cExplosionManager(){ClearExplosions();}

	// Member Methods
	void ClearExplosions();
	void AddExplosion(float X=0.0f, float Y=0.0f, float Velocity=0.0f, float Angle=0.0f, float Size=1.0f);
	void UpdateExplosions();
	void DrawExplosions(int ScrollX, int ScrollY);

	// Accessor Methods
	cTile *GetTE()const{return m_TE;}
	std::vector<cExplosion*> &GetExplosions();

	void SetTE(cTile *TE){m_TE = TE;}
};

class cAsteroid : public cMovingObject
{
private:
	// Data member
	bool m_Destructible;

public:
	// Constructor/Destructor
	cAsteroid(bool Destructible=false, float X=0.0f, float Y=0.0f, float Width=0.0f, float Height=0.0f, 
			  float Velocity=0.0f, float Angle=0.0f, 
			  cTexture *Texture=NULL):cMovingObject(X, Y, Width, Height, Velocity, Angle, Texture), 
									  m_Destructible(Destructible){}
	cAsteroid(cAsteroid &rhs):cMovingObject(rhs){}
	cAsteroid &operator=(cAsteroid &rhs){cMovingObject::operator=(rhs);}
	~cAsteroid(){}

	// Member Methods
	void Update();
	
	// Accessor Method
	bool GetDestructible()const{return m_Destructible;}

	void SetDestructible(bool Destructible){m_Destructible = Destructible;}
};

class cAsteroidManager
{
private:
	// Data Members
	std::vector<cAsteroid*> m_Asteroids;

public:
	// Constructor/Destructor
	cAsteroidManager(){ClearAsteroids();}
	~cAsteroidManager(){ClearAsteroids();}

	// Member Methods
	void ClearAsteroids();
	void UpdateAsteroids();
	void DrawAsteroids(int ScrollX, int ScrollY);
	void AddAsteroid(bool Destructible=false, float X=0.0f, float Y=0.0f, float Width=0.0f, float Height=0.0f, 
					 float Velocity=0.0f, float Angle=0.0f, cTexture *Texture=NULL);

	// Accessor Method
	std::vector<cAsteroid*> &GetAsteroids();
};

// Powerup stuff
class cPowerup
{
private:
	// Data Members
	float m_X, m_Y;
	int m_Life, m_Lifespan;
	cTexture *m_Texture;
	POWERUP_TYPE m_Type;

public:
	// Constructor/Destructor
	cPowerup(float X=0.0f, float Y=0.0f, int Life=0, int Lifespan=0, 
			 cTexture *Texture=NULL, POWERUP_TYPE Type=EMPTY):m_X(X), m_Y(Y),
															 m_Life(Life), m_Lifespan(Lifespan),
															 m_Texture(Texture), m_Type(Type)
															 {}
	~cPowerup(){}
	
	// Member Method
	void Update(){m_Life++;}

	// Accessor Methods
	float GetX()const{return m_X;}
	float GetY()const{return m_Y;}
	int GetLife()const{return m_Life;}
	int GetLifespan()const{return m_Lifespan;}
	cTexture *GetTexture()const{return m_Texture;}
	POWERUP_TYPE GetType()const{return m_Type;}

	void SetX(float X){m_X = X;}
	void SetY(float Y){m_Y = Y;}
	void SetLife(int Life){m_Life = Life;}
	void SetLifespan(int Lifespan){m_Lifespan = Lifespan;}
	void SetTexture(cTexture *Texture){m_Texture = Texture;}
	void SetType(POWERUP_TYPE Type){m_Type = Type;}
};

// Powerup manager class
class cPowerupManager
{
private:
	// Data member
	std::vector<cPowerup*> m_Powerups;

public:
	// Constructor/Destructor
	cPowerupManager(){ClearPowerups();}
	~cPowerupManager(){ClearPowerups();}

	// Member Methods
	void ClearPowerups();
	void UpdatePowerups();
	void DrawPowerups(int ScrollX, int ScrollY);
	void AddPowerup(float X=0.0f, float Y=0.0f, int Life=0, int Lifespan=0, 
					cTexture *Texture=NULL, POWERUP_TYPE Type=EMPTY);

	// Accessor Method
	std::vector<cPowerup*> &GetPowerups();
};











