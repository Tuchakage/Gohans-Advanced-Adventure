#pragma once

class CMyGame : public CGame
{
	// Define sprites and other instance variables here
	CSprite player;	// player sprite
	CSprite startScreen;
    CSprite background;
	CSpriteList platformList; // platforms and obstacles are kept in a sprite list
	CSpriteList shotList;
	CSpriteList enemyList;
	CSpriteList rockList;
	CSprite complete;
	CSprite complete1;
	CSprite lost;
	CSpriteList towerList;
	CSpriteList dragonballList;
	CSprite heart;
	CSprite heart2;
	CSprite heart3;
	CSprite heart4;
	CSprite heart5;
	CSpriteList cellList;
	CSpriteList blastList;

	// Sounds
	CSoundPlayer shotSound;
	CSoundPlayer crumblingSound;
	CSoundPlayer backgroundMusic;
	CSoundPlayer jumpSound;
	CSoundPlayer menuMusic;
	CSoundPlayer bossMusic;

	//Game Variables
	int level;
	int counter;
	int playerhealth;
	int currenthealth;
	int ecounter;
	int enemyhealth;
	int ecurrenthealth;
	int rockhealth;
	int rockcurrent;
	int towerhealth;
	int tcurrent;
	bool levelcomplete;
	int score;
	bool enemyonedown;
	bool enemytwospawned;
	int cellcounter;
	double distance;
	int cellhealth;
	int cellcurrent;
	bool bossdown;
	
	


	// my own member functions
void PlayerControl();
void EnemyControl();
void ScoreSystem();
void CellControl();


void SetupLevel1();
void SetupLevel2();
void SetupLevel3();

public:
	CMyGame(void);
	~CMyGame(void);

	// Per-Frame Callback Funtions (must be implemented!)
	virtual void OnUpdate();
	virtual void OnDraw(CGraphics* g);

	// Game Life Cycle
	virtual void OnInitialize();
	virtual void OnDisplayMenu();
	virtual void OnStartGame();
	virtual void OnStartLevel(Sint16 nLevel);
	virtual void OnGameOver();
	virtual void OnTerminate();

	// Keyboard Event Handlers
	virtual void OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode);
	virtual void OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode);

	// Mouse Events Handlers
	virtual void OnMouseMove(Uint16 x,Uint16 y,Sint16 relx,Sint16 rely,bool bLeft,bool bRight,bool bMiddle);
	virtual void OnLButtonDown(Uint16 x,Uint16 y);
	virtual void OnLButtonUp(Uint16 x,Uint16 y);
	virtual void OnRButtonDown(Uint16 x,Uint16 y);
	virtual void OnRButtonUp(Uint16 x,Uint16 y);
	virtual void OnMButtonDown(Uint16 x,Uint16 y);
	virtual void OnMButtonUp(Uint16 x,Uint16 y);
};
