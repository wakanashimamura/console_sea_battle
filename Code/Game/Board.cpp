#include "Board.h"
#include "UserInterface.h"
#include "../Tools/Random.h"

Board::Board()
{
	InitializeBoard();
}

bool Board::SetupPlayerShips(const std::string& playerName)
{
	UserInterface& userInterface = UserInterface::GetObjectWindow(); 
	userInterface.RestoreShipsSetupMenuSelectedItem();

	userInterface.RenderShipsSetupMenu(false);

	for (;;)
	{
		userInterface.UpdateShipsSetupMenu();
		userInterface.RenderShipsSetupMenu();
		userInterface.RenderGameBoard(GetGameBoard(), symbolColors, playerName, defaultPositionGameBoard);

		// Get the player's choice and check if the Enter key is pressed
 		std::pair<ShipsSetupMenuItem, bool> action = userInterface.HandleInputShipsSetupMenu();

		if (action.second == true)
		{
			switch (action.first)
			{
			case ShipsSetupMenuItem::BattleShip:
				SetupShipBySize(Ship::ShipSize::BattleShip, playerName);
				break;

			case ShipsSetupMenuItem::Cruisers:
				SetupShipBySize(Ship::ShipSize::Cruisers, playerName);
				break;

			case ShipsSetupMenuItem::Destroyers:
				SetupShipBySize(Ship::ShipSize::Destroyers, playerName);
				break;

			case ShipsSetupMenuItem::TorpedoBoats:
				SetupShipBySize(Ship::ShipSize::TorpedoBoats, playerName);
				break;

			case ShipsSetupMenuItem::RemoveShips:
				RemoveShips();
				break;

			case ShipsSetupMenuItem::Edit:
				EditMode(playerName);
				break;

			case ShipsSetupMenuItem::Auto:
				AutoPlaceShips();
				break;

			case ShipsSetupMenuItem::Save:
				if (CanSaveBoardState())
				{
					return true;
				}
				break;

			case ShipsSetupMenuItem::Exit:
				ResetToDefaultState();
				return false;

			default:
				throw GameException("Hasn't processed action for ShipsSetupMenuItem(" + std::to_string(static_cast<int>(action.first)) + ")");
			}
		}
	}
}

void Board::AutoPlaceShips()
{
	RemoveShips();

	for (auto& [id, ship] : ships)
	{
		ship.AutoPlace();
		for (; !ValidateShipPosition(ship) ;)
		{
			ship.AutoPlace();
		}
		UpdateShipPositionOnBoard(ship);
		ship.SetIsPlaced(true);
		Console::StopFor(Random::GenerateInt(0, 4));
	}
}

void Board::ResetToDefaultState()
{
	InitializeBoard();
	for (auto& [id, ship] : ships)
	{
		ship.RestoreDefault();
	}
}

void Board::MarkShipAsDestroyed()
{
	for (const auto& [id, ship] : ships)
	{
		if(ship.GetIsAlive())
		{
			continue;
		}

		std::pair<Vector2D, Vector2D> surroundingArea = GetSurroundingArea(ship);

		for (size_t i = surroundingArea.first.y; i <= surroundingArea.second.y && i < gameBoard.size(); ++i)
		{
			for (size_t j = surroundingArea.first.x; j <= surroundingArea.second.x && j < gameBoard.size(); ++j)
			{	
				if (gameBoard[i][j] == static_cast<char>(BoardSymbol::HitSymbol))
				{
					continue;
				}

				gameBoard[i][j] = static_cast<char>(BoardSymbol::MissSymbol);
				hiddenGameBoard[i][j] = static_cast<char>(BoardSymbol::MissSymbol);

			}
		}
	}
}

void Board::UpdateAllAliveShipsStatus()
{
	for (auto& [id, ship] : ships)
	{
		UpdateShipAliveStatus(ship);
	}
}

int Board::GetAliveShipsCount() const
{
	int count = 0;
	for (const auto& [id, ship] : ships)
	{
		if(ship.GetIsAlive())
		{
			++count;
		}
	}
	return count;
}

