#include "UserInterface.h"

#include <array>
#include <concepts>
#include "../Tools/Console.h"
#include "../Tools/FileManager.h"
#include "Game.h"
#include <iomanip>
#include "Player.h"
#include <unordered_map>
#include "../Tools/Vector2D.h"

const std::wstring UserInterface::FONT_NAME = L"Consolas";

UserInterface& UserInterface::GetObjectWindow()
{
	static UserInterface userInterface;
	return userInterface;
}

void UserInterface::RenderStartScreen()
{
	Console::ResetCursorPosition();
	Console::ChangeColor(Console::Color::White);
	std::string buffer;

	for (size_t i = 0; i < logoContent.size(); ++i)
	{
		if (IsColorCode(logoContent, i))
		{
			OutputBuffer(buffer);
			ChangeColor(logoContent, i);
		}

		buffer += logoContent[i];

		if (logoContent[i] == '\n')
		{
			OutputBuffer(buffer);
			Console::StopFor(128);
		}

		if (InputManager::UserPressedKey())
		{
			currentScreen = Screen::Menu;
			return;
		}
	}

	Console::StopFor(2048);
	currentScreen = Screen::Menu;
}

void UserInterface::UpdateMainMenu()
{
	static const std::vector<std::string> MAIN_MENU_ITEMS =
	{
		"Start Game",
		"Statistics",
		"Tutorials",
		"Settings",
		"Exit"
	};

	UpdateContent(mainMenuContent, MAIN_MENU_ITEMS, mainMenuSelectedItem, MAIN_MENU_FILE_NAME);
}

void UserInterface::RenderMainMenu()
{
	RenderContent(mainMenuContent);
}

void UserInterface::HandleInputMainMenu(Game& game)
{
	static const std::unordered_map<MainMenuItem, std::function<void()>> menuActions = {
		{MainMenuItem::StartGame,  [this, &game]() { currentScreen = Screen::Game; game.SetIsGameRunning(true);  }},
		{MainMenuItem::Statistics, [this]() { currentMenuWindow = MenuWindow::Statistics; }},
		{MainMenuItem::Tutorials,  [this]() { currentMenuWindow = MenuWindow::Tutorial; }},
		{MainMenuItem::Settings,   [this]() { currentMenuWindow = MenuWindow::Settings; }},
		{MainMenuItem::Exit,       [&game]() {game.SetIsLoadGame(false); game.SaveData(); }}
	};

	int sizebit = sizeof(menuActions);

	HandleKeyboardInput(mainMenuSelectedItem, MainMenuItem::StartGame, MainMenuItem::Exit, menuActions, []() {});
}

void UserInterface::RenderStatistics(const Player& player1, const Player& player2, const Player& player3)
{
	RenderContent(statisticsContent);
	RenderStatistics(player1, {24, 11});
	RenderStatistics(player2, {51, 11});
	RenderStatistics(player3, {78, 11});
}

void UserInterface::HandleInputStatistics()
{
	InputManager::ClearBuffer();
	for (;;)
	{
		if (!InputManager::UserPressedKey())
		{
			continue;
		}

		InputManager::Key key = InputManager::PressedKey();

		switch (key)
		{
		case InputManager::Key::Escape:
		case InputManager::Key::PageUp:
			currentMenuWindow = MenuWindow::MainMenu;
			return;
		}
	}
}

void UserInterface::RenderTutorial()
{
	RenderContent(tutorialPagesContent[currentTutorialPageIndex]);
}

