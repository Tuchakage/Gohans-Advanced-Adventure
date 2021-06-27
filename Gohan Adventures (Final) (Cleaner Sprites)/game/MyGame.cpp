#include "stdafx.h"
#include "MyGame.h"

CMyGame::CMyGame(void) : 
	startScreen(400,300,"startScreen.bmp",0), complete(400,300,"LevelComplete.bmp",0), 
	complete1(400, 300, "Level1Complete.bmp", 0), lost(400, 300, "LosingScreen.bmp", 0),
	heart(90, 10, "heart1.bmp", CColor::Black(), 0), heart2(115, 10, "heart1.bmp", CColor::Black(), 0),
	heart3(140, 10, "heart1.bmp", CColor::Black(), 0), heart4(165, 10, "heart1.bmp", CColor::Black(), 0),
	heart5(190, 10, "heart1.bmp", CColor::Black(), 0)
	// to initialise more sprites here use a comma-separated list
{
	// TODO: add initialisation here
	counter = 0;
	playerhealth = 50;
	currenthealth = playerhealth;
	ecounter = 0;
	enemyhealth = 20;
	ecurrenthealth = enemyhealth;
	rockhealth = 30;
	rockcurrent = rockhealth;
	towerhealth = 50;
	tcurrent = towerhealth;
	levelcomplete = false;
	level = 1;
	score = 0;
	enemyonedown = false;
	enemytwospawned = false;
	cellcounter = 0;
	cellhealth = 50;
	cellcurrent = 50;
	bossdown = false;

	for (CSprite* cell : cellList) 
	{
		distance = abs(player.GetX() - cell->GetX());

	}
	
}

CMyGame::~CMyGame(void)
{
	// TODO: add destruction code here
}

/////////////////////////////////////////////////////
// Per-Frame Callback Funtions (must be implemented!)

void CMyGame::OnUpdate()
{
	// do not run game logic when in menu mode
	if (IsMenuMode()) return;

	Uint32 t = GetTime();

	// TODO: add the game update code here
	player.Update(t);	
	

	for (CSprite* pSprite : platformList) pSprite->Update(t);
	for (CSprite* shot : shotList) shot->Update(t);
	for (CSprite* pEnemy : enemyList) pEnemy->Update(t);
	for (CSprite* pRock : rockList) pRock->Update(t);
	for (CSprite* tower : towerList) tower->Update(t);
	for (CSprite* dragonball : dragonballList) dragonball->Update(t);
	
	for (CSprite* cell : cellList) cell->Update(t);
	for (CSprite* blast : blastList) blast->Update(t);

	if (player.GetY() < 0) GameOver();

	if (player.GetX() < 0) 
	{
		player.SetVelocity(-player.GetXVelocity(), player.GetYVelocity());
	}
	PlayerControl();
	EnemyControl();
	ScoreSystem();
	CellControl();
	
	

	//RockControls
	for (CSprite* pRock : rockList) 
	{
		for (CSprite* shot : shotList) 
		{
			if (shot->HitTest(pRock, 1))
			{
				shot->Delete();
				crumblingSound.Play("crumbling.wav");
				rockcurrent = rockcurrent - 10;

				if (rockcurrent <= 0)
				{
					pRock->Delete();
				}
			}

		}

		if (pRock->HitTest(&player)) 
		{
			player.SetVelocity(-player.GetXVelocity(), player.GetYVelocity());
		}
	}

	rockList.delete_if(deleted);

	for (CSprite* tower : towerList) 
	{
		for (CSprite* shot : shotList)
		{
			if (shot->HitTest(tower))
			{
				shot->Delete();
				crumblingSound.Play("crumbling.wav");
				tcurrent = tcurrent - 10;

				if (tcurrent <= 0)
				{
					tower->Delete();
				}
			}
		}
	}
	

	towerList.delete_if(deleted);
	for (CSprite* tower : towerList) 
	{
		if (tower->HitTest(&player))
		{
			player.SetVelocity(-player.GetXVelocity(), player.GetYVelocity());
		}
	}

}


