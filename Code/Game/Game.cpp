#include "Game.h"

#include "../Tools/FileManager.h"

const std::string Game::PLAYER1_FILE_NAME   = "Data/Player1Data.bin";
const std::string Game::PLAYER2_FILE_NAME   = "Data/Player2Data.bin";
const std::string Game::BOT_FILE_NAME       = "Data/BotData.bin";
const std::string Game::GAME_MODE_FILE_NAME = "Data/GameModeData.bin";

Game::Game()
	:player1({ 25, 9 }, "Player1"), player2({ 56, 9 }, "Player1"),
	isLoadGame(true), isGameRunning(false), gameMode(GameMode::PlayerVsBot),
	userInterface(UserInterface::GetObjectWindow())
{ }

void Game::LoadGame()
{
	ReadData();
	try
	{
		for (; isLoadGame ;)
		{
			switch (userInterface.GetCurrentScreen())
			{
			case Screen::Logo:
				userInterface.RenderStartScreen();
				break;

			case Screen::Menu:
				Menu();
				break;

			case Screen::Game:
				if (gameMode == GameMode::PlayerVsPlayer)
				{
					StartGame(player1, player2);
				}
				else if (gameMode == GameMode::PlayerVsBot)
				{
					StartGame(player1, bot);
				}
				break;

			default:
				throw GameException("Incorrect 'currentScreen' value");
				break;
			}
		}
	}
	catch (const std::exception& erorr)
	{
		RenderErorr(erorr);
	}

	Console::ClearScreen();
}

void Game::SaveData() const
{
	FileManager::WriteGameModeInBinaryFile(GAME_MODE_FILE_NAME, gameMode);
	FileManager::WritePlayerInBinaryFile(PLAYER1_FILE_NAME, player1);
	FileManager::WritePlayerInBinaryFile(PLAYER2_FILE_NAME, player2);
	FileManager::WritePlayerInBinaryFile(BOT_FILE_NAME, bot);
}

void Game::ReadData()
{
	FileManager::ReadGameModeFromBinaryFile(GAME_MODE_FILE_NAME, gameMode);
	FileManager::ReadPlayerFromBinaryFile(PLAYER1_FILE_NAME, player1);
	FileManager::ReadPlayerFromBinaryFile(PLAYER2_FILE_NAME, player2);
	FileManager::ReadPlayerFromBinaryFile(BOT_FILE_NAME, bot);
}

void Game::Menu()
{
	switch (userInterface.GetCurrentMenuWindow())
	{
	case MenuWindow::MainMenu:
		userInterface.UpdateMainMenu();
		userInterface.RenderMainMenu();
		userInterface.HandleInputMainMenu(*this);
		break;

	case MenuWindow::Statistics:
	 	userInterface.RenderStatistics(player1, player2, bot);
		userInterface.HandleInputStatistics();
		break;

	case MenuWindow::Tutorial:
		userInterface.RenderTutorial();
		userInterface.HandleInputTutorial();
		break;

	case MenuWindow::Settings:
		userInterface.UpdateSettings();
		userInterface.RenderSettings();
		userInterface.HandleInputSettings();
		break;

	case MenuWindow::SettingsNameChange:
		userInterface.UpdateNameChangeSettings();
		userInterface.RenderNameChangeSettings();
		userInterface.HandleInputNameChangeSettings(player1, player2);
		break;

	case MenuWindow::SettingsGameMode:
		userInterface.UpdateGameModeSettings(*this);
		userInterface.RenderGameModeSettings();
		userInterface.HandleInputGameModeSettings(*this);
		break;

	default:
		throw GameException("Incorrect 'currentMenuWindow' value");
	}
}

void Game::StartGame(Player& p1, Player& p2)
{
	std::pair<bool, bool> RenderMode =
		(gameMode == GameMode::PlayerVsPlayer) ? std::make_pair(false, false) :
		(gameMode == GameMode::PlayerVsBot) ? std::make_pair(true, false) :
		std::make_pair(true, true);

	auto PlayMatch = [this](Player& shooter, bool modeShooter,   Player& target, bool modeTarget)
	{
		std::pair<ShotStatus, GameAction> shotStatus;
		for (;;)
		{
			shotStatus = shooter.Shot(target);

			if (shotStatus.second == GameAction::End)
				return MatchStatus::Cancelled;

			if (shotStatus.first == ShotStatus::NoneShot)
				continue;

			if (shotStatus.first == ShotStatus::Miss)
				return MatchStatus::InProgress;

				userInterface.RenderGameBoard(shooter, modeShooter, target, modeTarget);


			if (target.GetBoard().GetAliveShipsCount() == 0)
			{
				userInterface.RenderGameBoard(shooter, true, target, true);
				DeclareWinner(shooter, target);
				return MatchStatus::WinnerDeclared;
			}
		}
	};

	if (!p1.PlaceShips() || !p2.PlaceShips())
	{
		EndGame();
		return;
	}

	userInterface.RenderBattle();

	MatchStatus matchStatus;
	for (; isGameRunning ;)
	{
		userInterface.RenderGameBoard(p1, RenderMode.first, p2, RenderMode.second);
		if (matchStatus =  PlayMatch(p1, RenderMode.first, p2, RenderMode.second);
			matchStatus == MatchStatus::Cancelled || matchStatus == MatchStatus::WinnerDeclared)
		{
			break;
		}

		userInterface.RenderGameBoard(p1, RenderMode.first, p2, RenderMode.second);
		if (matchStatus = PlayMatch(p2, RenderMode.second, p1, RenderMode.first);
			matchStatus == MatchStatus::Cancelled || matchStatus == MatchStatus::WinnerDeclared)
		{
			break;
		}
	}

	EndGame();
}

void Game::EndGame() {
	player1.ResetAfterMatch();
	player2.ResetAfterMatch();
	bot.ResetAfterMatch();
	SaveData();
	SetIsGameRunning(false);
	userInterface.RestoreCurrentScreen();
}

void Game::DeclareWinner(Player& winner, Player& losses)
{
	userInterface.AnnounceWinner(winner.GetName());
	
	winner.UpdateWinnerStatus();
	losses.UpdateLoserStatus();

	Console::StopFor(2222);
	Console::PauseApplication();
}