void UserInterface::HandleInputTutorial()
{
	InputManager::ClearBuffer();
	for (;;)
	{
		if (!InputManager::UserPressedKey())
		{
			continue;
		}

		InputManager::Key key = InputManager::PressedKey();

		switch (key)
		{
		case InputManager::Key::ArrowLeft:
		case InputManager::Key::A:
			currentTutorialPageIndex > 0 ? currentTutorialPageIndex-- : currentTutorialPageIndex;
			return;

		case InputManager::Key::ArrowRight:
		case InputManager::Key::D:
			currentTutorialPageIndex < TUTORIAL_PAGES_COUNT - 1 ? currentTutorialPageIndex++ : currentTutorialPageIndex;
			return;

		case InputManager::Key::Escape:
		case InputManager::Key::PageUp:
			currentTutorialPageIndex = 0;
			currentMenuWindow = MenuWindow::MainMenu;
			return;
		}
	}
}

void UserInterface::UpdateSettings()
{
	static const std::vector<std::string> SETTINGS_MENU_ITEMS =
	{
		"Name Change",
		"Game Mode",
		"Exit"
	};

	UpdateContent(settingsContent, SETTINGS_MENU_ITEMS, settingsSelectedItem, SETTINGS_FILE_NAME);
}

void UserInterface::RenderSettings()
{
	RenderContent(settingsContent);
}

void UserInterface::HandleInputSettings()
{
	static const std::unordered_map<SettingsMenuItem, std::function<void()>> menuActions = {
		{SettingsMenuItem::NameChange,    [this]() { currentMenuWindow = MenuWindow::SettingsNameChange; }},
		{SettingsMenuItem::GameMode,      [this]() { currentMenuWindow = MenuWindow::SettingsGameMode; }},
		{SettingsMenuItem::Exit,          [this]() { currentMenuWindow = MenuWindow::MainMenu; RestoreSettingsSelectedItem(); }}
	};

	HandleKeyboardInput(settingsSelectedItem, SettingsMenuItem::NameChange, SettingsMenuItem::Exit, menuActions, [this]() { currentMenuWindow = MenuWindow::MainMenu; RestoreSettingsSelectedItem(); });
}

void UserInterface::UpdateNameChangeSettings()
{
	static const std::vector<std::string> SETTINGS_NAME_CHANGE_MENU_ITEMS =
	{
		"Player 1",
		"Player 2",
		"Exit"
	};

	UpdateContent(settingsNameChangeContent, SETTINGS_NAME_CHANGE_MENU_ITEMS, settingsNameChangeSelectedItem, SETTINGS_NAME_CHANGE_FILE_NAME);
}

void UserInterface::RenderNameChangeSettings()
{
	RenderContent(settingsNameChangeContent);
}

void UserInterface::HandleInputNameChangeSettings(Player& player1, Player& player2)
{
	static const std::unordered_map<SettingsNameChangeMenuItem, std::function<void()>> menuActions = {
		{SettingsNameChangeMenuItem::Player1, [this, &player1]() {ChangName(player1); FileManager::WritePlayerInBinaryFile(Game::PLAYER1_FILE_NAME, player1); }},
	 	{SettingsNameChangeMenuItem::Player2, [this, &player2]() {ChangName(player2); FileManager::WritePlayerInBinaryFile(Game::PLAYER2_FILE_NAME, player2); }},
		{SettingsNameChangeMenuItem::Exit,    [this]() { currentMenuWindow = MenuWindow::Settings; RestoreSettingsNameChangeSelectedItem(); }}
	};

	HandleKeyboardInput(settingsNameChangeSelectedItem, SettingsNameChangeMenuItem::Player1, SettingsNameChangeMenuItem::Exit, menuActions, [this]() { currentMenuWindow = MenuWindow::Settings; RestoreSettingsNameChangeSelectedItem(); });
}