void CMyGame::OnDraw(CGraphics* g)
{
	int leftScreenLimit = 300;
	int rightScreenLimit = 1900;
	
	// draw menu when in menu mode
	if (IsMenuMode())
	{
		startScreen.Draw(g);
		return;
	}


	//Draw Level Complete 
	if (player.GetX() > 2400 && level == 1) 
	{
		complete1.Draw(g);
		levelcomplete = true;
		return;
	}
	
	//Player cant complete level 2 until they collected all the Dragon Balls
	if (player.GetX() > 2400 && level == 2 && score == 7) 
	{
		complete1.Draw(g);
		levelcomplete = true;
		return;
	}

	//Player cant get pass the point
	if (player.GetX() > 2400 && level == 2 && score <7)
	{
		player.SetVelocity(-player.GetXVelocity(), player.GetYVelocity());
	}

	//Level 3 Complete condition
	if (player.GetX() > 2400 && level == 3 && bossdown == true)
	{
		complete.Draw(g);
		levelcomplete = true;
		return;
	}

	//Player cant get pass Level 3
	if (player.GetX() > 2400 && level == 3 && bossdown == false)
	{
		player.SetVelocity(-player.GetXVelocity(), player.GetYVelocity());
	}

	// we scroll the whole game world according to the player position
	if (player.GetX() >= leftScreenLimit && player.GetX() <= rightScreenLimit)
	{
		g->SetScrollPos(leftScreenLimit - player.GetX(), 0);
	}
	// we stop scrolling with the player once we have reached the right limit
	if (player.GetX() > rightScreenLimit)
	{
		g->SetScrollPos(-rightScreenLimit+leftScreenLimit, 0);
	}

	background.Draw(g);
	
	

	for (CSprite* pSprite : platformList) pSprite->Draw(g);
	player.Draw(g);
	for (CSprite* shot : shotList) shot->Draw(g);
	for (CSprite* pEnemy : enemyList) pEnemy->Draw(g);
	for (CSprite* pRock : rockList) pRock->Draw(g);
	for (CSprite* tower : towerList) tower->Draw(g);
	for (CSprite* dragonball : dragonballList) dragonball->Draw(g);
	
	for (CSprite* cell : cellList) cell->Draw(g);
	for (CSprite* blast : blastList) blast->Draw(g);

	  
	 // don't scroll the overlay screen
	 g->SetScrollPos(0, 0);
	
	 if (currenthealth == 50) 
	 {
		 heart.Draw(g);
		 heart2.Draw(g);
		 heart3.Draw(g);
		 heart4.Draw(g);
		 heart5.Draw(g);
	 }
	 if (currenthealth == 40) 
	 {
		 heart.Draw(g);
		 heart2.Draw(g);
		 heart3.Draw(g);
		 heart4.Draw(g);
		 
	 }
	 if (currenthealth == 30)
	 {
		 heart.Draw(g);
		 heart2.Draw(g);
		 heart3.Draw(g);

	 }
	 if (currenthealth == 20)
	 {
		 heart.Draw(g);
		 heart2.Draw(g);
	 }
	 if (currenthealth == 10)
	 {
		 heart.Draw(g);
	 }
	 

	 
	 //*g << font(28) << color(CColor::Red()) << xy(10, 570) << "player status: " << level;
	 //*g << font(28) << color(CColor::Blue()) << xy(0, 0) << "player health: " << currenthealth;

	 //Game Over image apears when GameOverMode Is Enabled
	 if (IsGameOverMode()) 
	 {
		 lost.Draw(g);
		 return;
	 }

	 //Score will appear when on Level 2
	 if (level == 2)
	 {
		 *g << font(28) << color(CColor::Red()) << xy(10, 570) << "Dragon Balls: " << score;
	 }

	 if (level == 3) 
	 {
		 *g << font(28) << color(CColor::Red()) << xy(10, 570) << "Boss Health: " << cellcurrent;
	 }
	 
}

