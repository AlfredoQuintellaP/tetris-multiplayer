#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include "classic_board.hpp"
#include "../rendering/board_renderer.hpp"
#include "../utils/game_state.hpp"
#include "../networking/network_manager.hpp"

enum class OnlineMenuOption {
    HOST_GAME,
    JOIN_GAME,
    BACK,
    COUNT
};

class Game {
private:
    sf::RenderWindow window;
    ClassicBoard board;
    BoardRenderer renderer;
    sf::Clock gameClock;
    sf::Clock fallClock;
    float fallSpeed;
    sf::Font font;
    
    // Game state management
    GameState currentState;
    MenuOption selectedMenuOption;
    OnlineMenuOption selectedOnlineOption;
    
    // Multiplayer local
    ClassicBoard board2;
    BoardRenderer renderer2;
    
    // Multiplayer online
    NetworkManager network;
    std::string inputIP;
    bool inputtingIP;
    
public:
    Game();
    void run();
    void handleEvents();
    void update();
    void render();
    
private:
    // Menu functions
    void handleMenuEvents(sf::Event& event);
    void handleOnlineLobbyEvents(sf::Event& event);
    void renderMainMenu();
    void renderOnlineLobby();
    
    // Single player functions
    void handleSinglePlayerEvents(sf::Event& event);
    void updateSinglePlayer();
    void renderSinglePlayer();
    void drawUI();
    void drawPanel(float x, float y, float width, float height, const std::string& title);
    void drawNextPiece();
    void drawHoldPiece();
    void drawGameOver();
    
    // Multiplayer local functions
    void handleMultiplayerLocalEvents(sf::Event& event);
    void updateMultiplayerLocal();
    void renderMultiplayerLocal();
    void drawPlayerUI(const ClassicBoard& playerBoard, const BoardRenderer& playerRenderer, 
                      float offsetX, const std::string& playerName);
    
    // Multiplayer online functions
    void handleMultiplayerOnlineEvents(sf::Event& event);
    void updateMultiplayerOnline();
    void renderMultiplayerOnline();
    void sendGameState();
    void receiveGameState();
};

#endif
