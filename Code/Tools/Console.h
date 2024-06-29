#pragma once
#include <string>
#include <Windows.h>

class Console
{
public:
	Console() = delete;

	enum class Color 
	{
		Blue = 1,
		Green = 2,
		Cyan = 3,
		Red = 4,
		Magenta = 5,
		LightBlue = 9,
		LightGreen = 10,
		LightCyan = 11,
		LightRed = 12,
		LightMagenta = 13,
		White = 15,
		Whiteandblue = 23
	};

	static void StopFor(unsigned milliseconds);
	static void ResetCursorPosition();
	static void SetCursorPosition(size_t x, size_t y);
	static void HideCursor();
	static void SetWindowSize(SHORT width, SHORT height);
	static void DisableScrollBars();
	static void SetTitle(const std::string& title);

	static void ClearScreen();
	static void PauseApplication();
	static void ChangColor(Color color);
private:
	static HANDLE consoleHandle;
	static HWND consoleWindow;
};