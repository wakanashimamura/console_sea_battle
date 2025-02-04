#pragma once

#include "Player.h"
#include "Bot.h"
#include "UserInterface.h"

class Game
{
public:

	Game();
	
	void LoadGame();

	void SetIsLoadGame(bool isLoadGame) { this->isLoadGame = isLoadGame; }
	void SetIsGameRunning(bool isGameRunning) { this->isGameRunning = isGameRunning; }
	void SetGameMode(GameMode gameMode) { this->gameMode = gameMode; }

	bool GetIsLoadGame() const { return isLoadGame; }
	bool GetIsGameRunning() const { return isGameRunning; }
	GameMode GetGameMode() const { return gameMode; }
	void SaveData() const;
	void ReadData();

	static const std::string PLAYER1_FILE_NAME;
	static const std::string PLAYER2_FILE_NAME;
	static const std::string BOT_FILE_NAME;
	static const std::string GAME_MODE_FILE_NAME;

private:

	void Menu();
	void StartGame(Player& p1, Player& p2);

	void EndGame();

	void DeclareWinner(Player& winner, Player& losses);

	bool isLoadGame;
	bool isGameRunning;
	GameMode gameMode;

	Player player1;
	Player player2;
	Bot bot;
	UserInterface& userInterface;
};