std::vector<Vector2D> Board::GetValidShotPositions() const
{
	std::vector<Vector2D> validShotPositions;
	
	for (size_t i = 0; i < hiddenGameBoard.size(); i++)
	{
		for (size_t j = 0; j < hiddenGameBoard.size(); j++)
		{
			if (hiddenGameBoard[i][j] != static_cast<char>(BoardSymbol::HitSymbol) &&
				hiddenGameBoard[i][j] != static_cast<char>(BoardSymbol::MissSymbol))
			{
				validShotPositions.push_back({ j, i });
			}
		}
	}

	return validShotPositions;
}

ShotStatus Board::RegisterShot(Vector2D shot)
{
	char& cell = gameBoard[shot.y][shot.x];
	char& hiddenCell = hiddenGameBoard[shot.y][shot.x];

	switch (cell)
	{
	case static_cast<char>(BoardSymbol::HitSymbol):
	case static_cast<char>(BoardSymbol::MissSymbol):
		return ShotStatus::NoneShot;

	case static_cast<char>(BoardSymbol::ShipSymbol):
		cell = hiddenCell = static_cast<char>(BoardSymbol::HitSymbol);
		UpdateAllAliveShipsStatus();
		MarkShipAsDestroyed();
		return ShotStatus::Hit;

	case static_cast<char>(BoardSymbol::EmptySymbol):
		cell = hiddenCell = static_cast<char>(BoardSymbol::MissSymbol);
		return ShotStatus::Miss;

	default:
		return ShotStatus::NoneShot;
	}
}

void Board::InitializeBoard()
{
	for (auto& line : gameBoard)
	{
		line.fill(static_cast<char>(BoardSymbol::EmptySymbol));
	}

	for (auto& line : hiddenGameBoard)
	{
		line.fill(static_cast<char>(BoardSymbol::EmptySymbol));
	}
}

Ship* Board::FindShipBySize(Ship::ShipSize shipSize)
{
	//This method finds the first ship by size that is not set. If the ship is set, it returns nullptr.
	auto shipRange = ships.equal_range(shipSize);
	for (auto iter = shipRange.first; iter != shipRange.second; ++iter)
	{
		if (!iter->second.GetIsPlaced())
		{
			return &iter->second;
		}
	}

	return nullptr;
}

void Board::MoveShip(Ship& ship, Direction direction)
{
	Ship tempShip(ship);

	auto move = [&ship, direction]()
	{
		switch (direction)
		{
		case Direction::Up:
			return ship.MoveUp();

		case Direction::Left:
			return ship.MoveLeft();

		case Direction::Right:
			return ship.MoveRight();

		case Direction::Down:
			return ship.MoveDown();
		default:
			return false;
		}
	};

	move();

	for (;!IsPositionFree(ship);)
	{
		if (!move())
		{
			ship = tempShip;
			break;
		}
	}

	if(tempShip.GetPosition() != ship.GetPosition())
	{
		UpdateShipPositionOnBoard(tempShip, BoardSymbol::EmptySymbol);
		UpdateShipPositionOnBoard(ship);
	}
}

void Board::SetupShipBySize(Ship::ShipSize shipSize, const std::string& playerName)
{
	Ship* ship = FindShipBySize(shipSize);
	if (ship != nullptr)
	{
		SetupShip(*ship, playerName);
	}
}

