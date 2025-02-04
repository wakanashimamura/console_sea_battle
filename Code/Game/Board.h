#pragma once

#include <array>
#include "../Tools/Console.h"
#include "Enumerations.h"
#include "Ship.h"
#include <string>
#include <unordered_map>


class Board
{
public:

    static constexpr size_t BOARD_SIZE = 10;

    using GameBoard = std::array<std::array<char, BOARD_SIZE>, BOARD_SIZE>; 
    using ShipMap = std::unordered_multimap<Ship::ShipSize, Ship>;

    enum class BoardSymbol
    {
        EmptySymbol = '-', // Empty cell
        ShipSymbol = '0',  // Ship
        MissSymbol = '/',  // Miss
        HitSymbol = 'X'    // Hit
    };

    struct Color
    {
        Console::Color emptyColor;
        Console::Color shipColor;
        Console::Color missColor;
        Console::Color hitColor;
    };

    Board();

    // ------------------------------------------------------------------------
    // -------------------------- BOARD SETUP METHODS -------------------------

    bool SetupPlayerShips(const std::string& playerName);
    void AutoPlaceShips();

    void ResetToDefaultState();

    void MarkShipAsDestroyed();
    void UpdateAllAliveShipsStatus();

    int GetAliveShipsCount() const;

    // ------------------------------------------------------------------------
    // -------------------------------- GETTERS -------------------------------

    std::vector<Vector2D> GetValidShotPositions() const;

    const GameBoard& GetGameBoard() const { return gameBoard; }
    const GameBoard& GetHiddenGameBoard() const { return hiddenGameBoard; }

    ShotStatus RegisterShot(Vector2D shot);

    static constexpr Color symbolColors = {
         Console::Color::LightBlue,
         Console::Color::Green,
         Console::Color::Cyan,
         Console::Color::Red
    };

private:

    // -------------------------------------------------------------------------
    // --------------------------- BOARD MANAGEMENT ----------------------------

    void InitializeBoard();

    Ship* FindShipBySize(Ship::ShipSize shipSize);

    void MoveShip(Ship& ship, Direction direction);

    void SetupShipBySize(Ship::ShipSize shipSize, const std::string& playerName);
    void SetupShip(Ship& ship, const std::string& playerName, bool editMode = false);

    std::pair<Vector2D, Vector2D> GetSurroundingArea(const Ship& ship) const;

    // -------------------------------------------------------------------------
    // ----------------------------- EDIT METHODS ------------------------------

    void EditMode(const std::string& playerName);
    void SwitchOrientation(Ship& ship);

    // -------------------------------------------------------------------------
    // -------------------------  POSITION VALIDATION --------------------------

    bool IsPositionFree(const Ship& ship) const;
    bool ValidateShipPosition(const Ship& ship) const;

    // -------------------------------------------------------------------------
    // -------------------------- SUPPORTIVE METHODS ---------------------------

    void UpdateShipPositionOnBoard(const Ship& ship, BoardSymbol boardSymbol = BoardSymbol::ShipSymbol);
   
    void RemoveShip(Ship& ship);
    void RemoveShips();

    bool CanSaveBoardState();

    void UpdateShipAliveStatus(Ship& ship);

    // -------------------------------------------------------------------------
    // --------------------------------- DATA ----------------------------------

    GameBoard gameBoard;
    GameBoard hiddenGameBoard;

    ShipMap ships =
    {
        {Ship::ShipSize::BattleShip, Ship(Ship::ShipSize::BattleShip)},

        {Ship::ShipSize::Cruisers, Ship(Ship::ShipSize::Cruisers)},
        {Ship::ShipSize::Cruisers, Ship(Ship::ShipSize::Cruisers)},

        {Ship::ShipSize::Destroyers, Ship(Ship::ShipSize::Destroyers)},
        {Ship::ShipSize::Destroyers, Ship(Ship::ShipSize::Destroyers)},
        {Ship::ShipSize::Destroyers, Ship(Ship::ShipSize::Destroyers)},

        {Ship::ShipSize::TorpedoBoats, Ship(Ship::ShipSize::TorpedoBoats)},
        {Ship::ShipSize::TorpedoBoats, Ship(Ship::ShipSize::TorpedoBoats)},
        {Ship::ShipSize::TorpedoBoats, Ship(Ship::ShipSize::TorpedoBoats)},
        {Ship::ShipSize::TorpedoBoats, Ship(Ship::ShipSize::TorpedoBoats)},
    };

    static constexpr Vector2D defaultPositionGameBoard { 27, 11 };
};