void CMyGame::PlayerControl()
{
	float jumpSpeed = 550; // jumpin up speed
	float slowDown = 15; // speed reduction when jumping
	float walkSpeed = 225; // horizontal walking speed
	
	
	CVector oldPos = player.GetPosition();
	
	enum { NONE, STANDLEFT, STANDRIGHT, WALKLEFT, WALKRIGHT, JUMPLEFT, JUMPRIGHT } p_state;
	/* 
	  player status variable is used to indicate which state the player is in

	  1: standing left looking
	  2: standing right looking
	  3: walking left
	  4: walking right
	  5: jumping/falling left
	  6: jumping/falling right

	*/


	// gravity simulation 
	if (player.GetStatus() > 4) player.SetYVelocity(player.GetYVelocity() - slowDown);

	// walking RIGHT
	if (IsKeyDown(SDLK_RIGHT) && player.GetStatus() != WALKRIGHT && player.GetStatus() < WALKRIGHT)
	{
		player.SetMotion( walkSpeed, 0);
		player.SetAnimation("walkright", 10);
		player.SetStatus( WALKRIGHT);
		player.Update(GetTime());
	}
	// standing right
	else if(!IsKeyDown(SDLK_RIGHT) && player.GetStatus() == WALKRIGHT)
	{
		player.SetMotion(0, 0);
		player.SetImage("standright");
		player.SetStatus(STANDRIGHT);
		player.Update(GetTime());
	}
	// walking LEFT
	if (IsKeyDown(SDLK_LEFT) && player.GetStatus() != WALKLEFT && player.GetStatus() < WALKLEFT)
	{
		player.SetMotion(-walkSpeed, 0);
		player.SetAnimation("walkleft", 10);
		player.SetStatus(WALKLEFT);
		player.Update(GetTime());
	}
	// standing left
	else if (!IsKeyDown(SDLK_LEFT) && player.GetStatus() == WALKLEFT)
	{
		player.SetMotion(0, 0);
		player.SetImage("standleft");
		player.SetStatus(STANDLEFT);
		player.Update(GetTime());
	}

	// JUMPING
	if (IsKeyDown(SDLK_SPACE) && player.GetStatus() != JUMPLEFT && player.GetStatus() != JUMPRIGHT)
	{
		if (player.GetStatus() == STANDLEFT || player.GetStatus() == WALKLEFT)
		{
		 player.SetYVelocity( jumpSpeed);
		 player.SetImage("jumpleft");
		 player.SetStatus(JUMPLEFT);
		 player.Update(GetTime());
		 jumpSound.Play("jump.wav");
		}
		if (player.GetStatus() == STANDRIGHT || player.GetStatus() == WALKRIGHT)
		{
			player.SetYVelocity( jumpSpeed);
			player.SetImage("jumpright");
			player.SetStatus(JUMPRIGHT);
			player.Update(GetTime());
			jumpSound.Play("jump.wav");
		}

		
	}

	//Game Over when Health is 0
	if (currenthealth <= 0)
	{
		GameOver();
	}
	//Attack & Shooting
	if (IsKeyDown(SDLK_LCTRL) && player.GetStatus() == STANDRIGHT && counter == 0)
	{
		player.SetAnimation("attackright");
		counter = 30;
		player.Update(GetTime());
	
	} 
	else if (IsKeyDown(SDLK_LCTRL) && player.GetStatus() == STANDLEFT && counter == 0) 
	{
		player.SetAnimation("attackleft");
		counter = 30;
		player.Update(GetTime());
	}
	
	
	// If the Counter is more than 0 (When we attack) The timer will go down and once the timer hits 1 we set the image to Stand right
	if (counter > 0)
	{
		counter--;
		if (counter == 1 && player.GetStatus() == STANDRIGHT)
		{

			player.SetImage("standright");
		}

		if (counter == 1 && player.GetStatus() == STANDLEFT)
		{

			player.SetImage("standleft");
		}


		//When the counter hits 14 do the Kamehameha
		if (counter == 14 && player.GetStatus() == STANDRIGHT)
		{
			CSprite* newShot = new CSprite(player.GetX() + 30, player.GetY(), "blast.bmp", CColor::Blue(), GetTime());
			newShot->SetMotion(300, 0);
			newShot->Die(300);
			shotList.push_back(newShot);		
			shotSound.Play("Blast1.wav");
		}

		if (counter == 14 && player.GetStatus() == STANDLEFT)
		{
			CSprite* newShot = new CSprite(player.GetX() - 30, player.GetY(), "blastl.bmp", CColor::Blue(), GetTime());
			newShot->SetMotion(-400, 0);
			newShot->Die(300);
			shotList.push_back(newShot);
			shotSound.Play("Blast1.wav");
		}
	}
	
	shotList.delete_if(deleted);


	// ----- testing if we are on a platform -------
	bool onPlatform = false;
	player.SetY(player.GetY() - 5); // move a little bit down
	for (CSprite* pPlatform : platformList)
	{
		if (player.HitTest(pPlatform))
		{
			onPlatform = true;
		}
	}
	player.SetY(player.GetY() + 5);

	// ---- collision with platforms
	player.Proceed(5);
	for (CSprite* pPlatform : platformList)
	{
		if (player.HitTest(pPlatform))
		{
			// where did we hit the platform
			 // below platform
			if (player.GetY() < pPlatform->GetBottom())
			{
				player.SetY(player.GetY() - 5);
				player.SetYVelocity(0);
			}
			// above platform
			else if (player.GetBottom() >= pPlatform->GetBottom())
			{
				onPlatform = true;
				if (player.GetStatus() == JUMPLEFT)
				{
					player.SetMotion(0, 0);
					player.SetImage("standleft");
					player.SetStatus(STANDLEFT);
					player.Update(GetTime());
					player.SetY(pPlatform->GetTop() + player.GetHeight() / 2);
				}
				if (player.GetStatus() == JUMPRIGHT)
				{
					player.SetMotion(0, 0);
					player.SetImage("standright");
					player.SetStatus(STANDRIGHT);
					player.Update(GetTime());
					player.SetY(pPlatform->GetTop() + player.GetHeight() / 2);
				}
			}
	        // left of platform;
			else if (player.GetX() < pPlatform->GetLeft())
			{
				player.SetXVelocity(0);
				player.SetYVelocity(0);
				player.SetPosition(oldPos);

			}
			// right of platform
			else if (player.GetX() > pPlatform->GetRight())
			{
				player.SetXVelocity(0);
				player.SetYVelocity(0);
				player.SetPosition(oldPos);

			}

		}
	}
	player.Proceed(-5);

	// if we are not on a platform then we should fall
	if (!onPlatform && (player.GetStatus() == STANDLEFT || player.GetStatus() == WALKLEFT)) // no ground
	{
		player.SetStatus(JUMPLEFT); // make fall

	}
	// if we are not on a platform then we should fall
	if (!onPlatform && (player.GetStatus() == STANDRIGHT || player.GetStatus() == WALKRIGHT)) // no ground
	{
		player.SetStatus(JUMPRIGHT); // make fall
	}
	
	player.Update(GetTime());

}

