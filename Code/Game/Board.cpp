#include "Board.h"

#include <iostream>
#include "Player.h"
#include "GameEnumerations.h"
#include "../Tools/InputManager.h"
#include "../Tools/Random.h"

Board::ColorPalette Board::displayPalette = { Console::Color::LightBlue, Console::Color::LightGreen, Console::Color::Cyan, Console::Color::LightRed };
Board::ColorPaletteWithPositions Board::placeShipPalette = { { Console::Color::LightBlue, Console::Color::Magenta, Console::Color::Cyan, Console::Color::LightRed }, Console::Color::LightGreen, Console::Color::LightRed };
Board::ColorPaletteWithPositions Board::editPalette = { { Console::Color::LightBlue, Console::Color::LightGreen, Console::Color::Cyan, Console::Color::LightRed }, Console::Color::Magenta, Console::Color::LightRed };

Board::Board()
{
	Initialization();
}


void Board::Show(size_t x, size_t y, bool hideMode, int shoot_X, int shoot_Y, bool shootMode, const ColorPalette& colorPalette) const
{
	Console::SetCursorPosition(x, y);
	PrintIndexHeader(); 

	for (int i = 0; i < SIZE; ++i)
	{
		Console::ChangColor(Console::Color::White);
		Console::SetCursorPosition(x, ++y);
		std::cout << i << " ";

		for (int j = 0; j < SIZE; ++j)
		{
			if (hideMode)
			{
				SetColorBasedOnSymbol(HiddenGameBoard[i][j], colorPalette);

				if (shootMode && shoot_Y == i && shoot_X == j)
					Console::ChangColor(Console::Color::Whiteandblue);

				std::cout << HiddenGameBoard[i][j] << " ";
			}
			else
			{
				SetColorBasedOnSymbol(GameBoard[i][j], colorPalette);

				if (shootMode && shoot_Y == i && shoot_X == j)
					Console::ChangColor(Console::Color::Whiteandblue);

				std::cout << GameBoard[i][j] << " ";
			}
		}
		std::cout << std::endl;
	}
}

void Board::Show(const Ship& ship, const ColorPaletteWithPositions& colorPalette) const
{
	PrintIndexHeader(); 

	for (int i = 0; i < SIZE; ++i)
	{
		Console::ChangColor(Console::Color::White);
		std::cout << i << " ";

		for (int j = 0; j < SIZE; ++j)
		{
			SetColorBasedOnSymbol(GameBoard[i][j], colorPalette.basePalette);

			bool isCurrentShipCell = false;
			if (ship.orientation == Ship::Direction::Vertical) 
			{
				if (j == ship.shipX && i >= ship.shipEnd && i <= ship.shipY)
					isCurrentShipCell = true;
			}
			else 
			{
				if (i == ship.shipY && j >= ship.shipX && j <= ship.shipEnd)
					isCurrentShipCell = true;
			}

			if (isCurrentShipCell) 
			{
				Console::ChangColor(IsShipPositionValid(ship) ? colorPalette.colorShipPositionCorrect : colorPalette.colorShipPositionIncorrect);
			}

			std::cout << GameBoard[i][j] << " ";
		}
		std::cout << std::endl;
	}
}

void Board::SetColorBasedOnSymbol(char symbol, const ColorPalette& colorPalette) const
{
	if (symbol == static_cast<char>(BoardSymbol::EmptySymbol))
		Console::ChangColor(colorPalette.emptyColor);
	else if (symbol == static_cast<char>(BoardSymbol::ShipSymbol))
		Console::ChangColor(colorPalette.shipColor);
	else if (symbol == static_cast<char>(BoardSymbol::MissSymbol))
		Console::ChangColor(colorPalette.missColor);
	else if (symbol == static_cast<char>(BoardSymbol::HitSymbol))
		Console::ChangColor(colorPalette.hitColor);
	else
		Console::ChangColor(Console::Color::White);
}

void Board::PrintIndexHeader() const {
	Console::ChangColor(Console::Color::White);
	std::cout << "  ";
	for (int i = 0; i < SIZE; ++i) {
		std::cout << static_cast<char>('A' + i) << " ";
	}
	std::cout << std::endl;
}

