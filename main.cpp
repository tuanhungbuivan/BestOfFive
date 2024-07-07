#include "main.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

// Constructor initializes SDL and the game board
BestOfFive::BestOfFive() : window(nullptr), renderer(nullptr), currentPlayer(PLAYER_X), isRunning(true), xWins(0), oWins(0) {
    initSDL();
    resetBoard();
}

// Destructor cleans up resources
BestOfFive::~BestOfFive() {
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}


// Initialize SDL components
void BestOfFive::initSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        exit(1);
    }

    if (TTF_Init() < 0) {
        std::cerr << "Failed to initialize SDL_ttf: " << TTF_GetError() << std::endl;
        exit(1);
    }

    // Create window and renderer
    window = SDL_CreateWindow("Best of Five", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
        exit(1);
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        std::cerr << "Failed to create renderer: " << SDL_GetError() << std::endl;
        exit(1);
    }

    // Load font
    font = TTF_OpenFont("/System/Library/Fonts/Supplemental/Arial.ttf", 24);
    if (!font) {
        std::cerr << "Failed to open font: " << TTF_GetError() << std::endl;
        exit(1);
    }
}

// Reset the game board to the initial state
void BestOfFive::resetBoard() {
    for (int row = 0; row < GRID_SIZE; ++row) {
        for (int col = 0; col < GRID_SIZE; ++col) {
            board[row][col] = NONE;
        }
    }
    currentPlayer = PLAYER_X; // X starts first
}

// Handle user input events
void BestOfFive::handleEvents() {
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            isRunning = false;
        } else if (event.type == SDL_MOUSEBUTTONDOWN) {
            int x, y;
            SDL_GetMouseState(&x, &y);
            int row = y / CELL_SIZE;
            int col = x / CELL_SIZE;

            // Check if the click is within the board
            if (row >= 0 && row < GRID_SIZE && col >= 0 && col < GRID_SIZE && board[row][col] == NONE) {
                board[row][col] = currentPlayer;
                if (checkWin()) {
                    if (currentPlayer == PLAYER_X) {
                        xWins++;
                    } else {
                        oWins++;
                    }
                    render(); // Update the display
                    showWinPopup(); // Show the win popup
                } else if (checkDraw()) {
                    resetBoard(); // Reset for a new game
                } else {
                    switchPlayer(); // Change turns
                }
            }
        }
    }
}

// Render the game board and pieces
void BestOfFive::render() {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    drawBoard();

    for (int row = 0; row < GRID_SIZE; ++row) {
        for (int col = 0; col < GRID_SIZE; ++col) {
            drawCell(row, col);
        }
    }

    SDL_RenderPresent(renderer);
}

// Draw the grid lines of the game board
void BestOfFive::drawBoard() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    for (int i = 1; i < GRID_SIZE; ++i) {
        drawLine(0, i * CELL_SIZE, WINDOW_WIDTH, i * CELL_SIZE);
        drawLine(i * CELL_SIZE, 0, i * CELL_SIZE, WINDOW_HEIGHT);
    }
}

// Draw X or O in a cell
void BestOfFive::drawCell(int row, int col) {
    int x = col * CELL_SIZE;
    int y = row * CELL_SIZE;

    if (board[row][col] == PLAYER_X) {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red for X
        int margin = 10;  // Adjust the margin as needed
        drawLine(x + margin, y + margin, x + CELL_SIZE - margin, y + CELL_SIZE - margin);
        drawLine(x + margin, y + CELL_SIZE - margin, x + CELL_SIZE - margin, y + margin);
    } else if (board[row][col] == PLAYER_O) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // Bluye for O
        int margin = 10;  // Adjust the margin as needed
        SDL_Rect rect = { x + margin, y + margin, CELL_SIZE - 2 * margin, CELL_SIZE - 2 * margin };
        SDL_RenderDrawRect(renderer, &rect);
    }
}

// Draw a line from (x1, y1) to (x2, y2)
void BestOfFive::drawLine(int x1, int y1, int x2, int y2) {
    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
}