void CMyGame::EnemyControl()
{
	float slowDown = 15; // speed reduction when jumping```	
	 // horizontal walking speed
	float ewalkSpeed = 125;

	enum { NONE, ESTANDLEFT, ESTANDRIGHT, EWALKLEFT, EWALKRIGHT, EJUMPLEFT, EJUMPRIGHT, EATTACKLEFT, EATTACKRIGHT } p_state;

	for (CSprite* pEnemy : enemyList)
	{
		for (CSprite* shot : shotList) 
		{
			if (shot->HitTest(pEnemy, 1))
			{
				shot->Delete();
				ecurrenthealth = ecurrenthealth - 10;

				if (ecurrenthealth <= 0 && enemytwospawned == false)
				{
					enemyonedown = true;
					pEnemy->Delete();
				}

				if (ecurrenthealth <= 0 && enemytwospawned == true)
				{
					pEnemy->Delete();
				}
			}

		}

		//Spawn a second enemy in
		if (level == 2 && enemyonedown == true)
		{
			ecurrenthealth = 20;
			CSprite* pBear = new CSprite(900, 350, "bearL.bmp", CColor::Blue(), GetTime());
			pBear->LoadAnimation("bearLeft.bmp", "walkleft", CSprite::Sheet(4, 1).Row(0).From(0).To(3), CColor::Blue());
			pBear->SetAnimation("walkleft", 10);
			pBear->LoadAnimation("bearattackL.bmp", "attackleft", CSprite::Sheet(2, 1).Row(0).From(0).To(1), CColor::Blue());
			pBear->LoadAnimation("bearattackR.bmp", "attackright", CSprite::Sheet(2, 1).Row(0).From(0).To(1), CColor::Blue());
			pBear->SetMotion(-125, 0);

			enemyList.push_back(pBear);
			enemyonedown = false;
			enemytwospawned = true;


		}
			
		if (pEnemy->GetX() < 300 && pEnemy->GetXVelocity() < 0 && pEnemy->GetStatus() != EATTACKRIGHT && enemyonedown == false && enemytwospawned == false)
		{
			pEnemy->SetMotion(ewalkSpeed, 0);
			pEnemy->LoadAnimation("bearRight.bmp", "walkright", CSprite::Sheet(4, 1).Row(0).From(0).To(3), CColor::Blue());
			pEnemy->SetAnimation("walkright", 10);
			pEnemy->SetStatus(EWALKRIGHT);
			pEnemy->Update(GetTime());

		}

		if (pEnemy->GetX() > 500 && pEnemy->GetXVelocity() > 0 && pEnemy->GetStatus() != EATTACKLEFT && enemyonedown == false && enemytwospawned == false)
		{
			pEnemy->SetMotion(-ewalkSpeed, 0);
			pEnemy->LoadAnimation("bearLeft.bmp", "walkleft", CSprite::Sheet(4, 1).Row(0).From(0).To(3), CColor::Blue());
			pEnemy->SetAnimation("walkleft", 10);
			pEnemy->SetStatus(EWALKLEFT);
		}

		//Movement for pBear (Enemy 2)

		if (pEnemy->GetX() < 800 && pEnemy->GetXVelocity() < 0 && pEnemy->GetStatus() != EATTACKRIGHT && enemytwospawned == true)
		{
			pEnemy->SetMotion(ewalkSpeed, 0);
			pEnemy->LoadAnimation("bearRight.bmp", "walkright", CSprite::Sheet(4, 1).Row(0).From(0).To(3), CColor::Blue());
			pEnemy->SetAnimation("walkright", 10);
			pEnemy->SetStatus(EWALKRIGHT);
			pEnemy->Update(GetTime());

		}

		if (pEnemy->GetX() > 1000 && pEnemy->GetXVelocity() > 0 && pEnemy->GetStatus() != EATTACKLEFT && enemytwospawned == true)
		{
			pEnemy->SetMotion(-ewalkSpeed, 0);
			pEnemy->LoadAnimation("bearLeft.bmp", "walkleft", CSprite::Sheet(4, 1).Row(0).From(0).To(3), CColor::Blue());
			pEnemy->SetAnimation("walkleft", 10);
			pEnemy->SetStatus(EWALKLEFT);
		}

		//if (pEnemy->GetStatus() > 4) pEnemy->SetYVelocity(pEnemy->GetYVelocity() - slowDown);

		//Hit Detection for Left Side
		if (pEnemy->HitTest(&player, 1) && pEnemy->GetStatus() == EWALKLEFT)
		{
			pEnemy->SetMotion(0, 0);
			player.SetVelocity(-player.GetXVelocity(), player.GetYVelocity());
			pEnemy->LoadImage("bearL.bmp", "standleft", 3, 1, 0, 0, CColor::Blue());
			pEnemy->SetImage("standleft");
			pEnemy->SetStatus(ESTANDLEFT);
			ecounter = 30;	
		}
		//Hit Detection For Right Side
		else if (pEnemy->HitTest(&player, 1) && pEnemy->GetStatus() == EWALKRIGHT)
		{
			pEnemy->SetMotion(0, 0);
			player.SetVelocity(-player.GetXVelocity(), player.GetYVelocity());
			pEnemy->LoadImage("bearR.bmp", "standright", 3, 1, 0, 0, CColor::Blue());
			pEnemy->SetImage("standright");
			pEnemy->SetStatus(ESTANDRIGHT);
			ecounter = 30;
		}

		if (ecounter > 0)
		{

			ecounter--;
			if (ecounter == 14 && pEnemy->GetStatus() == ESTANDLEFT)
			{
			pEnemy->SetAnimation("attackleft", 5);
			pEnemy->Update(GetTime());
			currenthealth = currenthealth - 10;
			player.SetPosition(player.GetX() - 50, player.GetY());
			pEnemy->SetStatus(EATTACKLEFT);
			}

			else if (ecounter == 14 && pEnemy->GetStatus() == ESTANDRIGHT)
			{
				pEnemy->SetAnimation("attackright", 5);
				pEnemy->Update(GetTime());
				currenthealth = currenthealth - 10;
				player.SetPosition(player.GetX() + 50, player.GetY());
				pEnemy->SetStatus(EATTACKRIGHT);
			}


			

			if (ecounter <= 0 && !pEnemy->HitTest(&player) && pEnemy->GetStatus() == EATTACKLEFT) 
			{
				pEnemy->SetMotion(-ewalkSpeed, 0);
				pEnemy->SetAnimation("walkleft", 10);
				pEnemy->SetStatus(EWALKLEFT);
			}

			else if (ecounter <= 0 && !pEnemy->HitTest(&player) && pEnemy->GetStatus() == EATTACKRIGHT)
			{
				pEnemy->SetMotion(ewalkSpeed, 0);
				pEnemy->SetAnimation("walkright", 10);
				pEnemy->SetStatus(EWALKRIGHT);
			}


		}
	}	

	enemyList.delete_if(deleted);
	shotList.delete_if(deleted);
}

