# Sea Battle Console Game

## Description

This project is a console-based implementation of the classic "Sea Battle" game. It provides an immersive and interactive gameplay experience in a text-based environment.

![Sea Battle](https://github.com/wak1zashi/Console-Sea-Battle/blob/main/Photo/0.png)

## Menu Controls

- **Up Arrow / W**: Move cursor to the previous menu item
- **Down Arrow / S**: Move cursor to the next menu item
- **Enter / E**: Select the current menu item
- **Escape / PgUp**: Return to the main menu

## Game Controls

- **Up Arrow / W**: Move the aiming cursor upward or move the ship when placing it on the grid
- **Down Arrow / S**: Move the aiming cursor downward or move the ship when placing it on the grid
- **Left Arrow / A**: Move the aiming cursor to the left or move the ship when placing it on the grid
- **Right Arrow / D**: Move the aiming cursor to the right or move the ship when placing it on the grid
- **Enter / E**: Place the ship on the grid or fire a shot during the game
- **Escape / PgUp**: Return to the main menu
- **Q / PgDn**: Rotate the ship horizontally or vertically

## How to Create and Compile This Project in Visual Studio

### Prerequisites
Ensure you have Visual Studio 2022 installed, as it supports the C++20 standard. Please follow the instructions on [how to install Visual Studio](https://visualstudio.microsoft.com/).

### Steps

1. **Download and Extract the Project Files**  
   Download the repository as a `.zip` file and extract it to your preferred location.  
   ![image](https://github.com/wak1zashi/Console-Sea-Battle/blob/main/Photo/1.png)

2. **Create a New C++ Project**  
   Open Visual Studio and create an empty C++ project.  
   ![image](https://github.com/wak1zashi/Console-Sea-Battle/blob/main/Photo/2.png)

3. **Add Project Files**  
   Open your project's folder in File Explorer and move the `Code`, `Data`, and `Menus` folders from the extracted archive into your project directory.  
   ![image](https://github.com/wak1zashi/Console-Sea-Battle/blob/main/Photo/3.png)

4. **Show All Files**  
   In Visual Studio, click on the "Show All Files" button in the Solution Explorer to make all files visible.  
   ![image](https://github.com/wak1zashi/Console-Sea-Battle/blob/main/Photo/4.png)

5. **Set C++20 Standard**  
   To ensure compatibility, change the project's C++ standard to C++20:
   - Right-click on your project in Solution Explorer and select **Properties**.
   - In the **Configuration Properties** tab, select **General**.
   - Set **C++ Language Standard** to **ISO C++20 Standard**.
   ![image](https://github.com/wak1zashi/Console-Sea-Battle/blob/main/Photo/5.png)

6. **Build and Run**  
   Click **Apply** to save the settings. To compile and launch the game, press **Ctrl + F5**.
