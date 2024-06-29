#include "Game.h"

#include <iostream>
#include "Player.h"
#include "Bot.h"
#include "Menu.h"
#include "../Tools/InputManager.h"
#include "../Tools/Console.h"

GameMode Game::gameMode = GameMode::PlayerVsBot;

Game::Game()
{
	SetApplicationSettings();
}

Game::~Game()
{
	Console::ChangColor(Console::Color::Cyan);
	Console::ClearScreen();
}

bool Game::IsRunning() const
{
	return isRunning;
}

void Game::RunGame()
{
	Console::ClearScreen();

	Menu::MainMenuItem mainMenuItemshipMenuItem = Menu::ShowMainMenuItem();

	switch (mainMenuItemshipMenuItem)
	{
	case Menu::MainMenuItem::StartGame:
		StartGame();
		break;
	case Menu::MainMenuItem::Tutorials:
		printGameRules();
		break;
	case Menu::MainMenuItem::Options:
		Options();
		break;
	case Menu::MainMenuItem::Exit:
		isRunning = false;
		break;
	case Menu::MainMenuItem::Esc:
		isRunning = false;
		break;
	default:
		break;
	}
}


void Game::SetApplicationSettings()
{
	Console::SetTitle("Sea Battle");
	Console::HideCursor();
	Console::DisableScrollBars();
}

void Game::StartGame()
{
	bool isStartGame = true;
	const Player* winner = nullptr;

	if (gameMode == GameMode::PlayerVsPlayer)
	{
		Player* player1 = new Player("Plaer1");
		Player* player2 = new Player("Plaer2");

		if(!Board::Initialization(*player1))
			return;
		if (!Board::Initialization(*player2))
			return;

		Console::ClearScreen();

		for (;isStartGame;)
		{
			Runder(*player1, *player2);
			winner = Player::Shoot(*player1, *player2, isStartGame, static_cast<int>(Coordinates::Player2BoardX), static_cast<int>(Coordinates::Player2BoardY));
			if (winner != nullptr)
			{
				Runder(*player1, *player2, Visibility::Visible, Visibility::Visible);
				ShowWinner(*winner);
				isStartGame = false;
			}

			Runder(*player1, *player2);
			winner = Player::Shoot(*player2, *player1, isStartGame, static_cast<int>(Coordinates::Player1BoardX), static_cast<int>(Coordinates::Player1BoardY));
			if (winner != nullptr)
			{
				Runder(*player1, *player2, Visibility::Visible, Visibility::Visible);
				ShowWinner(*winner);
				isStartGame = false;
			}

		}

		delete player1;
		delete player2;
	}
	else if (gameMode == GameMode::PlayerVsBot)
	{
		Player* player = new Player("Plaer");
		Bot* bot = new Bot;

		Board::Initialization(*player);
		Board::AutoPlaceShips(*bot);

		Console::ClearScreen();

		for (; isStartGame;)
		{
			Runder(*player, *bot, Visibility::Visible);
			winner = Player::Shoot(*player, *bot, isStartGame, static_cast<int>(Coordinates::Player2BoardX), static_cast<int>(Coordinates::Player2BoardY));
			if (winner != nullptr)
			{
				Runder(*player, *bot, Visibility::Visible, Visibility::Visible);
				ShowWinner(*winner);
				isStartGame = false;
			}

			Runder(*player, *bot, Visibility::Visible);
			winner = Bot::Shoot(*bot, *player, static_cast<int>(Coordinates::Player1BoardX), static_cast<int>(Coordinates::Player1BoardY));
			if (winner != nullptr)
			{
				Runder(*player, *bot, Visibility::Visible, Visibility::Visible);
				ShowWinner(*winner);
				isStartGame = false;
			}
		}

		delete player;
		delete bot;
	}
	else if (gameMode == GameMode::BotVsBot)
	{
		Bot* bot1 = new Bot;
		Bot* bot2 = new Bot;

		Board::AutoPlaceShips(*bot1);
		Board::AutoPlaceShips(*bot2);

		Console::ClearScreen();

		for (; isStartGame;)
		{
			Runder(*bot1, *bot2, Visibility::Visible, Visibility::Visible);
			winner = Bot::Shoot(*bot1, *bot2, static_cast<int>(Coordinates::Player2BoardX), static_cast<int>(Coordinates::Player2BoardY));
			if (winner != nullptr)
			{
				Runder(*bot1, *bot2, Visibility::Visible, Visibility::Visible);
				ShowWinner(*winner);
				isStartGame = false;
			}

			Runder(*bot1, *bot2, Visibility::Visible, Visibility::Visible);
			winner = Bot::Shoot(*bot2, *bot1, static_cast<int>(Coordinates::Player1BoardX), static_cast<int>(Coordinates::Player1BoardY));
			if (winner != nullptr)
			{
				Runder(*bot1, *bot2, Visibility::Visible, Visibility::Visible);
				ShowWinner(*winner);
				isStartGame = false;
			}
		}

		delete bot1;
		delete bot2;
	}
}

