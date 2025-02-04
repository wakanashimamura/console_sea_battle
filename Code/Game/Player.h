#pragma once

#include "Board.h"
#include "Statistics.h"
#include <string>

class Player
{
public:

    static constexpr int MAX_USERNAME_LENGTH = 17;

    Player(Vector2D boardPosition, const std::string name = "Player");

    virtual std::pair<ShotStatus, GameAction> Shot(Player& target);

    virtual bool PlaceShips();

    void ChangeName(const std::string& name);
    void ResetAfterMatch();

    void UpdateWinnerStatus();
    void UpdateLoserStatus();

    void Write(std::ofstream& fileStream) const;
    void Read(std::ifstream& fileStream);

    ShotStatus ShootAtBoard(const Vector2D& position) { return boardGame.RegisterShot(position); }

    const std::string& GetName() const { return name; }
    const Statistics& GetStatistics() const { return statistics; }
    const Board& GetBoard() const { return boardGame; }
    const Vector2D& GetBoardPosition() const { return boardPosition; }

    static constexpr int MR = 50;

protected:

    static constexpr Vector2D DefaultCrosshairPosition{ 5, 5 };

    std::string name;
    Statistics statistics;
    Board boardGame;

    Vector2D boardPosition;
    Vector2D crosshair;
};