void CMyGame:: ScoreSystem()
{
	for (CSprite* dragonball : dragonballList) 
	{
		if (dragonball->HitTest(&player, 1)) 
		{
			dragonball->Delete();
			score++;
		}
	}
	dragonballList.delete_if(deleted);
}

void CMyGame::CellControl() 
{
	enum { NONE, CSTANDLEFT, CSTANDRIGHT, CWALKLEFT, CWALKRIGHT, CJUMPLEFT, CJUMPRIGHT, CATTACKLEFT, CATTACKRIGHT } p_state;

	for (CSprite* cell : cellList) 
	{

		for (CSprite* shot : shotList) 
		{
			if (shot->HitTest(cell, 1))
			{
				shot->Delete();
				cellcurrent = cellcurrent - 10;

				if (cellcurrent <= 0) 
				{
					cell->Delete();
					bossdown = true;
				}
			}

		}
		if (cell->GetX() < 1500 && cell->GetStatus() != CATTACKLEFT) 
		{
			cell->SetMotion(150, 0);
			cell->LoadImage("cellrunr.bmp", "crunright", 4, 1, 2, 0, CColor::Blue());
			cell->SetImage("crunright");
			cell->SetStatus(CWALKRIGHT);
		}

		else if (cell->GetX() > 2100 && cell->GetStatus() != CATTACKRIGHT) 
		{
			cell->SetMotion(-150, 0);
			cell->SetImage("crunleft");
			cell->SetStatus(CWALKLEFT);
		}
		
		
		if (cell->HitTest(&player, 1) && cell->GetStatus() == CWALKLEFT)
		{
			cell->SetMotion(0, 0);
			player.SetVelocity(-player.GetXVelocity(), player.GetYVelocity());
			cell->LoadAnimation("cellattackl.bmp", "cattackleft", CSprite::Sheet(3, 1).Row(0).From(2).To(0), CColor::Blue());
			cell->SetAnimation("cattackleft", 5);
			cell->Update(GetTime());
			cellcounter = 30;
			cell->SetStatus(CSTANDLEFT);
		}
		else if (cell->HitTest(&player, 1) && cell->GetStatus() == CWALKRIGHT)
		{
			cell->SetMotion(0, 0);
			player.SetVelocity(-player.GetXVelocity(), player.GetYVelocity());
			cell->LoadAnimation("cellattackr.bmp", "cattackright", CSprite::Sheet(3, 1).Row(0).From(2).To(0), CColor::Blue());
			cell->SetAnimation("cattackright", 5);
			cell->Update(GetTime());
			cellcounter = 30;
			cell->SetStatus(CSTANDRIGHT);
		}

		if (cellcounter > 0)
		{

			cellcounter--;
			if (cellcounter == 14 && cell->GetStatus() == CSTANDLEFT)
			{
				CSprite* newShot = new CSprite(cell->GetX() - 30, cell->GetY(), "redblastl.bmp", CColor::Blue(), GetTime());
				newShot->SetMotion(-300, 0);
				newShot->Die(300);
				blastList.push_back(newShot);
				shotSound.Play("Blast1.wav");
				player.SetPosition(player.GetX() - 50, player.GetY());
				cell->SetStatus(CATTACKLEFT);
				cell->Update(GetTime());

				currenthealth = currenthealth - 10;
			}

			else if (cellcounter == 14 && cell->GetStatus() == CSTANDRIGHT)
			{
				CSprite* newShot = new CSprite(player.GetX() + 30, player.GetY(), "redblastr.bmp", CColor::Blue(), GetTime());
				newShot->SetMotion(300, 0);
				newShot->Die(300);
				blastList.push_back(newShot);
				shotSound.Play("Blast1.wav");
				player.SetPosition(player.GetX() + 50, player.GetY());
				cell->SetStatus(CATTACKRIGHT);
				cell->Update(GetTime());
				currenthealth = currenthealth - 10;
			}

			if (cellcounter <= 0 && !cell->HitTest(&player) && cell->GetStatus() == CATTACKLEFT)
			{
				cell->SetImage("crunleft");
				cell->SetMotion(-150, 0);
				cell->SetStatus(CWALKLEFT);
			}
			if (cellcounter <= 0 && !cell->HitTest(&player) && cell->GetStatus() == CATTACKRIGHT)
			{
				cell->SetImage("crunright");
				cell->SetMotion(150, 0);
				cell->SetStatus(CWALKRIGHT);
			}


		


		}
	}
	cellList.delete_if(deleted);
	blastList.delete_if(deleted);


}
/////////////////////////////////////////////////////
// Game Life Cycle