void UserInterface::UpdateGameModeSettings(const Game& game)
{
	const std::vector<std::string> SETTINGS_GAME_MODE_MENU_ITEMS =
	{
		"Player vs Player",
		"Player vs Bot",
		"Exit"
	};

	UpdateContent(settingsGameModeContent, SETTINGS_GAME_MODE_MENU_ITEMS, settingsGameModeSelectedItem, SETTINGS_GAME_MODE_FILE_NAME);

	if (size_t arrowPosition = settingsGameModeContent.find("X"); arrowPosition != std::string::npos)
	{
		settingsGameModeContent[arrowPosition] = ' ';
	}
	
	std::string gameMode = SETTINGS_GAME_MODE_MENU_ITEMS[static_cast<int>(game.GetGameMode())];
	size_t selectedItemPosition = settingsGameModeContent.find(gameMode);

	if (selectedItemPosition != std::string::npos)
	{
		settingsGameModeContent[selectedItemPosition + 19] = 'X';
	}

	if (selectedItemPosition == std::string::npos)
	{
	 	throw GameException("File " + SETTINGS_GAME_MODE_FILE_NAME + ", or array SETTINGS_GAME_MODE_MENU_ITEMS is incorrect!");
	}
}

void UserInterface::RenderGameModeSettings()
{
	RenderContent(settingsGameModeContent);
}

void UserInterface::HandleInputGameModeSettings(Game& game)
{
	static const std::unordered_map<SettingsGameModeMenuItem, std::function<void()>> menuActions = {
		{SettingsGameModeMenuItem::PlayerVsPlayer, [this, &game]() { game.SetGameMode(GameMode::PlayerVsPlayer); FileManager::WriteGameModeInBinaryFile(Game::GAME_MODE_FILE_NAME, game.GetGameMode()); }},
		{SettingsGameModeMenuItem::PlayerVsBot,    [this, &game]() { game.SetGameMode(GameMode::PlayerVsBot);    FileManager::WriteGameModeInBinaryFile(Game::GAME_MODE_FILE_NAME, game.GetGameMode()); }},
	 	{SettingsGameModeMenuItem::Exit,           [this]() { currentMenuWindow = MenuWindow::Settings;	RestoreSettingsGameModeSelectedItem(); }}
	};

	HandleKeyboardInput(settingsGameModeSelectedItem, SettingsGameModeMenuItem::PlayerVsPlayer, SettingsGameModeMenuItem::Exit, menuActions, [this]() { currentMenuWindow = MenuWindow::Settings;	RestoreSettingsGameModeSelectedItem(); });
}

void UserInterface::UpdateShipsSetupMenu()
{
	static const std::vector<std::string> SHIPS_SETUP_MENU_ITEMS =
	{
		"Battle Ship",
		"Cruisers",
		"Destroyers",
		"Torpedo Boats",
		"Remove Ships",
		"Edit",
		"Auto",
		"Save",
		"Exit"
	};

	UpdateContent(shipsSetupMenuContent, SHIPS_SETUP_MENU_ITEMS, shipsSetupMenuSelectedItem, SHIPS_SETUP_MENU_FILE_NAME);
}

void UserInterface::RenderShipsSetupMenu(bool skip)
{
	Console::ResetCursorPosition();
	std::string buffer;

	auto Skip = [this](const std::string& Content, size_t& index, std::string& buffer)
		{
			if (Content[index] != '>')
			{
				return;
			}

			if (std::string code = Content.substr(index, 2); code == ">>")
			{
				OutputBuffer(buffer += INDENT);
				index += 2;

				if (index + 21 < Content.size())
				{
					index += 21;
					Console::SetCursorPosition(Console::GetCursorPosition().first + 21, Console::GetCursorPosition().second);
				}
				else
				{
					throw GameException("Not enough space for the playing board");
				}
			}
		};

	Console::ChangeColor(Console::Color::White);

	for (size_t i = 0; i < shipsSetupMenuContent.size(); ++i)
	{
		if (IsColorCode(shipsSetupMenuContent, i))
		{
			OutputBuffer(buffer);
			ChangeColor(shipsSetupMenuContent, i);
		}

	 	if (skip)
		{
			Skip(shipsSetupMenuContent, i, buffer);
		}

		buffer += shipsSetupMenuContent[i];
	}

	OutputBuffer(buffer);
}

