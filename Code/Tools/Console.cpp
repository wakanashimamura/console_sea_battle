#include "Console.h"
#include <iostream>

HANDLE Console::consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
HWND Console::consoleWindow = GetConsoleWindow();

void Console::StopFor(unsigned milliseconds)
{
	Sleep(milliseconds);
}

void Console::ResetCursorPosition()
{
	SetCursorPosition(0, 0);
}

void Console::SetCursorPosition(size_t x, size_t y)
{
	std::cout.flush();
	COORD newCursorPosition = { static_cast<SHORT>(x), static_cast<SHORT>(y) };
	SetConsoleCursorPosition(consoleHandle, newCursorPosition);
}

void Console::HideCursor()
{
	CONSOLE_CURSOR_INFO cursorInfo;
	cursorInfo.dwSize = 100;
	cursorInfo.bVisible = FALSE;
	SetConsoleCursorInfo(consoleHandle, &cursorInfo);
}

void Console::SetWindowSize(SHORT width, SHORT height)
{
	SMALL_RECT windowArea = { 0, 0, width, height };

	SetConsoleWindowInfo(consoleHandle, TRUE, &windowArea);
}

void Console::DisableScrollBars()
{
	CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
	GetConsoleScreenBufferInfo(consoleHandle, &bufferInfo);
	COORD newBuffetSize =
	{
		bufferInfo.srWindow.Right - bufferInfo.srWindow.Left + 1,
		bufferInfo.srWindow.Bottom - bufferInfo.srWindow.Top + 1
	};

	SetConsoleScreenBufferSize(consoleHandle, newBuffetSize);
}

void Console::SetTitle(const std::string& title)
{
	SetConsoleTitleA(title.c_str());
}

void Console::ClearScreen()
{
	system("cls");
}

void Console::PauseApplication()
{
	system("pause");
}

void Console::ChangColor(Color color)
{
	SetConsoleTextAttribute(consoleHandle, static_cast<int>(color));
}