bool Board::Initialization(Player& player)
{
	ShipIndex Board;
	while (true)
	{
		Board = ShipIndex::None;
		player.gameBoard.Show();
		player.ShowPlayerName(Console::Color::Magenta);
		Menu::ShipMenuItem shipMenuItem = Menu::ShowShipMenuItem();

		switch (shipMenuItem)
		{
		case Menu::ShipMenuItem::Esc:
			return false;
			break;
		case Menu::ShipMenuItem::BattleShip:
		case Menu::ShipMenuItem::Cruisers:
		case Menu::ShipMenuItem::Destroyers:
		case Menu::ShipMenuItem::TorpedoBoats:
			Board = player.gameBoard.GetShipIndex(player, shipMenuItem);
			if (Board != ShipIndex::None)
				player.gameBoard.SetShip(player.Ships[static_cast<int>(Board)]);
			break;
		case Menu::ShipMenuItem::DeleteAll:
			player.gameBoard.RemoveAllShips(player);
			break;
		case Menu::ShipMenuItem::Edit:
			player.gameBoard.EditShip(player);
			break;
		case Menu::ShipMenuItem::Auto:
			player.gameBoard.AutoPlaceShips(player);
			break;
		case Menu::ShipMenuItem::Save:
			if (player.gameBoard.IsSaveAllowed(player))
			{
				Menu::RestartActiveMenuItem();
				return true;
			}
			break;
		default:
			break;
		}
	}
	Console::SetCursorPosition(40, 18);
}

Board::BoardSymbol Board::Shot(int shotX, int shotY, Player& target)
{
	if (shotX >= 0 && shotY <= SIZE - 1 && shotY >= 0 && shotY <= SIZE - 1)
	{
		if (target.gameBoard.GameBoard[shotY][shotX] == static_cast<char>(BoardSymbol::HitSymbol))
			return BoardSymbol::HitSymbol;
		if (target.gameBoard.GameBoard[shotY][shotX] == static_cast<char>(BoardSymbol::MissSymbol))
			return BoardSymbol::MissSymbol;

		if (target.gameBoard.GameBoard[shotY][shotX] == static_cast<char>(BoardSymbol::ShipSymbol))
		{
			target.gameBoard.GameBoard[shotY][shotX] = static_cast<char>(BoardSymbol::HitSymbol);
			target.gameBoard.HiddenGameBoard[shotY][shotX] = static_cast<char>(BoardSymbol::HitSymbol);
			return BoardSymbol::ShipSymbol;
		}
		else if (target.gameBoard.GameBoard[shotY][shotX] == static_cast<char>(BoardSymbol::EmptySymbol))
		{
			target.gameBoard.GameBoard[shotY][shotX] = static_cast<char>(BoardSymbol::MissSymbol);
			target.gameBoard.HiddenGameBoard[shotY][shotX] = static_cast<char>(BoardSymbol::MissSymbol);
			return BoardSymbol::EmptySymbol;
		}
	}

	return BoardSymbol::InvalidShot;
}

void Board::CheckAndSetAliveStatus(Player& player)
{
	for(int shipIndex = 0; shipIndex < static_cast<int>(ShipIndex::ShipCount); ++shipIndex)
	{
		Ship& ship = player.Ships[shipIndex];

		bool isAlive = true;

		if (ship.orientation == Ship::Direction::Vertical)
		{
			for (int i = ship.shipY; i >= ship.shipEnd; --i)
				if (player.gameBoard.GameBoard[i][ship.shipX] == static_cast<char>(BoardSymbol::ShipSymbol))
				{
					isAlive = true;
					break;
				}
				else
					isAlive = false;
		}
		else
		{
			for (int i = ship.shipX; i <= ship.shipEnd; ++i)
				if (player.gameBoard.GameBoard[ship.shipY][i] == static_cast<char>(BoardSymbol::ShipSymbol))
				{
					isAlive = true;
					break;
				}
				else
					isAlive = false;
		}

		ship.isAlive = isAlive;
	}
}

