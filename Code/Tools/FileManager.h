#pragma once

#include <string>
#include "../Game/Enumerations.h"

class Player;

class FileManager
{
public:

	FileManager() = delete;

	static bool ReadTextFromFile(const std::string& fileName, std::string& content);

	static bool ReadPlayerFromBinaryFile(const std::string& fileName, Player& player);

	static bool ReadGameModeFromBinaryFile(const std::string& fileName, GameMode& player);

	static bool	WriteTextInFile(const std::string& fileName, std::string& content);

	static bool	WritePlayerInBinaryFile(const std::string& fileName, const Player& player);

	static bool WriteGameModeInBinaryFile(const std::string& fileName, const GameMode& gameMode);
};