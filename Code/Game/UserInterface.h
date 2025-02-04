#pragma once

#include "Board.h"
#include "Enumerations.h"
#include <functional>
#include "../Tools/GameException.h"
#include "../Tools/InputManager.h"
#include "../Tools/Vector2D.h"

class Player;
class Game;
class Bot;

class UserInterface
{
public:

	const std::string INDENT = "  ";

	static UserInterface& GetObjectWindow();

	void RenderStartScreen();

	// - Main Menu -
	void UpdateMainMenu();
	void RenderMainMenu();
	void HandleInputMainMenu(Game& game);

	// - Statistics -
	void RenderStatistics(const Player& player1, const Player& player2, const Player& player3);
	void HandleInputStatistics();

	// - Tutorial Pages -
	void RenderTutorial();
	void HandleInputTutorial();

	// - Settings -
	void UpdateSettings();
	void RenderSettings();
	void HandleInputSettings();

	// - Settings Name Change -
	void UpdateNameChangeSettings();
	void RenderNameChangeSettings();
	void HandleInputNameChangeSettings(Player& player1, Player& player2);

	// - Settings Game Mode -
	void UpdateGameModeSettings(const Game& game);
	void RenderGameModeSettings();
	void HandleInputGameModeSettings(Game& game);

	// - Ships Setup Menu -
	void UpdateShipsSetupMenu();
	void RenderShipsSetupMenu(bool skip = true);
	std::pair<ShipsSetupMenuItem, bool> HandleInputShipsSetupMenu();

	void RenderBattle();

	// ------------------------------------------------------------------------
	// ------------------------------ RENDER BOARD ----------------------------

	void RenderGameBoard(const Board::GameBoard& gameBoard, Board::Color color, const std::string& playerName, Vector2D boardPosition);
	void RenderGameBoard(const Ship& ship, Console::Color colorShip, const Board::GameBoard& gameBoard, Board::Color color, const std::string& playerName, Vector2D boardPosition);
	void RenderGameBoard(const Vector2D& shot, const Board::GameBoard& gameBoard, Board::Color color, const std::string& playerName, Vector2D boardPosition);
	void RenderGameBoard(const Player& player1, bool RenderMode1, const Player& player2, bool RenderMode2);

	InputManager::Key MoveCrosshair(Vector2D& crosshair);

	void AnnounceWinner(const std::string& playerName);

	// ------------------------------------------------------------------------
	// -------------------------------- GETTERS -------------------------------

	const Screen& GetCurrentScreen() const { return currentScreen; }
	const MenuWindow& GetCurrentMenuWindow() const { return currentMenuWindow; }
	
	// ------------------------------------------------------------------------
	// -------------------------------- RESTORE -------------------------------

	void RestoreCurrentScreen() { currentScreen = Screen::Menu; }
	void RestoreShipsSetupMenuSelectedItem() { shipsSetupMenuSelectedItem = ShipsSetupMenuItem::BattleShip; }
	void RestoreSettingsSelectedItem() { settingsSelectedItem = SettingsMenuItem::NameChange;}
	void RestoreSettingsNameChangeSelectedItem() { settingsNameChangeSelectedItem = SettingsNameChangeMenuItem::Player1; }
	void RestoreSettingsGameModeSelectedItem() { settingsGameModeSelectedItem = SettingsGameModeMenuItem::PlayerVsPlayer; }

private:

	UserInterface();

	UserInterface(UserInterface&) = delete;
	UserInterface& operator=(UserInterface&) = delete;

	// -------------------------------------------------------------------------
	// --------------------------- CONSOLE SETTINGS ----------------------------

	void SetApplicationSettings();

	// -------------------------------------------------------------------------
	// ---------------------------- RENDER METHODS -----------------------------

	void RenderContent(const std::string& content);
	void RenderBoardHeader(Vector2D& boardPosition, const std::string& playerName, const Board::GameBoard& gameBoard);
	void RenderStatistics(const Player& player, Vector2D position);
	void OutputBuffer(std::string& buffer);

	// -------------------------------------------------------------------------
	// ---------------------------- UPDATE METHOD ------------------------------

	void UpdateContent(std::string& content, const std::vector<std::string>& menuItems, const auto& menuSelectedItem, const std::string& fileName = " ");

	// -------------------------------------------------------------------------
	// ----------------------------- NAME CHANGE  ------------------------------

	void ChangName(Player& player);

	// -------------------------------------------------------------------------
	// ------------------------- HANDLE KEYBOARD INPUT -------------------------

	template <std::equality_comparable _Ty>
	void HandleKeyboardInput(_Ty& MenuSelectedItem, _Ty firstItem, _Ty lastItem, const std::unordered_map<_Ty, std::function<void()>>& menuItemActions, std::function<void()> onEscape);

	// -------------------------------------------------------------------------
	// ------------------------ READING ALL DATA METHOD ------------------------

	void ReadGameDataFromFiles();

