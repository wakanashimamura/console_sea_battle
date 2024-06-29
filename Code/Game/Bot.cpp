#include "Bot.h"

#include <vector>
#include <iostream>
#include <cstring>

#include "../Tools/Random.h"

BotDifficulty Bot::difficulty = BotDifficulty::Normal;

void Bot::ChangeDifficulty(BotDifficulty difficulty)
{
	if (difficulty != BotDifficulty::Esc)
		Bot::difficulty = difficulty;
}

const Player* Bot::Shoot(Bot& shooter, Player& target, int boardX, int boardY)
{
	int shotX = 0, shotY = 0;
	const Player* winner = nullptr;

	for (;true;)
	{
		target.gameBoard.Show(boardX, boardY);

		if(!shooter.isHit)
		{
			if (difficulty == BotDifficulty::Normal)
			{
				if(shooter.dataEnemyShip == nullptr)
					shooter.dataEnemyShip = new DataEnemyShip;
				shooter.dataEnemyShip->sumEnemyLiveShips = target.SumLiveShips();
			}

			GetShootingCoordinates(target, shotX, shotY);
		}

		if(difficulty == BotDifficulty::Normal)
		{
			if (shooter.isHit && shooter.dataEnemyShip->orientation == Ship::Direction::None)
				shooter.ShotNextToHit(shotX, shotY);

			if (shooter.dataEnemyShip->orientation != Ship::Direction::None)
				shooter.DestroyShip(shotX, shotY);
		}

		Board::BoardSymbol boardSymbol = Board::Shot(shotX, shotY, target);

		Board::CheckAndSetAliveStatus(target);
		Board::MarkDestroyedShip(target);

		winner = SelectionWinner(shooter, target);

		if (winner != nullptr)
			return winner;

		if(boardSymbol == Board::BoardSymbol::ShipSymbol)
		{
			if(difficulty == BotDifficulty::Normal)
			{
				if (shooter.isHit && shooter.dataEnemyShip->orientation == Ship::Direction::None)
					shooter.DetermineOrientation();

				if (!shooter.isHit)
				{
					shooter.initialization();
					shooter.dataEnemyShip->isHitX = shotX;
					shooter.dataEnemyShip->isHitY = shotY;
					shooter.isHit = true;
				}

				if (shooter.dataEnemyShip->sumEnemyLiveShips != target.SumLiveShips())
					shooter.restart();
			}

			continue;
		}

		if (boardSymbol == Board::BoardSymbol::HitSymbol)
			continue;

		if (boardSymbol == Board::BoardSymbol::EmptySymbol || boardSymbol == Board::BoardSymbol::MissSymbol)
		{
			if (difficulty == BotDifficulty::Normal)
			{
				if (shooter.isHit && shooter.dataEnemyShip->directionIndex != 3 && shooter.dataEnemyShip->orientation == Ship::Direction::None)
					shooter.dataEnemyShip->directionIndex++;

				if (shooter.dataEnemyShip->orientation != Ship::Direction::None)
					shooter.ChangeDirection();
			}

			if (boardSymbol == Board::BoardSymbol::EmptySymbol)
				return nullptr;

			if (boardSymbol == Board::BoardSymbol::MissSymbol)
				continue;
		}

	}
}

void Bot::GetShootingCoordinates(const Player& target, int& shotX, int& shotY)
{
	Console::StopFor(Random::GenerateInt(780, 1300));

	std::vector<int> emptyColumns = target.gameBoard.FindEmptyColumns();
	shotY = emptyColumns[Random::GenerateInt(0, static_cast<int>(emptyColumns.size()) - 1)];

	std::vector<int> emptyStrings = target.gameBoard.EmptyStrings(shotY);
	shotX = emptyStrings[Random::GenerateInt(0, static_cast<int>(emptyStrings.size()) - 1)];
}

void Bot::initialization()
{
	dataEnemyShip->directions.clear();
	Direction directio;
	for (; dataEnemyShip->directions.size() != 4;)
	{
		directio = static_cast<Direction>(Random::GenerateInt(0, 3));

		bool isNumber = false;

		for (int i = 0; i < dataEnemyShip->directions.size(); ++i)
			if (dataEnemyShip->directions[i] == directio)
				isNumber = true;

		if (!isNumber)
			dataEnemyShip->directions.push_back(directio);
	}

}