void Board::SetupShip(Ship& ship, const std::string& playerName, bool editMode)
{
	static constexpr Color colorBoard = { Console::Color::Cyan, Console::Color::Magenta, Console::Color::LightMagenta, Console::Color::Red };
	UserInterface& userInterface = UserInterface::GetObjectWindow();
	Ship tempShip(ship);

	if (editMode && !ship.GetIsPlaced())
	{
		return;
	}

	// Sets a new position if there is another ship on the original one until it finds free cells without a ship.
	for (; !ValidateShipPosition(ship) && !editMode;)
	{
		ship.AutoPlace();
	}

	UpdateShipPositionOnBoard(ship);

	for (;;)
	{
		Console::Color ColorShip = ValidateShipPosition(ship) ? Console::Color::Green : Console::Color::Red;
		userInterface.RenderGameBoard(ship, ColorShip, GetGameBoard(), colorBoard, playerName, defaultPositionGameBoard);

		if (!InputManager::UserPressedKey())
		{
			continue;
		}

		InputManager::Key key = InputManager::PressedKey();

		switch (key)
		{
		case InputManager::Key::Enter:
		case InputManager::Key::E:
			if (ValidateShipPosition(ship))
			{
		 	 	ship.SetIsPlaced(true);
				return;
			}
			break;

		case InputManager::Key::Escape:
		case InputManager::Key::PageUp:
			if (editMode)
			{
				UpdateShipPositionOnBoard(ship, BoardSymbol::EmptySymbol);
				ship = tempShip;
				UpdateShipPositionOnBoard(ship);
			}
			else
			{
				RemoveShip(ship);
			}
			return;

		case InputManager::Key::Tab:
		case InputManager::Key::Delete:
			RemoveShip(ship);
			return;
		case InputManager::Key::ArrowUp:
		case InputManager::Key::W:
			MoveShip(ship, Direction::Up);
			break;

		case InputManager::Key::ArrowLeft:
		case InputManager::Key::A:
			MoveShip(ship, Direction::Left);
			break;

		case InputManager::Key::ArrowRight:
		case InputManager::Key::D:
			MoveShip(ship, Direction::Right);
			break;

		case InputManager::Key::ArrowDown:
		case InputManager::Key::S:
		 	MoveShip(ship, Direction::Down);
			break;

		case InputManager::Key::PageDown:
		case InputManager::Key::Q:
			SwitchOrientation(ship);
			break;
		}
	}
}

std::pair<Vector2D, Vector2D> Board::GetSurroundingArea(const Ship& ship) const
{
	std::pair<Vector2D, Vector2D> surroundingArea{ ship.GetPosition().front(), ship.GetPosition().back() };

	if (ship.GetOrientation() == Ship::Direction::Horizontal)
	{
		surroundingArea.first.x = surroundingArea.first.x == 0 ? surroundingArea.first.x : --surroundingArea.first.x;
		surroundingArea.first.y = surroundingArea.first.y == 0 ? surroundingArea.first.y : --surroundingArea.first.y;

		surroundingArea.second.x = surroundingArea.second.x == BOARD_SIZE - 1 ? surroundingArea.second.x : ++surroundingArea.second.x;
		surroundingArea.second.y = surroundingArea.second.y == BOARD_SIZE - 1 ? surroundingArea.second.y : ++surroundingArea.second.y;
	}
	else
	{
		surroundingArea.first.x = surroundingArea.first.x == BOARD_SIZE - 1 ? surroundingArea.first.x : ++surroundingArea.first.x;
		surroundingArea.first.y = surroundingArea.first.y == BOARD_SIZE - 1 ? surroundingArea.first.y : ++surroundingArea.first.y;

		surroundingArea.second.x = surroundingArea.second.x == 0 ? surroundingArea.second.x : --surroundingArea.second.x;
		surroundingArea.second.y = surroundingArea.second.y == 0 ? surroundingArea.second.y : --surroundingArea.second.y;

		std::swap(surroundingArea.first, surroundingArea.second);
	}

	return surroundingArea;
}


