#pragma once

#include <iostream>
#include "GameEnumerations.h"

class Menu
{
public:

	Menu() = delete;

	enum class MainMenuItem
	{
		Esc = -1,
		StartGame,
		Tutorials,
		Options,
		Exit
	};
	enum class ShipMenuItem
	{
		Esc = -1,
		BattleShip,
		Cruisers,
		Destroyers,
		TorpedoBoats,
		DeleteAll,
		Edit,
		Auto,
		Save
	};
	enum class OptionsMenuItem
	{
		Esc = -1,
		GameMode,
		BotDifficulty,
	};

	static MainMenuItem ShowMainMenuItem();
	static ShipMenuItem ShowShipMenuItem();
	static OptionsMenuItem ShowOptionsMenuItem();
	static GameMode ShowGameModeMenuItem();
	static BotDifficulty ShowBotDifficultyMenuItem();

	static void RestartActiveMenuItem();
private:

	template<typename T>
	static T ShowMenu(const std::string* MenuItem, size_t size, T& active_MenuItem, int x);

	static ShipMenuItem activeShipMenuItem;
};