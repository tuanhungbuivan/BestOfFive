# Best of Five

Best of Five is a game using SDL2 and SDL2_ttf. The game allows two players to compete in a "best of five" format (there is intruction/introduction to the game in the menu).

## Requirements

To run this project, you need to have the following installed on your system:
- [SDL2](https://www.libsdl.org/download-2.0.php)
- [SDL2_ttf](https://www.libsdl.org/projects/SDL_ttf/)

### macOS

1. Install SDL2 and SDL2_ttf using Homebrew:

    ```
    brew install sdl2 sdl2_ttf
    ```

2. Ensure that your compiler can find the SDL2 and SDL2_ttf headers and libraries. You may need to specify include and library paths when compiling. Move downloaded
   SDL2 and SDL2_ttf to /Library/Frameworks on macos.


## Build and Run

1. Clone the repository:

    ```
    git clone https://github.com/yourusername/BestOfFiveTicTacToe.git
    cd BestOfFive
    ```

2. Build and run:

    ```
    cmd + shift + b
    install_name_tool -add_rpath /Library/Frameworks main
    ./main
    ```

## Project Structure

- `main.hpp`: Header file containing declarations for the `BestOfFive` class and related constants.
- `main.cpp`: Main source file containing the implementation of the `BestOfFive` class.
- `introduction_text.txt`: Text file containing the introduction text displayed in the game.

## Controls

- Click on the "PLAY" button to start a new game.
- Click on the "INTRODUCTION" button to read the game introduction.
- Click on the "LEAVE" button to exit the game.
- During the game, click on the grid to place your marker (X or O).
- If a player wins or the game is a draw, a popup will appear with options to play again or quit.





