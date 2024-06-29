#include "Menu.h"

#include "../Tools/Console.h"
#include "../Tools/InputManager.h"

Menu::ShipMenuItem Menu::activeShipMenuItem = ShipMenuItem::BattleShip;

template<typename T>
static T Menu::ShowMenu(const std::string* MenuItem, size_t size, T& active_MenuItem, int x)
{
	while (true)
	{
		for (int i = 0; i < size; ++i)
		{
			Console::SetCursorPosition(x, i);
			if (active_MenuItem == static_cast<T>(i))
			{
				Console::ChangColor(Console::Color::LightMagenta);
				std::cout << "[->] ";
			}
			else
			{
				Console::ChangColor(Console::Color::White);
				std::cout << "[  ] ";
			}

			std::cout << MenuItem[i] << std::endl;
		}

		if (InputManager::UserPressedKey())
		{
			InputManager::Key key = InputManager::PressedKey();
			int activeIndex = static_cast<int>(active_MenuItem);

			switch (key)
			{
			case InputManager::Key::Escape:
				return T::Esc;
				break;
			case InputManager::Key::Enter:
				return active_MenuItem;
				break;
			case InputManager::Key::ArrowUp:
				if (activeIndex > 0)
					active_MenuItem = static_cast<T>(activeIndex - 1);
				break;
			case InputManager::Key::ArrowDown:
				if (activeIndex < size - 1)
					active_MenuItem = static_cast<T>(activeIndex + 1);
				break;
			default:
				break;
			}
		}
	}
}

Menu::MainMenuItem Menu::ShowMainMenuItem()
{
	const std::string MainMenuItemStrings[] = { "Start Game", "Tutorials", "Options",  "Exit" };
	MainMenuItem activeMainMenuItem = MainMenuItem::StartGame;
	return ShowMenu(MainMenuItemStrings, std::size(MainMenuItemStrings), activeMainMenuItem, 0);
}

Menu::ShipMenuItem Menu::ShowShipMenuItem()
{
	const std::string ShipMenuItemStrings[] = { "BattleShip", "Cruisers", "Destroyers", "Torpedo Boats",  "Delete all", "Edit", "Auto", "Save" };
	return ShowMenu(ShipMenuItemStrings, std::size(ShipMenuItemStrings), activeShipMenuItem, 30);
}

Menu::OptionsMenuItem Menu::ShowOptionsMenuItem()
{
	const std::string OptionsMenuItemStrings[] = { "Game mode", "Bot difficulty" };
	OptionsMenuItem activeOptionsMenuItem = OptionsMenuItem::GameMode;
	return ShowMenu(OptionsMenuItemStrings, std::size(OptionsMenuItemStrings), activeOptionsMenuItem, 0);
}

GameMode Menu::ShowGameModeMenuItem()
{
	const std::string GameModeMenuItemStrings[] = { "Player vs player", "Player vs bot", "Bot vs bot" };
	GameMode activeGameModeMenuItem = GameMode::PlayerVsPlayer;
	return ShowMenu(GameModeMenuItemStrings, std::size(GameModeMenuItemStrings), activeGameModeMenuItem, 0);
}

BotDifficulty Menu::ShowBotDifficultyMenuItem()
{
	const std::string BotDifficultyMenuItemStrings[] = { "Easy", "Normal" };
	BotDifficulty activeBotDifficultyMenuItem = BotDifficulty::Easy;
	return ShowMenu(BotDifficultyMenuItemStrings, std::size(BotDifficultyMenuItemStrings), activeBotDifficultyMenuItem, 0);
}

void Menu::RestartActiveMenuItem()
{
	activeShipMenuItem = ShipMenuItem::BattleShip;
}
