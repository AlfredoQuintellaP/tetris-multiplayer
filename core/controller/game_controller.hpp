#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "../model/game_data.hpp"
#include "networking/network_manager.hpp"

/**
 * Main game controller handling input processing, game logic, and state management.
 * Implements the Controller component of the MVC architecture.
 */
class GameController {
public:
    /**
     * Constructs a GameController with reference to game data.
     * 
     * @param data Reference to the game data model
     */
    explicit GameController(GameData& data);
    
    /**
     * Processes all game events and dispatches to appropriate handlers.
     * 
     * @param event SFML event to process
     * @param window SFML render window
     */
    void handleEvent(const sf::Event& event, sf::RenderWindow& window);
    
    /**
     * Updates game logic based on elapsed time.
     * 
     * @param deltaTime Time elapsed since last update in seconds
     */
    void update(float deltaTime);
    
    // State transition methods
    void startSinglePlayer();
    void startMultiplayerLocal();
    void enterOnlineLobby();
    void hostOnlineGame();
    void joinOnlineGame(const std::string& ip);
    void returnToMainMenu();
    void pauseGame();
    void resumeGame();
    
    // Accessors
    GameData& getGameData() { return gameData; }
    const GameData& getGameData() const { return gameData; }
    NetworkManager& getNetworkManager() { return networkManager; }
    
private:
    // Event handlers for different game states
    void handleMainMenuEvents(const sf::Event& event, sf::RenderWindow& window);
    void handleSinglePlayerEvents(const sf::Event& event);
    void handleMultiplayerLocalEvents(const sf::Event& event);
    void handleOnlineLobbyEvents(const sf::Event& event, sf::RenderWindow& window);
    void handleMultiplayerOnlineEvents(const sf::Event& event);
    void handleGameOverEvents(const sf::Event& event, sf::RenderWindow& window);
    void handlePauseEvents(const sf::Event& event);
    
    // Game logic updates for different modes
    void updateSinglePlayer(float deltaTime);
    void updateMultiplayerLocal(float deltaTime);
    void updateMultiplayerOnline(float deltaTime);
    
    // Network operations
    void disconnectFromServer();
    void sendGameState();
    void receiveGameState();
    
    // Game over handling
    void gameOver();
    
    // Member variables
    GameData& gameData;           // Reference to game model
    NetworkManager networkManager; // Network communication handler
    sf::Clock gameClock;          // Main game timing
    
    // Multiplayer local timers (one per player)
    float player1FallTimer;
    float player2FallTimer;
};

#endif