void Board::MarkDestroyedShip(Player& player)
{
	for (int shipIndex = 0; shipIndex < static_cast<int>(ShipIndex::ShipCount); ++shipIndex)
	{
		Ship& ship = player.Ships[shipIndex];

		if (ship.isAlive)
			continue;

		int shipYPlusOne = ship.shipY + 1 <= SIZE - 1 ? ship.shipY + 1 : ship.shipY;
		int shipXMinusOne = ship.shipX - 1 >= 0 ? ship.shipX - 1 : ship.shipX;

		if (ship.orientation == Ship::Direction::Vertical)
		{
			int shipEndMinusOne = ship.shipEnd - 1 >= 0 ? ship.shipEnd - 1 : ship.shipEnd;
			int shipXPlusOne = ship.shipX + 1 <= SIZE - 1 ? ship.shipX + 1 : ship.shipX;

			for (int i = shipEndMinusOne; i <= shipYPlusOne; ++i)
			{
				for (int j = shipXMinusOne; j <= shipXPlusOne; ++j)
				{
					if (j == ship.shipX && i >= ship.shipEnd && i <= ship.shipY)
						continue;

					player.gameBoard.GameBoard[i][j] = static_cast<char>(BoardSymbol::MissSymbol);
					player.gameBoard.HiddenGameBoard[i][j] = static_cast<char>(BoardSymbol::MissSymbol);
				}
			}
		}
		else
		{
			int shipYMinusOne = ship.shipY - 1 >= 0 ? ship.shipY - 1 : ship.shipY;
			int shipEndPlusOne = ship.shipEnd + 1 <= SIZE - 1 ? ship.shipEnd + 1 : ship.shipEnd;

			for (int i = shipYMinusOne; i <= shipYPlusOne; ++i)
			{
				for (int j = shipXMinusOne; j <= shipEndPlusOne; ++j)
				{
					if (i == ship.shipY && j >= ship.shipX && j <= ship.shipEnd)
						continue;

					player.gameBoard.GameBoard[i][j] = static_cast<char>(BoardSymbol::MissSymbol);
					player.gameBoard.HiddenGameBoard[i][j] = static_cast<char>(BoardSymbol::MissSymbol);
				}
			}
		}
	}
}

void Board::Initialization()
{
	for (int i = 0; i < SIZE; ++i)
	{
		for (int j = 0; j < SIZE; ++j)
		{
			GameBoard[i][j] = static_cast<char>(BoardSymbol::EmptySymbol);
			HiddenGameBoard[i][j] = static_cast<char>(BoardSymbol::EmptySymbol);
		}
	}
}

void Board::SetShip(Ship& ship, bool isEdit)
{
	if(!isEdit)
		CheckAndSetNewPosition(ship);

	int shipX = ship.shipX, shipY = ship.shipY;
	Ship::Direction orientation = ship.orientation;

	UpdateShipPositionInBoard(shipX, shipY, orientation, ship, true);

	while (true)
	{
		Console::ResetCursorPosition();
		
		if (InputManager::UserPressedKey())
		{
			InputManager::Key key = InputManager::PressedKey();

			switch (key)
			{
			case InputManager::Key::PgDn:
				ToggleShipDirection(shipX, shipY, orientation, ship);
				break;
			case InputManager::Key::Enter:
				if(IsShipPositionValid(ship))
				{
					UpdateShipProperties(shipX, shipY, orientation, ship);
					return;
				}
				break;
			case InputManager::Key::Escape:
				RemoveShip(ship);
				return;
				break;
			case InputManager::Key::ArrowUp:
				MoveShipUp(shipY, ship);
				break;
			case InputManager::Key::ArrowLeft:
				MoveShipLeft(shipX, ship);
				break;
			case InputManager::Key::ArrowRight:
				MoveShipRight(shipX, ship);
				break;
			case InputManager::Key::ArrowDown:
				MoveShipDown(shipY, ship);
				break;
			default:
				break;
			}
		}
		UpdateShipPositionInBoard(shipX, shipY, orientation, ship);
		Show(ship);
	}
}

