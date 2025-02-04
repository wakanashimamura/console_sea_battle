#include "Player.h"

#include "../Tools/GameException.h"
#include "UserInterface.h"

Player::Player(Vector2D boardPosition, const std::string name)
	: boardPosition(boardPosition), crosshair(DefaultCrosshairPosition)
{
	ChangeName(name);
}

std::pair<ShotStatus, GameAction> Player::Shot(Player& target)
{
	UserInterface& userInterface = UserInterface::GetObjectWindow();
	for (;;)
	{
		userInterface.RenderGameBoard(crosshair, target.GetBoard().GetHiddenGameBoard(), Board::symbolColors, target.GetName(), target.GetBoardPosition());
		InputManager::Key key = userInterface.MoveCrosshair(crosshair);
		if (key == InputManager::Key::Enter || key == InputManager::Key::E)
		{
			return { target.ShootAtBoard(crosshair), GameAction::Continue };
		}
		else if (key == InputManager::Key::Escape || key == InputManager::Key::PageUp)
		{
			return { ShotStatus::NoneShot, GameAction::End};
		}
	}
}

bool Player::PlaceShips()
{
	return boardGame.SetupPlayerShips(GetName());
}

void Player::ChangeName(const std::string& name)
{
	if (name.length() > MAX_USERNAME_LENGTH)
	{
		throw GameException("Your username is too long! maximum number of characters " + std::to_string(MAX_USERNAME_LENGTH));
	}

	this->name = name;
}

void Player::ResetAfterMatch()
{
	boardGame.ResetToDefaultState();
	crosshair = { 5, 5 };
}

void Player::UpdateWinnerStatus()
{
	statistics.UpdateWinnerStatus(MR);
}

void Player::UpdateLoserStatus()
{
	statistics.UpdateLoserStatus(MR);
}

void Player::Write(std::ofstream& fileStream) const
{
	// - Write Name -
	std::string::size_type nameLength = name.length();
	fileStream.write(reinterpret_cast<const char*>(&nameLength), sizeof(nameLength));
	fileStream.write(name.c_str(), nameLength);

	// - Write Statistics -
	statistics.Write(fileStream);
}

void Player::Read(std::ifstream& fileStream)
{
	// - Read Name -
	std::string::size_type nameLength;
	fileStream.read(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));
	name.resize(nameLength);
	fileStream.read(&name[0], nameLength);

	// - Read Statistics -
	statistics.Read(fileStream);
}