// Check if the current player has won
bool BestOfFive::checkWin() {
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE - 4; ++j) {
            // Check rows
            if (board[i][j] != NONE && board[i][j] == board[i][j+1] && board[i][j] == board[i][j+2] && board[i][j] == board[i][j+3] && board[i][j] == board[i][j+4]) {
                return true;
            }
            // Check columns
            if (board[j][i] != NONE && board[j][i] == board[j+1][i] && board[j][i] == board[j+2][i] && board[j][i] == board[j+3][i] && board[j][i] == board[j+4][i]) {
                return true;
            }
        }
    }
    // Check diagonals
    for (int i = 0; i < GRID_SIZE - 4; ++i) {
        for (int j = 0; j < GRID_SIZE - 4; ++j) {
            if (board[i][j] != NONE && board[i][j] == board[i+1][j+1] && board[i][j] == board[i+2][j+2] && board[i][j] == board[i+3][j+3] && board[i][j] == board[i+4][j+4]) {
                return true;
            }
            if (board[i][j+4] != NONE && board[i][j+4] == board[i+1][j+3] && board[i][j+4] == board[i+2][j+2] && board[i][j+4] == board[i+3][j+1] && board[i][j+4] == board[i+4][j]) {
                return true;
            }
        }
    }
    return false;
}

// Check if the game is a draw
bool BestOfFive::checkDraw() {
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            if (board[i][j] == NONE) {
                return false;
            }
        }
    }
    return true;
}

// Switch to the other player
void BestOfFive::switchPlayer() {
    currentPlayer = (currentPlayer == PLAYER_X) ? PLAYER_O : PLAYER_X;
}

// Main game loop
void BestOfFive::run() {
    showMenu();
    while (isRunning) {
        handleEvents();
        render();
    }
}

// Render text to a texture
SDL_Texture* BestOfFive::renderText(const std::string& message, SDL_Color color, SDL_Renderer* renderer) {
    SDL_Surface* surface = TTF_RenderText_Solid(font, message.c_str(), color);
    if (!surface) {
        std::cerr << "Failed to create surface for text: " << TTF_GetError() << std::endl;
        return nullptr; // Handle error appropriately
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        std::cerr << "Failed to create texture from surface: " << SDL_GetError() << std::endl;
        SDL_FreeSurface(surface);
        return nullptr; // Handle error appropriately
    }

    SDL_FreeSurface(surface); // Free the surface after creating texture
    return texture;
}

void BestOfFive::renderTextAt(const std::string& message, SDL_Color color, int x, int y, SDL_Renderer* renderer) {
    SDL_Texture* texture = renderText(message, color, renderer);
    if (texture) {
        int textW = 0, textH = 0;
        SDL_QueryTexture(texture, NULL, NULL, &textW, &textH);
        SDL_Rect textRect = {x - textW / 2, y - textH / 2, textW, textH};
        SDL_RenderCopy(renderer, texture, NULL, &textRect);
        SDL_DestroyTexture(texture);
    }
}