std::pair<ShipsSetupMenuItem, bool> UserInterface::HandleInputShipsSetupMenu()
{
	InputManager::ClearBuffer();
	for (;;)
	{
		if (!InputManager::UserPressedKey())
		{
			continue;
		}

		InputManager::Key key = InputManager::PressedKey();

		switch (key)
		{
		case InputManager::Key::ArrowUp:
		case InputManager::Key::W:
			ChangeMenuItemUp(shipsSetupMenuSelectedItem, ShipsSetupMenuItem::BattleShip, ShipsSetupMenuItem::Exit);
			return std::pair<ShipsSetupMenuItem, bool>(shipsSetupMenuSelectedItem, false);

		case InputManager::Key::ArrowDown:
		case InputManager::Key::S:
			ChangeMenuItemDown(shipsSetupMenuSelectedItem, ShipsSetupMenuItem::BattleShip, ShipsSetupMenuItem::Exit);
			return std::pair<ShipsSetupMenuItem, bool>(shipsSetupMenuSelectedItem, false);

		case InputManager::Key::Enter:
		case InputManager::Key::E:
			if (shipsSetupMenuSelectedItem == ShipsSetupMenuItem::Exit)
			{
			 	RestoreCurrentScreen();
			}
			return std::pair<ShipsSetupMenuItem, bool>(shipsSetupMenuSelectedItem, true);

		case InputManager::Key::Escape:
		case InputManager::Key::PageUp:
			RestoreCurrentScreen();
			shipsSetupMenuSelectedItem = ShipsSetupMenuItem::Exit;
			return std::pair<ShipsSetupMenuItem, bool>(shipsSetupMenuSelectedItem, true);
		}
	}
}

void UserInterface::RenderBattle()
{
	RenderContent(battleContent);
}

void UserInterface::RenderGameBoard(const Board::GameBoard& gameBoard, Board::Color color, const std::string& playerName, Vector2D boardPosition)
{
	RenderBoardHeader(boardPosition, playerName, gameBoard);

	for (int i = 0; i < gameBoard.size(); ++i)
	{
		Console::ChangeColor(Console::Color::White);
		Console::SetCursorPosition(boardPosition.x, ++boardPosition.y);
		std::cout << i << " ";

		for (int j = 0; j < gameBoard[0].size(); ++j)
		{
			if (gameBoard[i][j] == static_cast<char>(Board::BoardSymbol::EmptySymbol))
				Console::ChangeColor(color.emptyColor);
			else if (gameBoard[i][j] == static_cast<char>(Board::BoardSymbol::MissSymbol))
				Console::ChangeColor(color.missColor);
			else if (gameBoard[i][j] == static_cast<char>(Board::BoardSymbol::ShipSymbol))
				Console::ChangeColor(color.shipColor);
			else if (gameBoard[i][j] == static_cast<char>(Board::BoardSymbol::HitSymbol))
				Console::ChangeColor(color.hitColor);

			std::cout << gameBoard[i][j] << " ";
		}
		std::cout << std::endl;
	}
}

void UserInterface::RenderGameBoard(const Ship& ship, Console::Color colorShip, const Board::GameBoard& gameBoard, Board::Color color, const std::string& playerName, Vector2D boardPosition)
{
	RenderBoardHeader(boardPosition, playerName, gameBoard);

	Vector2D start = ship.GetPosition().front();
	Vector2D end = ship.GetPosition().back();

	if (ship.GetOrientation() == Ship::Direction::Vertical)
	{
		std::swap(start, end);
	}

	for (int i = 0; i < gameBoard.size(); ++i)
	{
		Console::ChangeColor(Console::Color::White);
		Console::SetCursorPosition(boardPosition.x, ++boardPosition.y);
		std::cout << i << " ";

		for (int j = 0; j < gameBoard[0].size(); ++j)
		{
			if (gameBoard[i][j] == static_cast<char>(Board::BoardSymbol::EmptySymbol))
				Console::ChangeColor(color.emptyColor);
			else if (gameBoard[i][j] == static_cast<char>(Board::BoardSymbol::MissSymbol))
				Console::ChangeColor(color.missColor);
			else if (j >= start.x && j <= end.x &&
				i >= start.y && i <= end.y)
				Console::ChangeColor(colorShip);
			else if (gameBoard[i][j] == static_cast<char>(Board::BoardSymbol::ShipSymbol))
				Console::ChangeColor(color.shipColor);
			else if (gameBoard[i][j] == static_cast<char>(Board::BoardSymbol::HitSymbol))
				Console::ChangeColor(color.hitColor);

			std::cout << gameBoard[i][j] << " ";
		}
		std::cout << std::endl;
	}
}