void Bot::ShotNextToHit(int& shotX, int& shotY)
{
	Console::StopFor(Random::GenerateInt(440, 750));
	for (; true;)
	{
		if (dataEnemyShip->directions[dataEnemyShip->directionIndex] == Direction::Up || dataEnemyShip->directions[dataEnemyShip->directionIndex] == Direction::Down)
		{
			shotX = dataEnemyShip->isHitX;

			if (dataEnemyShip->directions[dataEnemyShip->directionIndex] == Direction::Up)
				shotY = dataEnemyShip->isHitY - 1 >= 0 ? dataEnemyShip->isHitY - 1 : -1;

			if (dataEnemyShip->directions[dataEnemyShip->directionIndex] == Direction::Down)
				shotY = dataEnemyShip->isHitY + 1 <= 9 ? dataEnemyShip->isHitY + 1 : -1;

			if (shotY == -1)
			{
				dataEnemyShip->directionIndex++;
				continue;
			}
		}
		else if (dataEnemyShip->directions[dataEnemyShip->directionIndex] == Direction::Left || dataEnemyShip->directions[dataEnemyShip->directionIndex] == Direction::Right)
		{
			shotY = dataEnemyShip->isHitY;

			if (dataEnemyShip->directions[dataEnemyShip->directionIndex] == Direction::Left)
				shotX = dataEnemyShip->isHitX - 1 >= 0 ? dataEnemyShip->isHitX - 1 : -1;

			if (dataEnemyShip->directions[dataEnemyShip->directionIndex] == Direction::Right)
				shotX = dataEnemyShip->isHitX + 1 <= 9 ? dataEnemyShip->isHitX + 1 : -1;

			if (shotX == -1)
			{
				dataEnemyShip->directionIndex++;
				continue;
			}
		}
		break;
	}
}

void Bot::DestroyShip(int& shotX, int& shotY)
{
	Console::StopFor(Random::GenerateInt(128, 256));

	if (!dataEnemyShip->isDestroy)
	{
		dataEnemyShip->tempIsHitX = dataEnemyShip->isHitX;
		dataEnemyShip->tempIsHitY = dataEnemyShip->isHitY;
		dataEnemyShip->isDestroy = true;
	}

	for (; true;)
	{
		if (dataEnemyShip->orientation == Ship::Direction::Vertical)
		{
			shotX = dataEnemyShip->tempIsHitX;

			if (dataEnemyShip->direction == Direction::Up)
				if (dataEnemyShip->tempIsHitY - 1 >= 0)
					shotY = --dataEnemyShip->tempIsHitY;
				else
				{
					dataEnemyShip->direction = Direction::Down;
					continue;
				}

			if (dataEnemyShip->direction == Direction::Down)
				if (dataEnemyShip->tempIsHitY + 1 <= 9)
					shotY = ++dataEnemyShip->tempIsHitY;
				else
				{
					dataEnemyShip->direction = Direction::Up;
					continue;
				}
		}
		else if (dataEnemyShip->orientation == Ship::Direction::Horizontal)
		{
			shotY = dataEnemyShip->tempIsHitY;

			if (dataEnemyShip->direction == Direction::Left)
				if (dataEnemyShip->tempIsHitX - 1 >= 0)
					shotX = --dataEnemyShip->tempIsHitX;
				else
				{
					dataEnemyShip->direction = Direction::Right;
					continue;
				}

			if (dataEnemyShip->direction == Direction::Right)
				if (dataEnemyShip->tempIsHitX + 1 <= 9)
					shotX = ++dataEnemyShip->tempIsHitX;
				else
				{
					dataEnemyShip->direction = Direction::Left;
					continue;
				}
		}
		break;
	}
}

void Bot::DetermineOrientation()
{
	if (dataEnemyShip->directions[dataEnemyShip->directionIndex] == Direction::Up || dataEnemyShip->directions[dataEnemyShip->directionIndex] == Direction::Down)
	{
		dataEnemyShip->orientation = Ship::Direction::Vertical;
		dataEnemyShip->direction = dataEnemyShip->directions[dataEnemyShip->directionIndex];
	}
	else if (dataEnemyShip->directions[dataEnemyShip->directionIndex] == Direction::Left || dataEnemyShip->directions[dataEnemyShip->directionIndex] == Direction::Right)
	{
		dataEnemyShip->orientation = Ship::Direction::Horizontal;
		dataEnemyShip->direction = dataEnemyShip->directions[dataEnemyShip->directionIndex];
	}
}

void Bot::restart()
{
	if (dataEnemyShip != nullptr)
	{
		delete dataEnemyShip;
		dataEnemyShip = nullptr;
	}
	isHit = false;
}

void Bot::ChangeDirection()
{
	if (dataEnemyShip->orientation == Ship::Direction::Vertical)
		dataEnemyShip->direction = (dataEnemyShip->direction == Direction::Up) ? Direction::Down : Direction::Up;
	else if (dataEnemyShip->orientation == Ship::Direction::Horizontal)
		dataEnemyShip->direction = (dataEnemyShip->direction == Direction::Left) ? Direction::Right : Direction::Left;

	dataEnemyShip->isDestroy = false;
}