// one time initialisation
void CMyGame::OnInitialize()
{   
	// configuring the animations for the player sprite
	player.LoadImage("gohan.bmp", "standright", 10, 1, 0, 0, CColor::Blue());
	player.LoadAnimation("gohan.bmp", "walkright", CSprite::Sheet(10, 1).Row(0).From(8).To(9),CColor::Blue());
	player.LoadImage("gohanjumpr.bmp", "jumpright", 10, 1, 3, 0, CColor::Blue());
	player.LoadImage("gohanleft.bmp", "standleft", 10, 1, 9, 0, CColor::Blue());
	player.LoadAnimation("gohanleft.bmp", "walkleft", CSprite::Sheet(10, 1).Row(0).From(0).To(1), CColor::Blue());
	player.LoadImage("gohanjumpl.bmp", "jumpleft", 10, 1, 5, 0, CColor::Blue());
	player.LoadAnimation("gohanattackr.bmp", "attackright", CSprite::Sheet(10, 1).Row(0).From(0).To(4), CColor::Blue());
	player.LoadAnimation("gohanattackl.bmp", "attackleft", CSprite::Sheet(10, 1).Row(0).From(9).To(5), CColor::Blue());
	



	player.SetImage("jumpright");
	player.SetStatus(6);

	// configuring the animations for the bear sprite
	

	
	

}

// called when a new game is requested (e.g. when F2 pressed)
// use this function to prepare a menu or a welcome screen
void CMyGame::OnDisplayMenu()
{   
	platformList.delete_all();
	enemyList.delete_all();
	rockList.delete_all();
	towerList.delete_all();
	dragonballList.delete_all();

	if (level == 1)
	{
		SetupLevel1();
	}

	if (level == 2)
	{
		SetupLevel2();
	}

	if (level == 3) 
	{
		SetupLevel3();
	}

	//StartGame();
	menuMusic.Play("menuMusic.wav");
}