void UserInterface::RenderGameBoard(const Vector2D& shot, const Board::GameBoard& gameBoard, Board::Color color, const std::string& playerName, Vector2D boardPosition)
{
	RenderBoardHeader(boardPosition, playerName, gameBoard);

	for (int i = 0; i < gameBoard.size(); ++i)
	{
		Console::ChangeColor(Console::Color::White);
		Console::SetCursorPosition(boardPosition.x, ++boardPosition.y);
		std::cout << i << " ";

		for (int j = 0; j < gameBoard[0].size(); ++j)
		{
			if (shot.y == i && shot.x == j)
				Console::ChangeColor(Console::Color::Whiteandblue);
			else if (gameBoard[i][j] == static_cast<char>(Board::BoardSymbol::EmptySymbol))
				Console::ChangeColor(color.emptyColor);
			else if (gameBoard[i][j] == static_cast<char>(Board::BoardSymbol::MissSymbol))
				Console::ChangeColor(color.missColor);
			else if (gameBoard[i][j] == static_cast<char>(Board::BoardSymbol::ShipSymbol))
				Console::ChangeColor(color.shipColor);
			else if (gameBoard[i][j] == static_cast<char>(Board::BoardSymbol::HitSymbol))
				Console::ChangeColor(color.hitColor);

			std::cout << gameBoard[i][j] << " ";
		}
		std::cout << std::endl;
	}
}

void UserInterface::RenderGameBoard(const Player& player1, bool RenderMode1, const Player& player2, bool RenderMode2)
{
	const Board::GameBoard& gameBoard1 = RenderMode1 ? player1.GetBoard().GetGameBoard() : player1.GetBoard().GetHiddenGameBoard();
	const Board::GameBoard& gameBoard2 = RenderMode2 ? player2.GetBoard().GetGameBoard() : player2.GetBoard().GetHiddenGameBoard();

	RenderGameBoard(gameBoard1, Board::symbolColors, player1.GetName(), player1.GetBoardPosition());
	RenderGameBoard(gameBoard2, Board::symbolColors, player2.GetName(), player2.GetBoardPosition());
}

InputManager::Key UserInterface::MoveCrosshair(Vector2D& crosshair)
{
	InputManager::ClearBuffer();

	for (;;)
	{
		if (!InputManager::UserPressedKey())
		{
			continue;
		}

		InputManager::Key key = InputManager::PressedKey();

		switch (key)
		{
		case InputManager::Key::Escape:
		case InputManager::Key::PageUp:
			return key;

		case InputManager::Key::Enter:
		case InputManager::Key::E:
			return key;
	
		case InputManager::Key::ArrowUp:
		case InputManager::Key::W:
			crosshair.y = crosshair.y == 0 ? Board::BOARD_SIZE - 1 : crosshair.y - 1;
			return key;

		case InputManager::Key::ArrowLeft:
		case InputManager::Key::A:
			crosshair.x = crosshair.x == 0 ? Board::BOARD_SIZE - 1 : crosshair.x - 1;
			return key;

		case InputManager::Key::ArrowRight:
		case InputManager::Key::D:
			crosshair.x = crosshair.x == Board::BOARD_SIZE - 1 ? 0 : crosshair.x + 1;
			return key;

		case InputManager::Key::ArrowDown:
		case InputManager::Key::S:
			crosshair.y = crosshair.y == Board::BOARD_SIZE - 1 ? 0 : crosshair.y + 1;
			return key;
		}
	}
}