ShipIndex Board::GetShipIndex(const Player& player, Menu::ShipMenuItem shipMenuItem) const
{
	switch (shipMenuItem)
	{
	case Menu::ShipMenuItem::BattleShip:
		return findUnplacedShip(player, static_cast<int>(ShipIndex::Battleship), static_cast<int>(ShipIndex::Battleship));
		break;
	case Menu::ShipMenuItem::Cruisers:
		return findUnplacedShip(player, static_cast<int>(ShipIndex::Cruiser1), static_cast<int>(ShipIndex::Cruiser2));
		break;
	case Menu::ShipMenuItem::Destroyers:
		return findUnplacedShip(player, static_cast<int>(ShipIndex::Destroyer1), static_cast<int>(ShipIndex::Destroyer3));
		break;
	case Menu::ShipMenuItem::TorpedoBoats:
		return findUnplacedShip(player, static_cast<int>(ShipIndex::TorpedoBoat1), static_cast<int>(ShipIndex::TorpedoBoat4));
		break;
	default:
		return ShipIndex::None;
	}
}

ShipIndex Board::findUnplacedShip(const Player& player, int startIndex, int endIndex) const
{
	for (int i = startIndex; i <= endIndex; i++)
		if (!player.Ships[i].isPlaced)
			return static_cast<ShipIndex>(i);

	return	ShipIndex::None;
}

void Board::ToggleShipDirection(int& shipX, int& shipY, Ship::Direction& orientation, const Ship& ship)
{
	if (orientation == Ship::Direction::Vertical)
	{ 
		if (shipX + static_cast<int>(ship.shipSize) < SIZE)
		{
			int shipEnd = shipX + static_cast<int>(ship.shipSize);

			for (int i = shipX + 1; i <= shipEnd; ++i)
				if (GameBoard[ship.shipY][i] == static_cast<char>(BoardSymbol::ShipSymbol))
					return;

			orientation = Ship::Direction::Horizontal;
		}
		else
		{
			int shipXTemp = shipX - ((shipX + static_cast<int>(ship.shipSize)) - (SIZE - 1));
			int shipEnd = shipXTemp + static_cast<int>(ship.shipSize);

			for (int i = shipXTemp; i < shipEnd; ++i)
				if (GameBoard[ship.shipY][i] == static_cast<char>(BoardSymbol::ShipSymbol))
					return;

			orientation = Ship::Direction::Horizontal;
			shipX = shipXTemp;
		}
	}
	else
	{
		if (shipY - static_cast<int>(ship.shipSize) >= 0)
		{
			int shipEnd = shipY - static_cast<int>(ship.shipSize);

			for (int i = shipY - 1; i >= shipEnd; --i)
				if (GameBoard[i][shipX] == static_cast<char>(BoardSymbol::ShipSymbol))
					return;

			orientation = Ship::Direction::Vertical;
		}
		else
		{
			int shipYTemp = shipY - (shipY - static_cast<int>(ship.shipSize));
			int shipEnd = shipYTemp - static_cast<int>(ship.shipSize);;

			for (int i = shipYTemp; i > shipEnd; --i) 
				if (GameBoard[i][ship.shipX] == static_cast<char>(BoardSymbol::ShipSymbol))
					return;

			orientation = Ship::Direction::Vertical;
			shipY = shipYTemp;
		}
	}
}

void Board::UpdateShipProperties(int shipX, int shipY, Ship::Direction orientation, Ship& ship, bool isPlaced)
{
	ship.shipX = shipX;
	ship.shipY = shipY;
	ship.orientation = orientation;
	ship.SetShipEnd();
	ship.isPlaced = isPlaced;
}

void Board::UpdateShipPositionInBoard(int& shipX, int& shipY, Ship::Direction& orientation, Ship& ship, bool update)
{
	if (shipX != ship.shipX || shipY != ship.shipY || orientation != ship.orientation || update)
	{
		SetSymbolInArea(ship, BoardSymbol::EmptySymbol);

		UpdateShipProperties(shipX, shipY, orientation, ship);

		SetSymbolInArea(ship, BoardSymbol::ShipSymbol);
	}
}

