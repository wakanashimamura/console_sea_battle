#pragma once

#include<vector>
#include "Menu.h"
#include "Ship.h"
#include "../Tools/Console.h"
#include "GameEnumerations.h"

class Player;
class Ship;

class Board
{
public:

    struct ColorPalette 
    {
        Console::Color emptyColor;
        Console::Color shipColor;
        Console::Color missColor;
        Console::Color hitColor;
    };

    struct ColorPaletteWithPositions 
    {
        ColorPalette basePalette;
        Console::Color colorShipPositionCorrect;
        Console::Color colorShipPositionIncorrect;
    };

    enum class BoardSymbol
    {
        InvalidShot = '!',
        EmptySymbol = '-',
        ShipSymbol = '0',
        MissSymbol = '/',
        HitSymbol = 'X'
    };

    Board();

    void Show(size_t x = 0, size_t y = 0, bool Hidemode = false, int Xfire = 0, int Yfire = 0, bool fireMode = false, const ColorPalette& colorPalette = displayPalette) const;
    void Show(const Ship& ship, const ColorPaletteWithPositions& colorPalette = placeShipPalette) const;
   
    static bool Initialization(Player& player);

    static BoardSymbol Shot(int shotX, int shotY, Player& target);

    static void CheckAndSetAliveStatus(Player& player);
    static void MarkDestroyedShip(Player& player);

    static void AutoPlaceShips(Player& player);

    std::vector<int> FindEmptyColumns() const;
    std::vector<int> EmptyStrings(const int columns) const;

    static const size_t SIZE = 10;

private:
    
    void SetColorBasedOnSymbol(char symbol, const ColorPalette& colorPalette = displayPalette) const;
    void PrintIndexHeader() const;
    void Initialization(); 

    void SetShip(Ship& ship, bool isEdit = false);

    ShipIndex GetShipIndex(const Player& player, Menu::ShipMenuItem shipMenuItem) const;
    ShipIndex findUnplacedShip(const Player& player, int startIndex, int endIndex) const;

    void ToggleShipDirection(int& shipX, int& shipY, Ship::Direction& orientation, const Ship& ship);
    void UpdateShipProperties(int shipX, int shipY, Ship::Direction orientation, Ship& ship, bool isPlaced = true);
    void UpdateShipPositionInBoard(int& shipX, int& shipY, Ship::Direction& orientation, Ship& ship,  bool update = false);
    void SetSymbolInArea(const Ship& ship, BoardSymbol symbol);

    bool IsShipPositionValid(const Ship& ship, bool excludeCurrentShip = true) const;
    void CheckAndSetNewPosition(Ship& ship);

    void RemoveAllShips(Player& player);
    void RemoveShip(Ship& ship);

    bool IsSaveAllowed(const Player& player) const;
    
    void EditShip(Player& player);
    int FindFirstPlacedShipIndex(const Player& player) const;

    void MoveShipUp(int& shipY, const Ship& ship);
    void MoveShipLeft(int& shipX, const Ship& ship);
    void MoveShipRight(int& shipX, const Ship& ship);
    void MoveShipDown(int& shipY, const Ship& ship); 

    static ColorPalette displayPalette;
    static ColorPaletteWithPositions placeShipPalette;
    static ColorPaletteWithPositions editPalette;

    char GameBoard[SIZE][SIZE];
    char HiddenGameBoard[SIZE][SIZE];
};