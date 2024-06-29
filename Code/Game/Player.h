#pragma once

#include "Board.h"	
#include "Ship.h"

class Ship;
class Board;

class Player
{
public:

	void ChangePlayerName(const std::string& name);

    void ShowPlayerName(int X = 0, int Y = 0, Console::Color color = Console::Color::Magenta) const;
	void ShowPlayerName(Console::Color color = Console::Color::Magenta) const;

	static const Player* Shoot(const Player& shooter, Player& target, bool& isStartGame, int boardX, int boardY);
	
	static const Player* SelectionWinner(const Player& shooter, const Player& target);

	bool IsLivingShip() const;
	int SumLiveShips() const;

	Player(const std::string& name);

	static const size_t SIZE = 10;

	Ship Ships[SIZE] =
	{
		Ship(Ship::ShipSize::Battleship),
		Ship(Ship::ShipSize::Cruisers),Ship(Ship::ShipSize::Cruisers),
		Ship(Ship::ShipSize::Destroyers),Ship(Ship::ShipSize::Destroyers),Ship(Ship::ShipSize::Destroyers),
		Ship(Ship::ShipSize::TorpedoBoats),Ship(Ship::ShipSize::TorpedoBoats),Ship(Ship::ShipSize::TorpedoBoats),Ship(Ship::ShipSize::TorpedoBoats)
	};

	Board gameBoard;

private:

	static bool GetShootingCoordinates(const Player& target, int& shotX, int& shotY, int boardX, int boardY);

	std::string playerName = "None Name";
};