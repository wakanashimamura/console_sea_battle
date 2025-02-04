#pragma once

class InputManager
{
public:

	InputManager() = delete;

	enum class Key
	{
		None = -1,
		Tab,
		Enter,
		Escape,
		ArrowUp,
		ArrowLeft,
		ArrowRight,
		ArrowDown,
		PageDown,
		PageUp,
		Delete,
		A,
		D,
		E,
		Q,
		S,
		W
	};

	static void ClearBuffer();
	static bool UserPressedKey();
	static Key PressedKey();
	static int Getch();

private:
	static Key pressedKey;
};