void CMyGame::SetupLevel1()
{
	int type, x, y;
	level = 1;
	levelcomplete = false;

	// loading the game map
	background.LoadImage("background2.bmp", "map1", CColor::Black());
	background.SetImage("map1"); background.SetPosition(1200, 300);

	CSprite* pEnemy = new CSprite(400, 120, "bearL.bmp", CColor::Blue(), GetTime());
	pEnemy->LoadAnimation("bearLeft.bmp", "walkleft", CSprite::Sheet(4, 1).Row(0).From(0).To(3), CColor::Blue());
	pEnemy->SetAnimation("walkleft", 10);
	pEnemy->LoadAnimation("bearattackL.bmp", "attackleft", CSprite::Sheet(2, 1).Row(0).From(0).To(1), CColor::Blue());
	pEnemy->LoadAnimation("bearattackR.bmp", "attackright", CSprite::Sheet(2, 1).Row(0).From(0).To(1), CColor::Blue());

	pEnemy->SetMotion(-125, 0);

	enemyList.push_back(pEnemy);

	// declaring new file for reading in data
	fstream myfile;
	// opening file for reading
	myfile.open("level1.txt", ios_base::in);

	// reading while the end of file has not been reached
	bool neof; // not end of file
	do
	{
		// read in data from file line by line
		myfile >> type >> x >> y;

		neof = myfile.good();
		//cout << type << " " << x << " " << y << endl;
		if (neof)
		{
			if (type == 1)
			{
				CSprite* pSprite = new CSprite(x, y, "floor.bmp", CColor::Black(), GetTime());
				platformList.push_back(pSprite);
			}

			if (type == 2)
			{
				CSprite* pSprite = new CSprite(x, y, "platform.bmp", CColor::Blue(), GetTime());
				platformList.push_back(pSprite);
			}

			if (type == 3)
			{
				CSprite* pRock = new CSprite(x, y, "rock.bmp", CColor::Blue(), GetTime());
				rockList.push_back(pRock);
			}

			if (type == 4)
			{
				CSprite* tower = new CSprite(x, y, "Muscle.bmp", CColor::Blue(), GetTime());
				towerList.push_back(tower);
			}



		}
	} while (neof);
	myfile.close();



	



}

// setting up a level 
void CMyGame::SetupLevel2()
{
	int type, x, y;
	enemyonedown = false;
	level = 2;
	levelcomplete = false;

	background.LoadImage("background4.bmp", "map1", CColor::Black());
	background.SetImage("map1"); background.SetPosition(1200, 300);

	CSprite* pEnemy = new CSprite(400, 120, "bearL.bmp", CColor::Blue(), GetTime());
	pEnemy->LoadAnimation("bearLeft.bmp", "walkleft", CSprite::Sheet(4, 1).Row(0).From(0).To(3), CColor::Blue());
	pEnemy->SetAnimation("walkleft", 10);
	pEnemy->LoadAnimation("bearattackL.bmp", "attackleft", CSprite::Sheet(2, 1).Row(0).From(0).To(1), CColor::Blue());
	pEnemy->LoadAnimation("bearattackR.bmp", "attackright", CSprite::Sheet(2, 1).Row(0).From(0).To(1), CColor::Blue());

	pEnemy->SetMotion(-125, 0);

	enemyList.push_back(pEnemy);

	// declaring new file for reading in data
	fstream myfile;
	// opening file for reading
	myfile.open("level2.txt", ios_base::in);

	// reading while the end of file has not been reached
	bool neof; // not end of file
	do
	{
		// read in data from file line by line
		myfile >> type >> x >> y;

		neof = myfile.good();
		//cout << type << " " << x << " " << y << endl;
		if (neof)
		{
			if (type == 1)
			{
				CSprite* pSprite = new CSprite(x, y, "floor2.bmp", CColor::Black(), GetTime());
				platformList.push_back(pSprite);
				//1 1650 40
			}

			if (type == 2)
			{
				CSprite* pSprite = new CSprite(x, y, "platform.bmp", CColor::Blue(), GetTime());
				platformList.push_back(pSprite);
			}

			if (type == 3)
			{
				CSprite* pRock = new CSprite(x, y, "rock.bmp", CColor::Blue(), GetTime());
				rockList.push_back(pRock);
				//3 1400 180
			}

			if (type == 4)
			{
				CSprite* tower = new CSprite(x, y, "Muscle.bmp", CColor::Blue(), GetTime());
				towerList.push_back(tower);
			}

			if (type == 5)
			{
				CSprite* dragonball = new CSprite(x, y, "1star.bmp", CColor::Blue(), GetTime());
				dragonballList.push_back(dragonball);
			}

			if (type == 6)
			{
				CSprite* dragonball = new CSprite(x, y, "2star.bmp", CColor::Blue(), GetTime());
				dragonballList.push_back(dragonball);
			}

			if (type == 7)
			{
				CSprite* dragonball = new CSprite(x, y, "3star.bmp", CColor::Blue(), GetTime());
				dragonballList.push_back(dragonball);
			}

			if (type == 8)
			{
				CSprite* dragonball = new CSprite(x, y, "4star.bmp", CColor::Blue(), GetTime());
				dragonballList.push_back(dragonball);
			}

			if (type == 9)
			{
				CSprite* dragonball = new CSprite(x, y, "5star.bmp", CColor::Blue(), GetTime());
				dragonballList.push_back(dragonball);
			}

			if (type == 10)
			{
				CSprite* dragonball = new CSprite(x, y, "6star.bmp", CColor::Blue(), GetTime());
				dragonballList.push_back(dragonball);
			}

			if (type == 11)
			{
				CSprite* dragonball = new CSprite(x, y, "7star.bmp", CColor::Blue(), GetTime());
				dragonballList.push_back(dragonball);
			}



		}
	} while (neof);
	myfile.close();




}