void Board::EditMode(const std::string& playerName)
{
	UserInterface& userInterface = UserInterface::GetObjectWindow();

	auto ship = ships.end();
	auto FindFirstPlacedShip = [this, &ship]()
	{
		for (auto shipIter = ships.begin(); shipIter != ships.end(); ++shipIter)
		{
			if (shipIter->second.GetIsPlaced())
			{
				ship = shipIter;
				break;
			}
		}
	};

	if (FindFirstPlacedShip(); ship == ships.end())
	{
		return;
	}
   	
	for (;;)
	{
	 	userInterface.RenderGameBoard(ship->second, Console::Color::Magenta, GetGameBoard(), symbolColors, playerName, defaultPositionGameBoard);

		if (!InputManager::UserPressedKey())
		{
			continue;
		}

		InputManager::Key key = InputManager::PressedKey();

		switch (key)
		{
		case InputManager::Key::Enter:
		case InputManager::Key::E:
			SetupShip(ship->second, playerName, true);
			if(ship->second.GetIsPlaced())
			{
				break;
			}

			if (FindFirstPlacedShip(); ship == ships.end())
			{
				return;
			}
			break;

		case InputManager::Key::Escape:
		case InputManager::Key::PageUp:
			return;

		case InputManager::Key::ArrowLeft:
		case InputManager::Key::A:
			for (;;)
			{
				if (ship == ships.begin())
				{
					ship = ships.end();
				}
				--ship;

				if (ship->second.GetIsPlaced())
				{
					break;
				}
			}
			break;
	
		case InputManager::Key::ArrowRight:
		case InputManager::Key::D:
			for (;;)
			{
				++ship;
				if (ship == ships.end())
				{
					ship = ships.begin();
				}

				if (ship->second.GetIsPlaced())
				{
					break;
				}
			}
			break;
		}
	}
}

void Board::SwitchOrientation(Ship& ship)
{
	Ship tempShip(ship);
	ship.SwitchOrientation();
	if (IsPositionFree(ship))
	{
		UpdateShipPositionOnBoard(tempShip, BoardSymbol::EmptySymbol);
		UpdateShipPositionOnBoard(ship);
	}
	else
	{
		ship = tempShip;
	}
}

bool Board::IsPositionFree(const Ship& ship) const
{
	for (const auto& [id, currentShip] : ships)
	{	
		if (!currentShip.GetIsPlaced() || &currentShip == &ship)
		{
			continue;
		}

		for (const auto& firstCoordinates : ship.GetPosition())
		{
		 	for (const auto& secondCoordinates : currentShip.GetPosition())
			{
				if (firstCoordinates == secondCoordinates)
				{
					return false;
				}
			}
		}
	}

	return true;
}

bool Board::ValidateShipPosition(const Ship& ship) const
{
	std::pair<Vector2D, Vector2D> surroundingArea = GetSurroundingArea(ship);

	for (const auto& [id, currentShip] : ships)
	{
		if (!currentShip.GetIsPlaced() || &currentShip == &ship)
		{
			continue;
		}

		for (const auto& coordinate : currentShip.GetPosition())
		{
			if (coordinate.x >= surroundingArea.first.x && coordinate.x <= surroundingArea.second.x &&
				coordinate.y >= surroundingArea.first.y && coordinate.y <= surroundingArea.second.y)
			{
				return false;
			}
		}
	}

	return true;
}

void Board::UpdateShipPositionOnBoard(const Ship& ship, BoardSymbol boardSymbol)
{
	for (const auto& cordinat : ship.GetPosition())
	{
	 	gameBoard[cordinat.y][cordinat.x] = static_cast<char>(boardSymbol);
	}
}

void Board::RemoveShip(Ship& ship)
{
	UpdateShipPositionOnBoard(ship, BoardSymbol::EmptySymbol);
	ship.RestoreDefault();
}

void Board::RemoveShips()
{
	for (auto& [id, ship] : ships)
	{
		RemoveShip(ship);
	}
}

bool Board::CanSaveBoardState()
{
	for (const auto& [id, ship] : ships)
	{
		if (!ship.GetIsPlaced())
		{
			return false;
		}
	}
	return true;
}

void Board::UpdateShipAliveStatus(Ship& ship)
{
	for (auto& coordinate : ship.GetPosition())
	{
		if (gameBoard[coordinate.y][coordinate.x] == static_cast<char>(BoardSymbol::ShipSymbol))
		{
			return;
		}
	}

	ship.SetIsAlive(false);
}