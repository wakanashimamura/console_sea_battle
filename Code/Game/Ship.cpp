#include "Ship.h"
#include "Board.h"

Ship::Ship(ShipSize shipSize)
{
	this->shipSize = shipSize;
	SetShipEnd();
}

void Ship::SetShipEnd()
{
	if (orientation == Ship::Direction::Vertical)
		shipEnd = shipY - static_cast<int>(shipSize);
	else
		shipEnd = shipX + static_cast<int>(shipSize);
}