void Board::SetSymbolInArea(const Ship& ship, BoardSymbol symbol)
{
	if (ship.orientation == Ship::Direction::Vertical)
		for (int i = ship.shipY; i >= ship.shipEnd; --i)
			GameBoard[i][ship.shipX] = static_cast<char>(symbol);
	else
		for (int i = ship.shipX; i <= ship.shipEnd; ++i)
			GameBoard[ship.shipY][i] = static_cast<char>(symbol);
}


bool Board::IsShipPositionValid(const Ship& ship, bool excludeCurrentShip) const
{
	int shipYPlusOne = ship.shipY + 1 <= SIZE - 1 ? ship.shipY + 1 : ship.shipY;
	int shipXMinusOne = ship.shipX - 1 >= 0 ? ship.shipX - 1 : ship.shipX;
		
	if (ship.orientation == Ship::Direction::Vertical)
	{
		int shipEndMinusOne = ship.shipEnd - 1 >= 0 ? ship.shipEnd - 1 : ship.shipEnd;
		int shipXPlusOne = ship.shipX + 1 <= SIZE - 1 ? ship.shipX + 1 : ship.shipX;

		for (int i = shipEndMinusOne; i <= shipYPlusOne; ++i)
		{
			for (int j = shipXMinusOne; j <= shipXPlusOne; ++j)
			{
				if (excludeCurrentShip)
				{
					if (j == ship.shipX && i >= ship.shipEnd && i <= ship.shipY)
						continue;
				}

				if (GameBoard[i][j] == static_cast<char>(BoardSymbol::ShipSymbol))
					return false;
			}
		}
	}
	else
	{
		int shipYMinusOne = ship.shipY - 1 >= 0 ? ship.shipY - 1 : ship.shipY;
		int shipEndPlusOne = ship.shipEnd + 1 <= SIZE - 1 ? ship.shipEnd + 1 : ship.shipEnd;

		for (int i = shipYMinusOne; i <= shipYPlusOne; ++i)
		{
			for (int j = shipXMinusOne; j <= shipEndPlusOne; ++j)
			{
				if (excludeCurrentShip)
				{
					if (i == ship.shipY && j >= ship.shipX && j <= ship.shipEnd)
						continue;
				}

				if (GameBoard[i][j] == static_cast<char>(BoardSymbol::ShipSymbol))
					return false;
			}
		}
	}
	return true;
}

void Board::CheckAndSetNewPosition(Ship& ship)
{
	bool positionValid = IsShipPositionValid(ship, false);
	while (!positionValid)
	{
		int shipX = Random::GenerateInt(0, SIZE - 1);
		int shipY = Random::GenerateInt(0, SIZE - 1);;
		Ship::Direction orientation = static_cast<Ship::Direction>(Random::GenerateInt(0, 1));
		UpdateShipProperties(shipX, shipY, orientation, ship);

		if ((ship.orientation == Ship::Direction::Vertical && ship.shipEnd >= 0) ||
			(ship.orientation == Ship::Direction::Horizontal && ship.shipEnd <= SIZE - 1))
		{
				positionValid = IsShipPositionValid(ship, false);
		}
	}
}

void Board::RemoveAllShips(Player& player)
{
	Initialization();
	for(int i = 0; i < static_cast<int>(ShipIndex::ShipCount); ++i)
		UpdateShipProperties(0, 3, Ship::Direction::Vertical, player.Ships[i], false);
}

void Board::RemoveShip(Ship& ship)
{
	SetSymbolInArea(ship, BoardSymbol::EmptySymbol);
	UpdateShipProperties(0, 3, Ship::Direction::Vertical, ship, false);
}

bool Board::IsSaveAllowed(const Player& player) const
{
	bool isSave = false;
	for (int i = 0; i < static_cast<int>(ShipIndex::ShipCount); ++i)
		if (player.Ships[i].isPlaced == true)
			isSave = true;
		else
			return false;
		
	return isSave;
}