	// -------------------------------------------------------------------------
	// ----------------------------- COLOR MANAGEMENT --------------------------

	void ChangeColor(const std::string& content, size_t& index);
	bool IsColorCode(const std::string& content, const size_t& index) const;

	std::unordered_map<std::string, Console::Color> ColorCode =
	{
		{"CW",  Console::Color::White},
		{"CB",  Console::Color::Blue},
		{"CM",  Console::Color::Magenta},
		{"CC",  Console::Color::Cyan},
		{"CR",  Console::Color::Red},
		{"CG",  Console::Color::Green},
		{"LB",  Console::Color::LightBlue},
		{"LM",  Console::Color::LightMagenta},
		{"LC",  Console::Color::LightCyan},
		{"LG",  Console::Color::LightGreen},
		{"LR",  Console::Color::LightRed},
	};

	// -------------------------------------------------------------------------
	// --------------------------- APPLICATION DATA ----------------------------

	static constexpr unsigned WINDOW_WIDTH = 103, WINDOW_HEIGHT = 27;
	static constexpr short FONT_SIZE = 30;
	static const std::wstring FONT_NAME;
	Screen currentScreen = Screen::Logo;

	// -------------------------------------------------------------------------
	// ------------------------------- MENU DATA -------------------------------

	// - Current Menu Window - 
	MenuWindow currentMenuWindow = MenuWindow::MainMenu;

	// - Logo -
	const std::string LOGO_FILE_NAME = "Menus/Logo.txt";
	std::string logoContent;

	// - Main Menu -
	const std::string MAIN_MENU_FILE_NAME = "Menus/MainMenu.txt";
	std::string mainMenuContent;
	MainMenuItem mainMenuSelectedItem = MainMenuItem::StartGame;

	// - Ships Setup Menu -
	const std::string SHIPS_SETUP_MENU_FILE_NAME = "Menus/ShipsSetupMenu.txt";
	std::string shipsSetupMenuContent;
	ShipsSetupMenuItem shipsSetupMenuSelectedItem = ShipsSetupMenuItem::BattleShip;

	// - Battle -
	const std::string BATTLE_FILE_NAME = "Menus/Battle.txt";
	std::string battleContent;

	// - Statistics -
	const std::string STATISTICS_FILE_NAME = "Menus/Statistics.txt";
	std::string statisticsContent;

	// - Tutorial Pages -
	static const size_t TUTORIAL_PAGES_COUNT = 4;
	const std::string TUTORIAL_PAGES_NAMES[TUTORIAL_PAGES_COUNT] =
	{
		"Menus/TutorialPage1.txt",
		"Menus/TutorialPage2.txt",
		"Menus/TutorialPage3.txt",
		"Menus/TutorialPage4.txt",
	};

	std::string tutorialPagesContent[TUTORIAL_PAGES_COUNT];
	size_t currentTutorialPageIndex = 0;

	// - Settings -
	const std::string SETTINGS_FILE_NAME = "Menus/Settings.txt";
	std::string settingsContent;
	SettingsMenuItem settingsSelectedItem = SettingsMenuItem::NameChange;

	// - Settings Name Change -
	const std::string SETTINGS_NAME_CHANGE_FILE_NAME = "Menus/SettingsNameChange.txt";
	std::string settingsNameChangeContent;
	SettingsNameChangeMenuItem settingsNameChangeSelectedItem = SettingsNameChangeMenuItem::Player1;

	// - Name Change -
	const std::string NAME_CHANGE_FILE_NAME = "Menus/NameChange.txt";
	std::string nameChangeContent;

	// - Settings Game Mode -
	const std::string SETTINGS_GAME_MODE_FILE_NAME = "Menus/SettingsGameMode.txt";
	std::string settingsGameModeContent;
	SettingsGameModeMenuItem settingsGameModeSelectedItem = SettingsGameModeMenuItem::PlayerVsPlayer;
};

template<std::equality_comparable _Ty>
void UserInterface::HandleKeyboardInput(_Ty& MenuSelectedItem, _Ty firstItem, _Ty lastItem, const std::unordered_map<_Ty, std::function<void()>>& menuItemActions, std::function<void()> onEscape)
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
			ChangeMenuItemUp(MenuSelectedItem, firstItem, lastItem);
			return;

		case InputManager::Key::ArrowDown:
		case InputManager::Key::S:
		   	ChangeMenuItemDown(MenuSelectedItem, firstItem, lastItem);
			return;

		case InputManager::Key::Enter:
		case InputManager::Key::E:
			if (menuItemActions.find(MenuSelectedItem) != menuItemActions.end())
			{
				menuItemActions.at(MenuSelectedItem)();
			}
			else
			{
				throw GameException(
					"In 'menuItemActions', no action for 'MenuSelectedItem': " +
					std::to_string(static_cast<int>(MenuSelectedItem)));
			}
			return;
		case InputManager::Key::Escape:
		case InputManager::Key::PageUp:
			onEscape();
			return;
		}
	}
}