// Show the main menu
void BestOfFive::showMenu() {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    SDL_Color black = { 0, 0, 0, 255 };

    // Define button dimensions and positions
    int buttonWidth = 220;
    int buttonHeight = 80;
    int buttonSpacing = 20;  // Space between buttons
    int totalHeight = (buttonHeight * 3) + (buttonSpacing * 2);
    int startY = (WINDOW_HEIGHT - totalHeight) / 2;

    // Draw "PLAY" button
    SDL_Rect playButton = { (WINDOW_WIDTH - buttonWidth) / 2, startY, buttonWidth, buttonHeight };
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &playButton);
    SDL_Texture* playText = renderText("PLAY", black, renderer);
    if (playText) {
        int textW = 0, textH = 0;
        SDL_QueryTexture(playText, NULL, NULL, &textW, &textH);
        SDL_Rect playTextRect = { playButton.x + (playButton.w - textW) / 2, playButton.y + (playButton.h - textH) / 2, textW, textH };
        SDL_RenderCopy(renderer, playText, NULL, &playTextRect);
        SDL_DestroyTexture(playText);
    } else {
        std::cerr << "Failed to render PLAY text!" << std::endl;
    }

    // Draw "INTRODUCTION" button
    SDL_Rect introButton = { (WINDOW_WIDTH - buttonWidth) / 2, startY + buttonHeight + buttonSpacing, buttonWidth, buttonHeight };
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &introButton);
    SDL_Texture* introText = renderText("INTRODUCTION", black, renderer);
    if (introText) {
        int textW = 0, textH = 0;
        SDL_QueryTexture(introText, NULL, NULL, &textW, &textH);
        SDL_Rect introTextRect = { introButton.x + (introButton.w - textW) / 2, introButton.y + (introButton.h - textH) / 2, textW, textH };
        SDL_RenderCopy(renderer, introText, NULL, &introTextRect);
        SDL_DestroyTexture(introText);
    } else {
        std::cerr << "Failed to render INTRODUCTION text!" << std::endl;
    }

    // Draw "LEAVE" button
    SDL_Rect leaveButton = { (WINDOW_WIDTH - buttonWidth) / 2, startY + (buttonHeight + buttonSpacing) * 2, buttonWidth, buttonHeight };
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &leaveButton);
    SDL_Texture* leaveText = renderText("LEAVE", black, renderer);
    if (leaveText) {
        int textW = 0, textH = 0;
        SDL_QueryTexture(leaveText, NULL, NULL, &textW, &textH);
        SDL_Rect leaveTextRect = { leaveButton.x + (leaveButton.w - textW) / 2, leaveButton.y + (leaveButton.h - textH) / 2, textW, textH };
        SDL_RenderCopy(renderer, leaveText, NULL, &leaveTextRect);
        SDL_DestroyTexture(leaveText);
    } else {
        std::cerr << "Failed to render LEAVE text!" << std::endl;
    }

    SDL_RenderPresent(renderer);

    bool inMenu = true;
    while (inMenu) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    isRunning = false;
                    inMenu = false;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    int x, y;
                    SDL_GetMouseState(&x, &y);

                    if (x >= playButton.x && x <= playButton.x + playButton.w &&
                        y >= playButton.y && y <= playButton.y + playButton.h) {
                        render();
                        inMenu = false;
                    } else if (x >= introButton.x && x <= introButton.x + introButton.w &&
                               y >= introButton.y && y <= introButton.y + introButton.h) {
                        std::cout << "Introduction button clicked!" << std::endl;
                        showIntroduction(); // Show introduction before starting the game
                        inMenu = false;
                    } else if (x >= leaveButton.x && x <= leaveButton.x + leaveButton.w &&
                               y >= leaveButton.y && y <= leaveButton.y + leaveButton.h) {
                        std::cout << "Leave button clicked!" << std::endl;
                        isRunning = false;
                        inMenu = false;
                    }
                    break;
                default:
                    break;
            }
        }
    }
}

