#pragma once

#include "Enumerations.h"
#include "Player.h"

class Bot : public Player
{
public:

	struct ShipSearchState
	{
		int aliveShipsCount = 0;
		bool hitDetected = false;
		Vector2D initialHit{0, 0};
		bool isDirectionFound = false;
		Direction direction = Direction::Up;
	};

	Bot();

	std::pair<ShotStatus, GameAction> Shot(Player& target) override;
	bool PlaceShips() override;

private:

	ShipSearchState shipSearchState;
	void GenerateRandomShot(const Player& target);
	void ReverseDirection();
	void UpdateSearchDirection();
	bool TryMoveInDirection();
	void ResetToDefaultShipSearchState();
};