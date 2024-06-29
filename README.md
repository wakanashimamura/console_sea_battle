# Sea Battle Console Game

## Description

This project is a console-based implementation of the classic game "Sea Battle".

![Sea Battle](https://github.com/wak1zashi/Console-Sea-Battle/blob/main/Photo/0.png)

## Menu Controls

- **Up arrow**: Move cursor to the previous menu item
- **Down arrow**: Move cursor to the next menu item
- **Enter**: Select the current menu item
- **Escape**: Return to the main menu / Exit the game

## Game Controls

- **Up arrow**: Move targeting cursor up
- **Down arrow**: Move targeting cursor down
- **Left arrow**: Move targeting cursor left
- **Right arrow**: Move targeting cursor right
- **Enter**: Fire a shot
- **Escape**: Return to the main menu

## Game Modes

There are two modes in the game:

- **Easy mode**: The bot shoots randomly and does not attempt to sink a ship when it hits.
- **Normal mode**: The bot also shoots randomly but attempts to sink a ship when it hits.

## How to Create and Compile This Project in Visual Studio

NOTE: You need to have Visual Studio 2022 installed, which supports C++20 standard. Please search for instructions on how to install it.

1. Download this repository as a .zip file and extract it.<br />
![image](https://github.com/wak1zashi/Console-Sea-Battle/blob/main/Photo/1.png)

2. Create an empty C++ project in Visual Studio.<br />
![image](https://github.com/wak1zashi/Console-Sea-Battle/blob/main/Photo/2.png)

3. Open the project's folder in File Explorer and move the Code folder to it.<br />
![image](https://github.com/wak1zashi/Console-Sea-Battle/blob/main/Photo/3.png)

4. To make all those files visible in your project, click the button "Show all files" in Solution Explorer.<br />
![image](https://github.com/wak1zashi/Console-Sea-Battle/blob/main/Photo/4.png)

5. Let's change the project's standard to C++20. Go to the project's properties. Select "All Configurations" and "All Platforms". In "Configuration Properties" go to the tab "General", find "C++ Language Standard" and select "ISO C++20 Standard".<br />
![image](https://github.com/wak1zashi/Console-Sea-Battle/blob/main/Photo/5.png)

6. That's it! Press "Apply". To compile & launch the game, press "Ctrl + F5".