void UserInterface::AnnounceWinner(const std::string& playerName)
{
	Console::ChangeColor(Console::Color::LightMagenta);
	Console::SetCursorPosition(25, 22);
	std::cout << "Player " << playerName << " has won!";
	Console::SetCursorPosition(25, 24);
}

UserInterface::UserInterface()
{
	SetApplicationSettings();
	ReadGameDataFromFiles();
}

void UserInterface::SetApplicationSettings()
{
	Console::SetTitle("Sea Battle");
	Console::SetWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	Console::SetFontSettings(FONT_SIZE, FONT_NAME);
	Console::HideCursor();
	Console::DisableScrollBars();
	Console::DisableMaximizeButton();
	Console::DisableWindowResizing();
}

void UserInterface::RenderContent(const std::string& content)
{
	Console::ResetCursorPosition();
	Console::ChangeColor(Console::Color::White);
	std::string buffer;

	for (size_t i = 0; i < content.size(); ++i)
	{

		if (IsColorCode(content, i))
		{
			OutputBuffer(buffer);
			ChangeColor(content, i);
		}

		buffer += content[i];
	}

	OutputBuffer(buffer);
}

void UserInterface::RenderBoardHeader(Vector2D& boardPosition, const std::string& playerName, const Board::GameBoard& gameBoard)
{
	Console::ChangeColor(Console::Color::White);

	Console::SetCursorPosition(boardPosition.x, boardPosition.y);
	std::cout << INDENT << "[" << playerName << "]\n";
	++boardPosition.y;

 	Console::SetCursorPosition(boardPosition.x, boardPosition.y);
	Console::ChangeColor(Console::Color::White);

	std::cout << INDENT;
	for (int i = 0; i < gameBoard.size(); ++i)
	{
		std::cout << static_cast<char>('A' + i) << " ";
	}
	std::cout << std::endl;
}

void UserInterface::RenderStatistics(const Player& player, Vector2D position)
{
	const Statistics& statistics = player.GetStatistics();
	Console::SetCursorPosition(position.x, position.y);
	std::cout << "[" << player.GetName() << "]";
	position.y += 2;
	Console::SetCursorPosition(position.x, position.y);
	std::cout << "[" << static_cast<int>(statistics.GetLevel()) << "]";
	
	for (const auto& [id, stat] : statistics.GetStatistics())
	{
		position.y += 2;
		Console::SetCursorPosition(position.x, position.y);
		std::cout << "[" << stat << "]";
	}
}

void UserInterface::OutputBuffer(std::string& buffer)
{
	if (!buffer.empty())
	{
		std::cout << buffer;
		buffer.clear();
	}
}

void UserInterface::UpdateContent(std::string& content, const std::vector<std::string>& menuItems, const auto& menuSelectedItem, const std::string& fileName)
{
	if (size_t arrowPosition = content.find("->"); arrowPosition != std::string::npos)
	{
		content[arrowPosition] = ' ';
		content[arrowPosition + 1] = ' ';
	}

	size_t selectedItemPosition;

	if (static_cast<int>(menuSelectedItem) > menuItems.size())
	{
		throw GameException("'menuSelectedItem' exceeds the number of available 'menuItems'");
	}

	if (selectedItemPosition = content.find(menuItems[static_cast<int>(menuSelectedItem)]); selectedItemPosition != std::string::npos)
	{
		content[selectedItemPosition - 4] = '-';
		content[selectedItemPosition - 3] = '>';
	}

	if (selectedItemPosition == std::string::npos)
	{
		throw GameException("File " + fileName + " is incorrect!");
	}
}

