#include "Ship.h"

#include "Board.h"
#include "../Tools/Random.h"

const Vector2D Ship::STARTING_COORDINATE = {4, 6};
const size_t Ship::BOARD_SIZE = Board::BOARD_SIZE - 1;

Ship::Ship(ShipSize shipSize)
	:shipSize(shipSize)
{ 
	for (size_t i = 0; i <= static_cast<int>(shipSize); ++i)
	{
		positions.emplace_back(STARTING_COORDINATE.x, STARTING_COORDINATE.y - i);
	}
}

void Ship::AutoPlace()
{
	orientation = static_cast<Direction>(Random::GenerateInt(static_cast<int>(Direction::Horizontal), static_cast<int>(Direction::Vertical)));

	Vector2D position{ 0, 0 };

	if (orientation == Direction::Horizontal)
	{
		position.x = static_cast<size_t>(Random::GenerateInt(0, BOARD_SIZE - static_cast<int>(shipSize)));
		position.y = static_cast<size_t>(Random::GenerateInt(0, BOARD_SIZE));
	}
	else
	{
		position.x = static_cast<size_t>(Random::GenerateInt(0, BOARD_SIZE));
		position.y = static_cast<size_t>(Random::GenerateInt(0 + static_cast<int>(shipSize), BOARD_SIZE));
	}

	for (size_t i = 0; i < positions.size(); ++i)
	{
		if (orientation == Direction::Horizontal)
		{
			positions[i] = { position.x + i, position.y };
		}
		else
		{
			positions[i] = { position.x, position.y - i };
		}
	}
}

bool Ship::MoveUp()
{
	if (positions.back().y > 0)
	{
		for (auto& coordinate : positions)
		{
			--coordinate.y;
		}
		return true;
	}
	return false;
}

bool Ship::MoveLeft()
{
	if (positions.front().x > 0)
	{
		for (auto& coordinate : positions)
		{
			--coordinate.x;
		}
		return true;
	}
	return false;
}

bool Ship::MoveRight()
{
	if (positions.back().x < BOARD_SIZE)
	{
		for (auto& coordinate : positions)
		{
			++coordinate.x;
		}
		return true;
	}
	return false;
}

bool Ship::MoveDown()
{
	if (positions.front().y < BOARD_SIZE)
	{
		for (auto& coordinate : positions)
		{
			++coordinate.y;
		}
		return true;
	}
	return false;
}

void Ship::SwitchOrientation()
{
	for (size_t i = 0; i < positions.size(); ++i)
	{
		if (orientation == Direction::Horizontal)
		{
			if (positions[i].y < static_cast<int>(shipSize))
			{
				positions[i].y = static_cast<size_t>(shipSize) - i;
				positions[i].x = positions[0].x;
			}
			else
			{
				positions[i] = { positions[0].x, positions[i].y - i };
			}
		}
		else
		{
		  	if (positions[i].x > BOARD_SIZE - static_cast<size_t>(shipSize))
			{
				positions[i].y = positions[0].y;
				positions[i].x = (BOARD_SIZE - static_cast<size_t>(shipSize)) + i;
			}
			else
			{
				positions[i] = { positions[i].x + i, positions[0].y };
			}
		}
	}

	orientation = orientation == Direction::Horizontal ? Direction::Vertical : Direction::Horizontal;
}

void Ship::RestoreDefault()
{
	for (size_t i = 0; i < positions.size(); ++i)
	{
		positions[i] = { STARTING_COORDINATE.x, STARTING_COORDINATE.y - i };
	}

	orientation = Direction::Vertical;
	isAlive = true;
	isPlaced = false;
}