void Board::AutoPlaceShips(Player& player)
{
	player.gameBoard.RemoveAllShips(player);

	for (;!player.gameBoard.IsSaveAllowed(player);)
	{
		for (int i = 0; i < static_cast<int>(ShipIndex::ShipCount); ++i)
		{
			bool positionValid = false;
			for (;!positionValid;)
			{
				int shipX = Random::GenerateInt(0, SIZE - 1);
				int shipY = Random::GenerateInt(0, SIZE - 1);

				int randomValue = Random::GenerateInt(0, 20);
				Ship::Direction orientation = (randomValue <= 10) ? Ship::Direction::Horizontal : Ship::Direction::Vertical;
				
				player.gameBoard.UpdateShipProperties(shipX, shipY, orientation, player.Ships[i], false);

				bool inBounds = (orientation == Ship::Direction::Vertical) ? (player.Ships[i].shipEnd < SIZE) : (player.Ships[i].shipEnd < SIZE);

				if (inBounds && player.gameBoard.IsShipPositionValid(player.Ships[i], false))
				{
					player.gameBoard.UpdateShipPositionInBoard(shipX, shipY, orientation, player.Ships[i], true);
					positionValid = true;
				}
			}
		}
	}
}

std::vector<int> Board::FindEmptyColumns() const
{
	std::vector<int> emptyColumns;
	for (int i = 0; i < SIZE; ++i)
	{
		for (int j = 0; j < SIZE; ++j)
			if (GameBoard[i][j] == static_cast<char>(BoardSymbol::EmptySymbol) || GameBoard[i][j] == static_cast<char>(BoardSymbol::ShipSymbol))
			{
				emptyColumns.push_back(i);
				break;
			}
	}
	return emptyColumns;
}

std::vector<int> Board::EmptyStrings(const int columns) const 
{
	std::vector<int> emptyStrings;

	for (int i = 0; i < SIZE; ++i)
		if (GameBoard[columns][i] == static_cast<char>(BoardSymbol::EmptySymbol) || GameBoard[columns][i] == static_cast<char>(BoardSymbol::ShipSymbol))
			emptyStrings.push_back(i);

	return emptyStrings;
}

void Board::EditShip(Player& player)
{
	Console::ResetCursorPosition();

	int shipIndex = FindFirstPlacedShipIndex(player);

	if (shipIndex == static_cast<int>(ShipIndex::None))
		return;

	while (true)
	{
		Console::ResetCursorPosition();
		if (InputManager::UserPressedKey())
		{
			InputManager::Key key = InputManager::PressedKey();
				
			switch (key)
			{
			case InputManager::Key::Enter:
				SetShip(player.Ships[shipIndex], true);
				if (player.Ships[shipIndex].isPlaced == true)
					break;
				else
					shipIndex = FindFirstPlacedShipIndex(player);
				break;
			case InputManager::Key::Escape:
				return;
				break;
			case InputManager::Key::ArrowLeft:
				for (int i = 0; i < static_cast<int>(ShipIndex::ShipCount); ++i)
				{
					shipIndex--;

					if (shipIndex < static_cast<int>(ShipIndex::Battleship)) 
						shipIndex = static_cast<int>(ShipIndex::TorpedoBoat4);

					if (player.Ships[shipIndex].isPlaced) 
						break;
				}
				break;
			case InputManager::Key::ArrowRight:
				for (int i = 0; i < static_cast<int>(ShipIndex::ShipCount); ++i) 
				{
					shipIndex++;

					if (shipIndex > static_cast<int>(ShipIndex::TorpedoBoat4)) 
						shipIndex = static_cast<int>(ShipIndex::Battleship);

					if (player.Ships[shipIndex].isPlaced) 
						break;
				}
				break;
			default:
				break;
			}
		}

		if (shipIndex == static_cast<int>(ShipIndex::None))
			return;

		Show(player.Ships[shipIndex], editPalette);
	}
}

int Board::FindFirstPlacedShipIndex(const Player& player) const
{
	for (int i = 0; i < static_cast<int>(ShipIndex::ShipCount); ++i)
		if (player.Ships[i].isPlaced == true)
			return i;

	return static_cast<int>(ShipIndex::None);
}

