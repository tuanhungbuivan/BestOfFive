#include <SDL2/SDL.h>
#include <SDL_ttf.h>
#include <iostream>

const int WINDOW_WIDTH = 900;
const int WINDOW_HEIGHT = 900;
const int GRID_SIZE = 20;
const int CELL_SIZE = WINDOW_WIDTH / GRID_SIZE;
const int LINE_WIDTH = 5;

enum Player { NONE = 0, PLAYER_X, PLAYER_O };

class BestOfFive {
public:
    BestOfFive();
    ~BestOfFive();
    void run();

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    TTF_Font* font;
    Player board[GRID_SIZE][GRID_SIZE];
    Player currentPlayer;
    bool isRunning;
    int xWins;
    int oWins;
    SDL_Event event;

    void initSDL();
    void handleEvents();
    void render();
    void resetBoard();
    void drawBoard();
    void drawCell(int row, int col);
    void drawLine(int x1, int y1, int x2, int y2);
    bool checkWin();
    bool checkDraw();
    void switchPlayer();
    void showMenu();
    void showIntroduction();
    void showWinPopup();
    void renderTextAt(const std::string& message, SDL_Color color, int x, int y, SDL_Renderer* renderer);
    SDL_Texture* renderText(const std::string& message, SDL_Color color, SDL_Renderer* renderer);
};
