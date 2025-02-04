#pragma once

enum class GameMode
{
	PlayerVsPlayer,
	PlayerVsBot,
};

enum class ShotStatus
{
	Hit,
	Miss,
	NoneShot
};

enum class MatchStatus
{
	Cancelled,
	WinnerDeclared,
	InProgress  
};

enum class GameAction
{
	Continue,
	End
};

enum class Direction
{
	Up,
	Left,
	Right,
	Down
};

enum class RenderingMode {
	Hidden,
	Visible
};

enum class Screen
{
	Logo,
	Menu,
	Game
};

enum class MenuWindow
{
	MainMenu,
	Statistics,
	Settings,
	SettingsNameChange,
	SettingsGameMode,
	Tutorial
};

enum class MainMenuItem
{
	StartGame,
	Statistics,
	Tutorials,
	Settings,
	Exit
};

enum class ShipsSetupMenuItem
{
	BattleShip,
	Cruisers,
	Destroyers,
	TorpedoBoats,
	RemoveShips,
	Edit,
	Auto,
	Save,
	Exit
};

enum class SettingsMenuItem
{
	NameChange,
	GameMode,
	Exit
};

enum class SettingsNameChangeMenuItem
{
	Player1,
	Player2,
	Exit
};

enum class SettingsGameModeMenuItem
{
	PlayerVsPlayer,
	PlayerVsBot,
	Exit
};

template<typename MenuItemType>
void ChangeMenuItemUp(MenuItemType& item, MenuItemType firstItem, MenuItemType lastItem)
{
	item = (item == firstItem) ? lastItem : MenuItemType(static_cast<int>(item) - 1);
}

template<typename MenuItemType>
void ChangeMenuItemDown(MenuItemType& item, MenuItemType firstItem, MenuItemType lastItem)
{
	item = (item == lastItem) ? firstItem : MenuItemType(static_cast<int>(item) + 1);
}