void Board::MoveShipUp(int& shipY, const Ship& ship)
{
	if (ship.orientation == Ship::Direction::Vertical)
	{
		int shipX = ship.shipX;
		int shipEndMinusOne = ship.shipEnd - 1 >= 0 ? ship.shipEnd - 1 : ship.shipEnd;
		
		if (ship.shipY == 0)
			return;

		if (GameBoard[shipEndMinusOne][shipX] == static_cast<char>(BoardSymbol::ShipSymbol))
		{
			int NewshipY = ship.shipEnd - 1;
			int NewshipEnd = NewshipY - static_cast<int>(ship.shipSize);
			while (NewshipEnd >= 0)
			{
				bool isShip = false;

				if (NewshipY < 0 + static_cast<int>(ship.shipSize))
					return;
				if (NewshipEnd < 0)
					return;

				for (int i = NewshipY; i >= NewshipEnd; --i)
					if (GameBoard[i][shipX] == static_cast<char>(BoardSymbol::ShipSymbol))
					{
						isShip = true;
						break;
					}

				if (!isShip)
				{
					shipY = NewshipY;
					return;
				}
				NewshipY--;
				NewshipEnd--;
			}
		}
		else
		{
			if (ship.shipEnd != 0)
				shipY--;
			else
				return;
		}
	}
	else
	{
		int shipX = ship.shipX;
		int shipEnd = ship.shipEnd;
		int shipYMinusOne = shipY - 1 >= 0 ? shipY - 1 : shipY;

		while (shipYMinusOne >= 0)
		{
			bool isShip = false;

			for (int i = shipYMinusOne; i <= shipYMinusOne; ++i)
			{
				for (int j = shipX; j <= shipEnd; ++j)
				{
					if (GameBoard[i][j] == static_cast<char>(BoardSymbol::ShipSymbol))
					{
						isShip = true;
						break;
					}
				}

				if (isShip)
					break;
			}

			if (!isShip)
			{
				if (shipY < SIZE)
				{
					shipY = shipYMinusOne;
					return;

				}
			}

			shipYMinusOne--;
		}
	}
}

void Board::MoveShipLeft(int& shipX, const Ship& ship)
{
	if (ship.orientation == Ship::Direction::Vertical)
	{
		int shipY = ship.shipY;
		int shipEnd = ship.shipEnd;
		int shipXMinusOne = shipX - 1 >= 0 ? shipX - 1 : shipX;

		while (shipXMinusOne >= 0)
		{
			bool isShip = false;

			for (int i = shipEnd; i <= shipY; ++i)
			{
				for (int j = shipXMinusOne; j <= shipXMinusOne; ++j)
				{
					if (GameBoard[i][j] == static_cast<char>(BoardSymbol::ShipSymbol))
					{
						isShip = true;
						break;
					}
				}

				if (isShip)
					break;
			}

			if (!isShip)
			{
				if (shipX >= 0)
				{
					shipX = shipXMinusOne;
					return;

				}
			}

			shipXMinusOne--;
		}
	}
	else
	{
		int shipY = ship.shipY;
		int shipXMinusOne = shipX - 1 >= 0 ? shipX - 1 : shipX;

		if (ship.shipX == 0)
			return;

		if (GameBoard[shipY][shipXMinusOne] == static_cast<char>(BoardSymbol::ShipSymbol))
		{
			int NewshipX = shipX - 1 - static_cast<int>(ship.shipSize);
			int NewshipEnd = NewshipX + static_cast<int>(ship.shipSize);

			while (NewshipX >= 0)
			{
				bool isShip = false;

				if (NewshipX < 0)
					return;

				for (int i = NewshipX; i <= NewshipEnd; ++i)
					if (GameBoard[shipY][i] == static_cast<char>(BoardSymbol::ShipSymbol))
					{
						isShip = true;
						break;
					}

				if (!isShip)
				{
					shipX = NewshipX;
					return;
				}
				NewshipX--;
				NewshipEnd--;
			}
		}
		else
		{
			if (ship.shipX >= 0)
				shipX--;
			else
				return;
		}
	}
}

