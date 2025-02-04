#include "Bot.h"

#include "../Tools/Random.h"
#include "UserInterface.h"

Bot::Bot()
	: Player({ 56, 9 }, "Nami")
{ 
	shipSearchState.direction = static_cast<Direction>(Random::GenerateInt(static_cast<int>(Direction::Up), static_cast<int>(Direction::Down)));
}


std::pair<ShotStatus, GameAction> Bot::Shot(Player& target)
{	
	if(!shipSearchState.hitDetected)
	{
		shipSearchState.aliveShipsCount = target.GetBoard().GetAliveShipsCount();
		GenerateRandomShot(target);
	}
	else
	{
		for (; !TryMoveInDirection() ;) {}
	}

	ShotStatus shotRes = target.ShootAtBoard(crosshair);

	if (shotRes == ShotStatus::Hit)
	{
		if (shipSearchState.hitDetected && !shipSearchState.isDirectionFound)
		{
			shipSearchState.isDirectionFound = true;
		}
		else if (!shipSearchState.hitDetected)
		{
			shipSearchState.initialHit = crosshair;
			shipSearchState.hitDetected = true;
		}
	}
	else if ((shotRes == ShotStatus::Miss || shotRes == ShotStatus::NoneShot) && shipSearchState.hitDetected)
	{
		UpdateSearchDirection();
	}

	if (shipSearchState.aliveShipsCount != target.GetBoard().GetAliveShipsCount())
	{
		ResetToDefaultShipSearchState();
	}

	Console::StopFor(shipSearchState.hitDetected ? 300 : Random::GenerateInt(256, 1000));

	return { shotRes, GameAction::Continue };
}

bool Bot::PlaceShips()
{
	boardGame.AutoPlaceShips();

	return true;
}

void Bot::GenerateRandomShot(const Player& target)
{
	std::vector<Vector2D> validShotPositions = target.GetBoard().GetValidShotPositions();

	if (!validShotPositions.empty()) 
	{
		size_t index = Random::GenerateSize_t(validShotPositions.size() - 1);
		
		crosshair = validShotPositions[index];
	}
}

void Bot::ReverseDirection()
{
	switch (shipSearchState.direction)
	{ 
	case Direction::Up:    shipSearchState.direction = Direction::Down;  break;
	case Direction::Left:  shipSearchState.direction = Direction::Right; break;
	case Direction::Right: shipSearchState.direction = Direction::Left;  break;
	case Direction::Down:  shipSearchState.direction = Direction::Up;    break;
	default: throw GameException("Incorrect 'shipSearchState.direction' value");
	}
}

void Bot::UpdateSearchDirection()
{
	crosshair = shipSearchState.initialHit;

	if (shipSearchState.isDirectionFound)
	{
		ReverseDirection();
	}
	else
	{
		ChangeMenuItemUp(shipSearchState.direction, Direction::Up, Direction::Down);
	}
}

bool Bot::TryMoveInDirection()
{
	if (!shipSearchState.isDirectionFound)
	{
		crosshair = shipSearchState.initialHit;
	}

	switch (shipSearchState.direction)
	{
	case Direction::Up:
		if (crosshair.y > 0)
		{
			--crosshair.y;
			return true;
		}
		break;

	case Direction::Left:
		if (crosshair.x > 0)
		{
			--crosshair.x;
			return true;
		}
		break;

	case Direction::Right:
		if (crosshair.x < 9)
		{
			++crosshair.x;
			return true;
		}
		break;

	case Direction::Down:
		if (crosshair.y < 9)
		{
			++crosshair.y;
			return true;
		}
		break;

	default:
		throw GameException("Incorrect 'shipSearchState.direction' value");
	}

	UpdateSearchDirection();
	return false;
}

void Bot::ResetToDefaultShipSearchState()
{
	shipSearchState.aliveShipsCount = 0;
	shipSearchState.hitDetected = false;
	shipSearchState.initialHit = { 0, 0 };
	shipSearchState.isDirectionFound = false;
	shipSearchState.direction = static_cast<Direction>(Random::GenerateInt(static_cast<int>(Direction::Up), static_cast<int>(Direction::Down)));
}