#pragma once

#include "GameEnumerations.h"

class Player;

class Game
{
public:

	Game();
	~Game();

	bool IsRunning() const;

	void RunGame();

private:

	void SetApplicationSettings();

	void StartGame();

	void Runder(Player& player1, Player& player2, Visibility visibilityPlayer1 = Visibility::Hidden, Visibility visibilityPlayer2 = Visibility::Hidden);

	void ShowWinner(const Player& player);

	void printGameRules();

	void Options();

	static GameMode gameMode;
	bool isRunning = true;
};