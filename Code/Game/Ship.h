#pragma once

class Ship
{
public: 
	enum class ShipSize
	{
		TorpedoBoats,
		Destroyers,
		Cruisers,
		Battleship,
		None
	};

	enum class Direction {
		None = -1,
		Horizontal,
		Vertical,
	};

	Ship(ShipSize shipSize);

	bool GetIsPlaced() const { return isPlaced; } 
	bool GetIsAlive() const { return isAlive; } 

private:
	ShipSize shipSize = ShipSize::None;
	Direction orientation = Direction::Horizontal;
	int shipX = 0;
	int	shipY = 3;
	int shipEnd = 0;
	bool isAlive = true;
	bool isPlaced = false;

	void SetShipEnd();

	friend class Board;
};