// Show the introduction screen
void BestOfFive::showIntroduction() {
    std::cout << "Entered showIntroduction function!" << std::endl; // Log
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    // Draw introduction text block
    SDL_Rect textBlock = {50, 50, WINDOW_WIDTH - 100, WINDOW_HEIGHT - 200};
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &textBlock);

    // Read introduction text from file
    std::ifstream ifs("introduction_text.txt");
    if (!ifs) {
        std::cerr << "Failed to open introduction_text.txt" << std::endl;
        return; // Handle error appropriately
    }

    std::string introductionText;
    std::string line;
    std::vector<std::string> lines;

    while (std::getline(ifs, line)) {
        introductionText += line + "\n";
    }
    ifs.close();

    // Split the text into lines that fit within the width of the text block
    int maxWidth = textBlock.w - 20; // 10 pixels padding on each side
    std::stringstream ss(introductionText);
    while (std::getline(ss, line, '\n')) {
        lines.push_back(line);
    }

    SDL_Color black = {0, 0, 0, 255};
    int totalHeight = 0;
    std::vector<SDL_Texture*> textures;
    std::vector<int> lineHeights;
    std::vector<int> lineWidths;

    // Render each line to get its width and height
    for (size_t i = 0; i < lines.size(); ++i) {
        SDL_Surface* surface = TTF_RenderText_Blended_Wrapped(font, lines[i].c_str(), black, maxWidth);
        if (!surface) {
            std::cerr << "Failed to create surface for text: " << TTF_GetError() << std::endl;
            continue;
        }
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (!texture) {
            std::cerr << "Failed to create texture from surface: " << SDL_GetError() << std::endl;
            SDL_FreeSurface(surface);
            continue;
        }
        int textW = surface->w;
        int textH = surface->h;
        textures.push_back(texture);
        lineWidths.push_back(textW);
        lineHeights.push_back(textH);
        totalHeight += textH;
        SDL_FreeSurface(surface);
    }

    // Calculate starting y position to center the text vertically
    int startY = textBlock.y + (textBlock.h - totalHeight) / 2;

    // Render each line centered horizontally
    for (size_t i = 0; i < textures.size(); ++i) {
        int textX = textBlock.x + (textBlock.w - lineWidths[i]) / 2;
        int textY = startY;
        SDL_Rect textRect = {textX, textY, lineWidths[i], lineHeights[i]};
        SDL_RenderCopy(renderer, textures[i], NULL, &textRect);
        startY += lineHeights[i];
        SDL_DestroyTexture(textures[i]);
    }

    // Draw "RETURN" button below the text block
    int returnButtonY = textBlock.y + textBlock.h + 20; // 20 pixels below the text block
    SDL_Rect returnButton = {WINDOW_WIDTH / 2 - 100, returnButtonY, 200, 50};
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &returnButton);

    SDL_Texture* returnText = renderText("RETURN", black, renderer);  // Render "RETURN" text
    if (returnText) {
        int textW = 0, textH = 0;
        SDL_QueryTexture(returnText, NULL, NULL, &textW, &textH);
        SDL_Rect returnTextRect = {returnButton.x + (returnButton.w - textW) / 2, returnButton.y + (returnButton.h - textH) / 2, textW, textH};
        SDL_RenderCopy(renderer, returnText, NULL, &returnTextRect);
        SDL_DestroyTexture(returnText);
    } else {
        std::cerr << "Failed to render RETURN text!" << std::endl;
        // Handle error appropriately
    }

    SDL_RenderPresent(renderer);

    bool inIntro = true;
    while (inIntro && SDL_WaitEvent(&event)) {
        if (event.type == SDL_QUIT) {
            isRunning = false;
            inIntro = false;
        } else if (event.type == SDL_MOUSEBUTTONDOWN) {
            int x, y;
            SDL_GetMouseState(&x, &y);

            if (x >= returnButton.x && x <= returnButton.x + returnButton.w && y >= returnButton.y && y <= returnButton.y + returnButton.h) {
                // Return to the main menu
                showMenu();
                inIntro = false;
            }
        }
    }
}

// Show a popup when a player wins
void BestOfFive::showWinPopup() {
    render(); 

    SDL_Window* popupWindow = SDL_CreateWindow("Game Over", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 300, 200, SDL_WINDOW_SHOWN);
    SDL_Renderer* popupRenderer = SDL_CreateRenderer(popupWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    SDL_SetRenderDrawColor(popupRenderer, 255, 255, 255, 255);
    SDL_RenderClear(popupRenderer);

    SDL_Rect playAgainRect = {50, 50, 200, 50};
    SDL_SetRenderDrawColor(popupRenderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(popupRenderer, &playAgainRect);

    SDL_Color blackColor = {0, 0, 0, 255};
    renderTextAt("PLAY AGAIN", blackColor, playAgainRect.x + playAgainRect.w / 2, playAgainRect.y + playAgainRect.h / 2, popupRenderer);

    SDL_Rect quitRect = {50, 120, 200, 50};
    SDL_SetRenderDrawColor(popupRenderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(popupRenderer, &quitRect);

    renderTextAt("QUIT", blackColor, quitRect.x + quitRect.w / 2, quitRect.y + quitRect.h / 2, popupRenderer);

    SDL_RenderPresent(popupRenderer);

    bool inPopup = true;
    while (inPopup) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                isRunning = false;
                inPopup = false;
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                int x, y;
                SDL_GetMouseState(&x, &y);
                if (x >= playAgainRect.x && x <= playAgainRect.x + playAgainRect.w && y >= playAgainRect.y && y <= playAgainRect.y + playAgainRect.h) {
                    resetBoard();
                    inPopup = false;
                } else if (x >= quitRect.x && x <= quitRect.x + quitRect.w && y >= quitRect.y && y <= quitRect.y + quitRect.h) {
                    SDL_DestroyRenderer(popupRenderer);
                    SDL_DestroyWindow(popupWindow);
                    resetBoard();
                    showMenu();
                    return; // Exit the function after showing the menu
                }
            }
        }
    }

    SDL_DestroyRenderer(popupRenderer);
    SDL_DestroyWindow(popupWindow);
}

int main(int argc, char* argv[]) {
    BestOfFive game;
    game.run();
    return 0;
}

