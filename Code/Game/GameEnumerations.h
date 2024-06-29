#pragma once

enum class ShipIndex {
	Battleship = 0,
	Cruiser1,
	Cruiser2,
	Destroyer1,
	Destroyer2,
	Destroyer3,
	TorpedoBoat1,
	TorpedoBoat2,
	TorpedoBoat3,
	TorpedoBoat4,
	ShipCount,
	None = -1
};

enum class Coordinates {
	Player1NameX = 8,
	Player1NameY = 0,
	Player1BoardX = 0,
	Player1BoardY = 1,
	
	Player2NameX = 38,
	Player2NameY = 0,
	Player2BoardX = 30,
	Player2BoardY = 1
};

enum class Visibility {
	Hidden = true,
	Visible = false
};

enum class GameMode
{
	Esc = -1,
	PlayerVsPlayer,
	PlayerVsBot,
	BotVsBot
};

enum class BotDifficulty
{
	Esc = -1,
	Easy,
	Normal
};