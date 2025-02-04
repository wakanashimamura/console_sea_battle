#include "InputManager.h"
#include <conio.h>
#include "Hash.h"
#include <unordered_map>

InputManager::Key InputManager::pressedKey = Key::None;

void InputManager::ClearBuffer()
{
	for (; UserPressedKey();)
	{
		(void)_getch();
	}
}

bool InputManager::UserPressedKey()
{
	return _kbhit();
}

InputManager::Key InputManager::PressedKey()
{
	// _getch() returns two values if the navigation key is pressed.
	// The first value is '224', the second is the actual key code.
	// In unordered_map, the key is std::pair, where:
	// - the first value of the pair is the key code,
	// - the second value of the pair indicates whether the navigation key was pressed (true if pressed, false otherwise).
	// Since the standard hash function does not work for std::pair, you need to use your own hash function (pair_hash).

	static const std::unordered_map <std::pair<short, bool>, Key, pair_hash> keyMap
	{
		{{9,   false}, Key::Tab},
		{{13,  false}, Key::Enter},
		{{27,  false}, Key::Escape},
		{{72,  true},  Key::ArrowUp},    
		{{75,  true},  Key::ArrowLeft},
		{{77,  true},  Key::ArrowRight},
		{{80,  true},  Key::ArrowDown},
		{{81,  true},  Key::PageDown},
		{{73,  true},  Key::PageUp},
		{{83,  true},  Key::Delete},
		{{65,  false}, Key::A},
		{{68,  false}, Key::D},
		{{69,  false}, Key::E},
		{{81,  false}, Key::Q},
		{{83,  false}, Key::S},
		{{97,  false}, Key::A},
		{{100, false}, Key::D},
		{{101, false}, Key::E},
		{{113, false}, Key::Q},
		{{115, false}, Key::S},
		{{119, false}, Key::W},
	};

	std::pair<short, bool> asciiCode;
	asciiCode.first = Getch();

	if (asciiCode.first == 224)
	{
		asciiCode.second = true;
		asciiCode.first = Getch();
	}

	auto it = keyMap.find(asciiCode);
	return it != keyMap.end() ? it->second : Key::None;
}

int InputManager::Getch()
{
	return _getch();
}