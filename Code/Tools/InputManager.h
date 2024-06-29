#pragma once
class InputManager
{
public:
	InputManager() = delete;

	enum class Key
	{
		PgDn = 81,
		Enter = 13,
		Escape = 27,
		ArrowUp = 72,
		ArrowLeft = 75,
		ArrowRight = 77,
		ArrowDown = 80,
		Arrow = 224,
		None = -1
	};

	static bool UserPressedKey();
	static Key PressedKey();
private:
	static Key pressedKey;
};