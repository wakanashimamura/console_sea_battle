#include "Player.h"

#include <iostream>
#include <cstring>
#include "../Tools/InputManager.h"

void Player::ChangePlayerName(const std::string& name)
{
	playerName = name;
}

void Player::ShowPlayerName(int X, int Y, Console::Color color) const
{
	Console::SetCursorPosition(X, Y);
	Console::ChangColor(color);
	std::cout << playerName;
}

void Player::ShowPlayerName(Console::Color color) const
{
	Console::ChangColor(color);
	std::cout << playerName;
}

const Player* Player::Shoot(const Player& shooter, Player& target, bool& isStartGame, int boardX, int boardY)
{
	int shotX = 5, shotY = 5;
	const Player* winner = nullptr;

	for (;true;)
	{
		if(!GetShootingCoordinates(target, shotX, shotY, boardX, boardY))
		{
			isStartGame = false;
			return nullptr;
		}
		Board::BoardSymbol boardSymbol = Board::Shot(shotX, shotY, target);


		Board::CheckAndSetAliveStatus(target);
		Board::MarkDestroyedShip(target);

		winner = SelectionWinner(shooter, target);

		if (winner != nullptr)
			return winner;

		if (boardSymbol == Board::BoardSymbol::HitSymbol || boardSymbol == Board::BoardSymbol::MissSymbol || boardSymbol == Board::BoardSymbol::ShipSymbol)
			continue;

		if (boardSymbol == Board::BoardSymbol::EmptySymbol)
			return nullptr;
	}
}

bool Player::IsLivingShip() const
{
	for (int shipIndex = 0; shipIndex < static_cast<int>(ShipIndex::ShipCount); ++shipIndex)
	{
		const Ship& ship = Ships[shipIndex];

		if (ship.GetIsAlive())
			return true;
	}

	return false;
}

int Player::SumLiveShips() const
{
	int sum = 0;
	for (int shipIndex = 0; shipIndex < static_cast<int>(ShipIndex::ShipCount); ++shipIndex)
	{
		const Ship& ship = Ships[shipIndex];

		if (ship.GetIsAlive())
			++sum;
	}

	return sum;
}

bool Player::GetShootingCoordinates(const Player& target, int& shotX, int& shotY, int boardX, int boardY)
{
	for (;true;)
	{
		if (InputManager::UserPressedKey())
		{
			target.gameBoard.Show(boardX, boardY, true);
			InputManager::Key key = InputManager::PressedKey();

			switch (key)
			{
			case InputManager::Key::Escape:
				return false;
			case InputManager::Key::Enter:
				if (shotX >= 0 && shotX <= Board::SIZE - 1 && shotY >= 0 && shotY <= Board::SIZE - 1)
					return true;
				break;
			case InputManager::Key::ArrowUp:
				if (shotY > 0)
					shotY--;
				else
					shotY = Board::SIZE - 1;
				break;
			case InputManager::Key::ArrowLeft:
				if (shotX > 0)
					shotX--;
				else
					shotX = Board::SIZE - 1;
				break;
			case InputManager::Key::ArrowRight:
				if (shotX < Board::SIZE - 1)
					shotX++;
				else
					shotX = 0;
				break;
			case InputManager::Key::ArrowDown:
				if (shotY < Board::SIZE - 1)
					shotY++;
				else
					shotY = 0;
				break;
			default:
				break;
			}
		}
		target.gameBoard.Show(boardX, boardY, true, shotX, shotY, true);
	}
}

const Player* Player::SelectionWinner(const Player& shooter, const Player& target) 
{
	int sumShipsShooter = shooter.SumLiveShips();
	int sumShipsTarget = target.SumLiveShips();

	if (sumShipsShooter > 0 && sumShipsTarget > 0)
		return nullptr;

	if (sumShipsTarget == 0)
		return &shooter;

	if (sumShipsShooter == 0)
		return &target;

	return nullptr;
}

Player::Player(const std::string& name)
{
	ChangePlayerName(name);
}