void CMyGame::SetupLevel3()
{
	int type, x, y;
	level = 3;
	levelcomplete = false;
	// loading the game map
	background.LoadImage("background5.bmp", "map1", CColor::Black());
	background.SetImage("map1"); background.SetPosition(1200, 300);

	CSprite* cell = new CSprite(1900, 110, "cellstandl.bmp", CColor::Blue(), GetTime());
	cell->LoadImage("cellstandl.bmp", "cstandleft", 6, 1, 0, 0, CColor::Blue());
	cell->LoadImage("cellrunl.bmp", "crunleft", 4, 1, 1, 0, CColor::Blue());
	cell->SetImage("crunleft");

	cell->SetMotion(-150, 0);
	
	
	
	cellList.push_back(cell);

	// declaring new file for reading in data
	fstream myfile;
	// opening file for reading
	myfile.open("level3.txt", ios_base::in);

	// reading while the end of file has not been reached
	bool neof; // not end of file
	do
	{
		// read in data from file line by line
		myfile >> type >> x >> y;

		neof = myfile.good();
		//cout << type << " " << x << " " << y << endl;
		if (neof)
		{
			if (type == 1)
			{
				CSprite* pSprite = new CSprite(x, y, "floor.bmp", CColor::Black(), GetTime());
				platformList.push_back(pSprite);
			}
		}
	} while (neof);
	myfile.close();







}


// called when a new game is started
// as a second phase after a menu or a welcome screen
void CMyGame::OnStartGame()
{
	platformList.delete_all();
	enemyList.delete_all();
	rockList.delete_all();
	towerList.delete_all();
	dragonballList.delete_all();
	cellList.delete_all();
	blastList.delete_all();
	
	menuMusic.Stop();
	if (level == 1 || level == 2) 
	{
		bossMusic.Stop();
		backgroundMusic.Play("battle1music.wav", -1);
	}

	if (level == 3) 
	{
		backgroundMusic.Stop();
		bossMusic.Play("Cell.wav", -1);
	}
	
	levelcomplete = false;
    player.SetPosition(100, 200);
	player.SetMotion(0, 0);
	currenthealth = 50;
	rockcurrent = 30;
	ecurrenthealth = 20;
	tcurrent = 50;
	enemyonedown = false;
	enemytwospawned = false;
	score = 0;
	cellcurrent = 50;
	
	if (level == 1) 
	{
		SetupLevel1();
	}

	if (level == 2) 
	{
		SetupLevel2();
	}

	if (level == 3) 
	{
		SetupLevel3();
	}
	
	
	


}

// called when a new level started - first call for nLevel = 1
void CMyGame::OnStartLevel(Sint16 nLevel)
{	

}

// called when the game is over
void CMyGame::OnGameOver()
{
}

// one time termination code
void CMyGame::OnTerminate()
{
}

/////////////////////////////////////////////////////
// Keyboard Event Handlers

void CMyGame::OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode)
{
	if (sym == SDLK_F4 && (mod & (KMOD_LALT | KMOD_RALT)))
		StopGame();
	if (sym == SDLK_F1)
		PauseGame();
	if (sym == SDLK_F2)
	{
		NewGame();
		backgroundMusic.Stop();
		bossMusic.Stop();

		if (levelcomplete == true && (level == 2 || level == 3)) 
		{
			level = 1;
			SetupLevel1();
			NewGame();
		}

		

	}
	if (sym == SDLK_F3) 
	{
		if (levelcomplete == true && level == 1)
		{
			level = 2;
			SetupLevel2();
			StartGame();

		}

		if (levelcomplete == true && level == 2)
		{
			level = 3;
			SetupLevel3();
			StartGame();
		}

	}
	
		
		
}

void CMyGame::OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode)
{
}


/////////////////////////////////////////////////////
// Mouse Events Handlers

void CMyGame::OnMouseMove(Uint16 x,Uint16 y,Sint16 relx,Sint16 rely,bool bLeft,bool bRight,bool bMiddle)
{
}

void CMyGame::OnLButtonDown(Uint16 x,Uint16 y)
{
	// start game with a left mouse button click
	if (IsMenuMode()) StartGame();
	
	
	
	
}

void CMyGame::OnLButtonUp(Uint16 x,Uint16 y)
{
}

void CMyGame::OnRButtonDown(Uint16 x,Uint16 y)
{
}

void CMyGame::OnRButtonUp(Uint16 x,Uint16 y)
{
}

void CMyGame::OnMButtonDown(Uint16 x,Uint16 y)
{
}

void CMyGame::OnMButtonUp(Uint16 x,Uint16 y)
{
}