void Game::Runder(Player& player1, Player& player2, Visibility visibilityPlayer1, Visibility visibilityPlayer2)
{
	player1.ShowPlayerName(static_cast<size_t>(Coordinates::Player1NameX), static_cast<size_t>(Coordinates::Player1NameY));
	player1.gameBoard.Show(static_cast<size_t>(Coordinates::Player1BoardX), static_cast<size_t>(Coordinates::Player1BoardY), static_cast<bool>(visibilityPlayer1));

	player2.ShowPlayerName(static_cast<size_t>(Coordinates::Player2NameX), static_cast<size_t>(Coordinates::Player2NameY));
	player2.gameBoard.Show(static_cast<size_t>(Coordinates::Player2BoardX), static_cast<size_t>(Coordinates::Player2BoardY), static_cast<bool>(visibilityPlayer2));
}

void Game::ShowWinner(const Player& player)
{
	Console::ChangColor(Console::Color::LightMagenta);
	std::cout << "Winner ";
	player.ShowPlayerName(Console::Color::LightMagenta);
	Console::StopFor(5000);
}

void Game::printGameRules() 
{
	Console::ClearScreen();
	Console::ChangColor(Console::Color::White);
	std::cout << "1. Game Rules\n";
	std::cout << "Objective: Destroy all opponent's ships located on their field.\n";
	std::cout << "The game field consists of two parts: your field and the opponent's field (bot's).\n";
	std::cout << "Each player has their set of ships placed on the field.\n";
	std::cout << "Total number of ships - 10:\n\n";
	std::cout << "  Torpedo Boats - 4\n";
	std::cout << "  Destroyers - 3\n";
	std::cout << "  Cruisers - 2\n";
	std::cout << "  Battleship - 1\n\n";
	
	std::cout << "An empty cell is marked with ";
	Console::ChangColor(Console::Color::LightBlue);
	std::cout << "'-'.\n";
	Console::ChangColor(Console::Color::White);
	std::cout << "A ship is marked with ";
	Console::ChangColor(Console::Color::LightGreen);
	std::cout << "'0'.\n";
	Console::ChangColor(Console::Color::White);
	std::cout << "A miss is marked with ";
	Console::ChangColor(Console::Color::Cyan);
	std::cout << "'/'.\n";
	Console::ChangColor(Console::Color::White);
	std::cout << "A hit is marked with ";
	Console::ChangColor(Console::Color::LightRed);
	std::cout << "'X'.\n\n";
	Console::ChangColor(Console::Color::White);
	std::cout << "A player who hits shoots again until a miss.\n\n";

	std::cout << "2. Menu Navigation\n";
	std::cout << "Use up and down arrows to navigate the menu.\n";
	std::cout << "Press Enter to select an option.\n\n";

	std::cout << "3. Ship Placement\n";
	std::cout << "During ship placement, use arrows to move around the field.\n";
	std::cout << "Press PgDn to change the ship's orientation between horizontal and vertical.\n";
	std::cout << "Press Esc to remove the ship.\n";
	std::cout << "A green ship can be placed on the field.\n";
	std::cout << "A red ship cannot be placed in the current position.\n";
	std::cout << "Press Enter to save the ship.\n\n";

	std::cout << "4. Ship Editing\n";
	std::cout << "When editing, the purple ship is the selected one for editing.\n";
	std::cout << "Use left and right arrows to select a ship for editing.\n";
	std::cout << "Press Enter to edit the selected ship.\n";
	std::cout << "After that, use arrows to move around the field.\n";
	std::cout << "Press PgDn to change the ship's orientation.\n";
	std::cout << "Press Esc to remove the ship.\n";
	std::cout << "If the ship is green, press Enter to save it.\n";
	std::cout << "Press Esc to exit editing mode.\n\n";

	std::cout << "5. Shooting\n";
	std::cout << "Use arrows to aim.\n";
	std::cout << "Press Enter to shoot.\n";
	std::cout << "Enjoy the game!\n\n";

	std::cout << "6. Game Process\n";
	std::cout << "Select ship placement option from the menu.\n";
	std::cout << "Use arrows to move the ship on the field.\n";
	std::cout << "Press PgDn to change the ship's orientation.\n";
	std::cout << "Press Enter to confirm ship placement.\n";
	std::cout << "Press Esc to remove a ship.\n";
	std::cout << "After placing all ships, start the game and select Save.\n";
	std::cout << "Fire by selecting a cell on the opponent's field.\n";
	std::cout << "Continue the game, taking turns with the opponent, until one player destroys all the opponent's ships.\n";

	while (true)
	{
		if (InputManager::UserPressedKey())
		{
			InputManager::Key key = InputManager::PressedKey();

			switch (key)
			{
			case InputManager::Key::Escape:
				return;
			default:
				break;
			}	
		}
	}
}

void Game::Options()
{
	while (true)
	{
		Console::ClearScreen();

		switch (Menu::ShowOptionsMenuItem())
		{
		case Menu::OptionsMenuItem::GameMode:
			Console::ClearScreen();
			GameMode TempGameMode;

			TempGameMode = Menu::ShowGameModeMenuItem();
			if (TempGameMode != GameMode::Esc)
				gameMode = TempGameMode;
			break;
		case Menu::OptionsMenuItem::BotDifficulty:
			Console::ClearScreen();
			BotDifficulty difficulty;

			difficulty = Menu::ShowBotDifficultyMenuItem();
			if (difficulty != BotDifficulty::Esc)
				Bot::ChangeDifficulty(difficulty);
			break;
		case Menu::OptionsMenuItem::Esc:
			return;
			break;
		default:
			break;
		}
	}
}

