#pragma once

#include <vector>
#include "../Tools/Vector2D.h"

class Ship
{
public:

	enum class ShipSize
	{
		TorpedoBoats,
		Destroyers,
		Cruisers,
		BattleShip
	};

	enum class Direction {
		None = -1,
		Horizontal,
		Vertical
	};

	Ship(ShipSize shipSize);

	void AutoPlace();

	bool MoveUp();
	bool MoveLeft();
	bool MoveRight();
	bool MoveDown();

	void SwitchOrientation();

	void RestoreDefault();

	void SetIsAlive(bool isAlive) { this->isAlive = isAlive; }
	void SetIsPlaced(bool isPlaced) { this->isPlaced = isPlaced; }

	const std::vector<Vector2D>& GetPosition() const { return positions; }
	const Direction& GetOrientation() const { return orientation; }
	const ShipSize& GetShipSize() const { return shipSize; }
	const bool GetIsAlive() const { return isAlive; }
	const bool GetIsPlaced() const { return isPlaced; }

private:

	std::vector<Vector2D> positions;
	ShipSize shipSize;
	Direction orientation = Direction::Vertical;

	bool isAlive = true;
	bool isPlaced = false;

	static const Vector2D STARTING_COORDINATE;
	static const size_t BOARD_SIZE;
};