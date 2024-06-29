#include "InputManager.h"
#include <conio.h>

InputManager::Key InputManager::pressedKey = Key::None;

bool InputManager::UserPressedKey()
{
	return _kbhit();
}

InputManager::Key InputManager::PressedKey()
{
	pressedKey = static_cast<Key>(_getch());

	if(pressedKey == Key::Arrow)
		pressedKey = static_cast<Key>(_getch());

	return pressedKey;
}