void UserInterface::ChangName(Player& player)
{
	Vector2D namePosition{ 51 - (player.GetName().length() / 2), 13 };
	Vector2D EnterNamePosition{ 43, 18 };

	RenderContent(nameChangeContent);

	Console::SetCursorPosition(namePosition.x, namePosition.y);
	std::cout << player.GetName();
	
	int index = 0;
	char inputChar;
	std::string newName;

	InputManager::ClearBuffer();
	for (;;)
	{
		if (!InputManager::UserPressedKey())
		{
			continue;
		}

		inputChar = InputManager::Getch();

		if (inputChar == 27) // Escape
		{
			return;
		}

		if (inputChar == 13) // Enter
		{
			player.ChangeName(newName);
			return;
		}

		if (inputChar == 8) // Backspace
		{
			if (index == 0)
				continue;
			--index;
			std::cout << "\b \b";
		 	if(!newName.empty())
			{
				newName.pop_back();
			}
			continue;
		}

		if ( ((inputChar >= '!' && inputChar <= '9') || (inputChar >= '?' && inputChar <= 'z')) && index < Player::MAX_USERNAME_LENGTH)
		{
			Console::SetCursorPosition(EnterNamePosition.x + index, EnterNamePosition.y);
			std::cout << inputChar;
			newName += inputChar;
			index++;
		}
	}
}

void UserInterface::ReadGameDataFromFiles()
{
	auto ReadTextFromFile = [](const std::string& FileName, std::string& content)
	{
		if (!FileManager::ReadTextFromFile(FileName, content))
		{
			throw GameException("File '" + FileName + "' could not been opened/found!");
		}
	};

	try
	{
		// --- Read Logo ---
		ReadTextFromFile(LOGO_FILE_NAME, logoContent);

		// --- Read Main Menu ---
		ReadTextFromFile(MAIN_MENU_FILE_NAME, mainMenuContent);

		// --- Read Ships Setup Menu ---
		ReadTextFromFile(SHIPS_SETUP_MENU_FILE_NAME, shipsSetupMenuContent);

		// --- Read Game Board ---
		ReadTextFromFile(BATTLE_FILE_NAME, battleContent);

		// --- Read Statistics ---
		ReadTextFromFile(STATISTICS_FILE_NAME, statisticsContent);

		// --- Read Tutorial Pages ---
		for (size_t i = 0; i < TUTORIAL_PAGES_COUNT; ++i)
		{
			ReadTextFromFile(TUTORIAL_PAGES_NAMES[i], tutorialPagesContent[i]);
		}

		// --- Read Settings ---
		ReadTextFromFile(SETTINGS_FILE_NAME, settingsContent);

		// --- Read Settings Name Change ---
		ReadTextFromFile(SETTINGS_NAME_CHANGE_FILE_NAME, settingsNameChangeContent);

		// --- Read Name Change ---
	 	ReadTextFromFile(NAME_CHANGE_FILE_NAME, nameChangeContent);

		// --- Read Settings Game Mode ---
		ReadTextFromFile(SETTINGS_GAME_MODE_FILE_NAME, settingsGameModeContent);
	}
	catch (const std::exception& erorr)
	{
		RenderErorr(erorr);
	}
}

void UserInterface::ChangeColor(const std::string& content, size_t& index)
{
	if (std::string code = content.substr(index, 2); ColorCode.find(code) != ColorCode.end())
	{
		Console::ChangeColor(ColorCode[code]);
		std::cout << INDENT; // Replacing the color code with 2 space
		index += 2; // Move the index 2 positions forward to skip the color code
	}
}

bool UserInterface::IsColorCode(const std::string& content, const size_t& index) const
{
	// Colors starting with 'C' 'L'
	return (index + 2 < content.size() && content[index] == 'C' || content[index] == 'L') && (ColorCode.find(content.substr(index, 2)) != ColorCode.cend());
}