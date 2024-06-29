#pragma once

#include<vector>
#include "Player.h"
#include "GameEnumerations.h"

class Bot : public Player
{
public:

	Bot() : Player("Bot"){}

	enum class Direction
	{
		None = -1,
		Up, 
		Left, 
		Down, 
		Right
	};

	struct DataEnemyShip
	{
		int isHitY = -1;
		int isHitX = -1;

		int tempIsHitX = -1;
		int tempIsHitY = -1;

		int	sumEnemyLiveShips = 0;

		Ship::Direction orientation = Ship::Direction::None;

		bool isDestroy = false;

		int directionIndex = 0;
		std::vector<Direction> directions;
		Direction direction = Direction::None;
	};

	static void ChangeDifficulty(BotDifficulty difficulty);

	static const Player* Shoot (Bot& shooter, Player& target, int boardX, int boardY);

private:

	static void GetShootingCoordinates(const Player& target, int& shotX, int& shotY);
	
	void initialization();

	void ShotNextToHit(int& shotX, int& shotY);
	void DestroyShip(int& shotX, int& shotY);
	void DetermineOrientation();
	void ChangeDirection();
	void restart();

	static BotDifficulty difficulty;

	DataEnemyShip* dataEnemyShip = nullptr;
	bool isHit = false;
};