void Board::MoveShipRight(int& shipX, const Ship& ship)
{
	if (ship.orientation == Ship::Direction::Vertical)
	{
		int shipY = ship.shipY ;
		int shipEnd = ship.shipEnd;
		int shipXPlusOne = shipX + 1 <= SIZE - 1 ? shipX + 1 : shipX;

		while (shipXPlusOne < SIZE)
		{
			bool isShip = false;

			for (int i = shipEnd; i <= shipY; ++i)
			{
				for (int j = shipXPlusOne; j <= shipXPlusOne; ++j)
				{
					if (GameBoard[i][j] == static_cast<char>(BoardSymbol::ShipSymbol))
					{
						isShip = true;
						break;
					}
				}

				if(isShip)
					break;
			}
						
			if (!isShip)
			{
				if (shipX < SIZE)
				{
					shipX = shipXPlusOne;
					return;

				}
			}

			shipXPlusOne++;
		}	
	}
	else
	{
		int shipY = ship.shipY;
		int shipEndPlusOne = ship.shipEnd + 1 <= SIZE - 1 ? ship.shipEnd + 1 : ship.shipEnd;

		if (ship.shipEnd == SIZE - 1)
			return;

		if (GameBoard[shipY][shipEndPlusOne] == static_cast<char>(BoardSymbol::ShipSymbol))
		{
			int NewshipX = ship.shipEnd + 1;
			int NewshipEnd = NewshipX + static_cast<int>(ship.shipSize);
			while (NewshipEnd < SIZE)
			{
				bool isShip = false;

				if (NewshipX > (SIZE - 1) -static_cast<int>(ship.shipSize))
					return;
				if (NewshipEnd >= SIZE)
					return;

				for (int i = NewshipX; i <= NewshipEnd; ++i)
					if (GameBoard[shipY][i] == static_cast<char>(BoardSymbol::ShipSymbol))
					{
						isShip = true;
						break;
					}

				if (!isShip)
				{
					shipX = NewshipX;
					return;
				}
				NewshipX++;
				NewshipEnd++;
			}
		}
		else
		{
			if (ship.shipEnd < SIZE)
				shipX++;
			else
				return;
		}
	}
}

void Board::MoveShipDown(int& shipY, const Ship& ship)
{
	if (ship.orientation == Ship::Direction::Vertical)
	{
		int shipX = ship.shipX;
		int shipYPlusOne = shipY + 1 <= SIZE - 1 ? shipY + 1 : shipY;

		if (ship.shipY == SIZE - 1)
			return;

		if (GameBoard[shipYPlusOne][shipX] == static_cast<char>(BoardSymbol::ShipSymbol))
		{
			int NewshipY = ship.shipY + 1 + static_cast<int>(ship.shipSize);
			int NewshipEnd = NewshipY - static_cast<int>(ship.shipSize);
			while (NewshipY < SIZE)
			{
				bool isShip = false;

				for (int i = NewshipY; i >= NewshipEnd; --i)
					if (GameBoard[i][shipX] == static_cast<char>(BoardSymbol::ShipSymbol))
					{
						isShip = true;
						break;
					}

				if (!isShip)
				{
					shipY = NewshipY;
					return;
				}
				NewshipY++;
				NewshipEnd++;
			}
		}
		else
		{
			if (ship.shipY != (SIZE - 1))
				shipY++;
			else
				return;
		}
	}
	else
	{
		int shipX = ship.shipX;
		int shipEnd = ship.shipEnd;
		int shipYPlusOne = shipY + 1 <= SIZE - 1 ? shipY + 1 : shipY;

		while (shipYPlusOne < SIZE)
		{
			bool isShip = false;

			for (int i = shipYPlusOne; i <= shipYPlusOne; ++i)
			{
				for (int j = shipX; j <= shipEnd; ++j)
				{
					if (GameBoard[i][j] == static_cast<char>(BoardSymbol::ShipSymbol))
					{
						isShip = true;
						break;
					}
				}

				if (isShip)
					break;
			}

			if (!isShip)
			{
				if (shipY < SIZE)
				{
					shipY = shipYPlusOne;
					return;

				}
			}

			shipYPlusOne++;
		}
	}
}