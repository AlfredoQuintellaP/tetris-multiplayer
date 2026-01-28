#include "game_controller.hpp"
#include <iostream>
#include <algorithm>

/**
 * Constructs GameController and initializes timers.
 */
GameController::GameController(GameData& data) 
    : gameData(data), 
      networkManager(),
      gameClock(),
      player1FallTimer(0.0f), 
      player2FallTimer(0.0f) {
}

// ======================
// MAIN EVENT DISPATCH
// ======================

/**
 * Routes events to appropriate handlers based on current game state.
 */
void GameController::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
    if (event.type == sf::Event::Closed) {
        window.close();
        return;
    }
    
    switch (gameData.getCurrentState()) {
        case GameState::MAIN_MENU:
            handleMainMenuEvents(event, window);
            break;
            
        case GameState::SINGLE_PLAYER:
            handleSinglePlayerEvents(event);
            break;
            
        case GameState::MULTIPLAYER_LOCAL:
            handleMultiplayerLocalEvents(event);
            break;
            
        case GameState::MULTIPLAYER_ONLINE_LOBBY:
            handleOnlineLobbyEvents(event, window);
            break;
            
        case GameState::MULTIPLAYER_ONLINE_GAME:
            handleMultiplayerOnlineEvents(event);
            break;
            
        case GameState::GAME_OVER:
            handleGameOverEvents(event, window);
            break;
            
        case GameState::PAUSED:
            handlePauseEvents(event);
            break;
            
        default:
            break;
    }
}

// ======================
// STATE EVENT HANDLERS
// ======================

/**
 * Handles main menu navigation and selection.
 */
void GameController::handleMainMenuEvents(const sf::Event& event, sf::RenderWindow& window) {
    if (event.type == sf::Event::KeyPressed) {
        switch (event.key.code) {
            case sf::Keyboard::Up:
                gameData.moveMenuUp();
                break;
                
            case sf::Keyboard::Down:
                gameData.moveMenuDown();
                break;
                
            case sf::Keyboard::Enter:
            case sf::Keyboard::Space:
                switch (gameData.getSelectedMenuOption()) {
                    case MenuOption::SINGLE_PLAYER:
                        startSinglePlayer();
                        break;
                        
                    case MenuOption::MULTIPLAYER_LOCAL:
                        startMultiplayerLocal();
                        break;
                        
                    case MenuOption::MULTIPLAYER_ONLINE:
                        enterOnlineLobby();
                        break;
                        
                    case MenuOption::EXIT:
                        window.close();
                        break;
                        
                    default:
                        break;
                }
                break;
                
            case sf::Keyboard::Escape:
                window.close();
                break;
                
            default:
                break;
        }
    }
}

/**
 * Handles single player game controls.
 */
void GameController::handleSinglePlayerEvents(const sf::Event& event) {
    if (gameData.isPausedState()) return;
    
    if (event.type == sf::Event::KeyPressed) {
        auto& board = gameData.getPlayerBoard(0);
        
        switch (event.key.code) {
            case sf::Keyboard::Left:    board.moveLeft(); break;
            case sf::Keyboard::Right:   board.moveRight(); break;
            case sf::Keyboard::Down:    board.moveDown(); break;
            case sf::Keyboard::Up:      board.rotate(); break;
            case sf::Keyboard::Space:   board.hardDrop(); break;
            case sf::Keyboard::C:       board.hold(); break;
            case sf::Keyboard::R:       startSinglePlayer(); break; // Restart
            case sf::Keyboard::P:       pauseGame(); break; // Pause
            case sf::Keyboard::Escape:  returnToMainMenu(); break;
            default: break;
        }
    }
}

/**
 * Handles local multiplayer controls for both players.
 */
void GameController::handleMultiplayerLocalEvents(const sf::Event& event) {
    if (gameData.isPausedState()) return;
    
    if (event.type == sf::Event::KeyPressed) {
        // Player 1 controls (arrow keys)
        auto& board1 = gameData.getPlayerBoard(0);
        switch (event.key.code) {
            case sf::Keyboard::Left:    board1.moveLeft(); break;
            case sf::Keyboard::Right:   board1.moveRight(); break;
            case sf::Keyboard::Down:    board1.moveDown(); break;
            case sf::Keyboard::Up:      board1.rotate(); break;
            case sf::Keyboard::RShift:  board1.hardDrop(); break;
            case sf::Keyboard::RAlt:    board1.hold(); break;
            default: break;
        }
        
        // Player 2 controls (WASD keys)
        auto& board2 = gameData.getPlayerBoard(1);
        switch (event.key.code) {
            case sf::Keyboard::A:       board2.moveLeft(); break;
            case sf::Keyboard::D:       board2.moveRight(); break;
            case sf::Keyboard::S:       board2.moveDown(); break;
            case sf::Keyboard::W:       board2.rotate(); break;
            case sf::Keyboard::LShift:  board2.hardDrop(); break;
            case sf::Keyboard::LControl: board2.hold(); break;
            case sf::Keyboard::R:       startMultiplayerLocal(); break; // Restart
            case sf::Keyboard::P:       pauseGame(); break; // Pause
            case sf::Keyboard::Escape:  returnToMainMenu(); break;
            default: break;
        }
    }
}

/**
 * Handles online lobby menu and IP input.
 */
void GameController::handleOnlineLobbyEvents(const sf::Event& event, sf::RenderWindow& window) {
    if (event.type == sf::Event::KeyPressed) {
        if (gameData.isInputtingIP()) {
            // IP input handling
            if (event.key.code == sf::Keyboard::Enter) {
                if (!gameData.getInputIP().empty()) {
                    joinOnlineGame(gameData.getInputIP());
                    gameData.setInputtingIP(false);
                }
            } else if (event.key.code == sf::Keyboard::Escape) {
                gameData.clearInputIP();
                gameData.setInputtingIP(false);
            } else if (event.key.code == sf::Keyboard::BackSpace) {
                gameData.backspaceInputIP();
            }
        } else {
            // Menu navigation
            switch (event.key.code) {
                case sf::Keyboard::Up:
                    gameData.moveOnlineMenuUp();
                    break;
                    
                case sf::Keyboard::Down:
                    gameData.moveOnlineMenuDown();
                    break;
                    
                case sf::Keyboard::Enter:
                case sf::Keyboard::Space:
                    switch (gameData.getSelectedOnlineOption()) {
                        case OnlineMenuOption::HOST_GAME:
                            hostOnlineGame();
                            break;
                            
                        case OnlineMenuOption::JOIN_GAME:
                            gameData.setInputtingIP(true);
                            gameData.clearInputIP();
                            break;
                            
                        case OnlineMenuOption::BACK:
                            returnToMainMenu();
                            break;
                            
                        default:
                            break;
                    }
                    break;
                    
                case sf::Keyboard::Escape:
                    returnToMainMenu();
                    break;
                    
                default:
                    break;
            }
        }
    } else if (event.type == sf::Event::TextEntered && gameData.isInputtingIP()) {
        // IP address character input
        if (event.text.unicode < 128 && event.text.unicode != 8 && event.text.unicode != 13) {
            char c = static_cast<char>(event.text.unicode);
            if ((c >= '0' && c <= '9') || c == '.') {
                gameData.appendToInputIP(c);
            }
        }
    }
    
    // Check for client connections if hosting
    if (gameData.isHosting() && !gameData.isConnected()) {
        if (networkManager.waitForClient()) {
            gameData.setConnected(true);
            std::cout << "Client connected!" << std::endl;
            
            // Start online game
            gameData.transitionToState(GameState::MULTIPLAYER_ONLINE_GAME);
            gameData.getPlayerBoard(0).reset();
            gameData.getPlayerBoard(0).spawnPiece();
        }
    }
}

/**
 * Handles online multiplayer controls and network communication.
 */
void GameController::handleMultiplayerOnlineEvents(const sf::Event& event) {
    if (gameData.isPausedState()) return;
    
    if (event.type == sf::Event::KeyPressed) {
        NetworkMessage msg;
        msg.playerID = networkManager.getIsHost() ? 1 : 2;
        
        auto& board = gameData.getPlayerBoard(0);
        
        switch (event.key.code) {
            case sf::Keyboard::Left:
                board.moveLeft();
                msg.type = NetworkMessageType::MOVE_LEFT;
                networkManager.sendMessage(msg);
                break;
                
            case sf::Keyboard::Right:
                board.moveRight();
                msg.type = NetworkMessageType::MOVE_RIGHT;
                networkManager.sendMessage(msg);
                break;
                
            case sf::Keyboard::Down:
                board.moveDown();
                msg.type = NetworkMessageType::MOVE_DOWN;
                networkManager.sendMessage(msg);
                break;
                
            case sf::Keyboard::Up:
                board.rotate();
                msg.type = NetworkMessageType::ROTATE;
                networkManager.sendMessage(msg);
                break;
                
            case sf::Keyboard::Space:
                board.hardDrop();
                msg.type = NetworkMessageType::HARD_DROP;
                networkManager.sendMessage(msg);
                break;
                
            case sf::Keyboard::C:
                board.hold();
                msg.type = NetworkMessageType::HOLD;
                networkManager.sendMessage(msg);
                break;
                
            case sf::Keyboard::R:
                board.reset();
                board.spawnPiece();
                break;
                
            case sf::Keyboard::P:
                pauseGame();
                break;
                
            case sf::Keyboard::Escape:
                networkManager.disconnect();
                returnToMainMenu();
                break;
                
            default:
                break;
        }
    }
}

/**
 * Handles game over screen input (restart or exit).
 */
void GameController::handleGameOverEvents(const sf::Event& event, sf::RenderWindow& window) {
    if (event.type == sf::Event::KeyPressed) {
        switch (event.key.code) {
            case sf::Keyboard::R:
                // Restart based on previous game mode
                if (gameData.getPreviousState() == GameState::SINGLE_PLAYER) {
                    startSinglePlayer();
                } else if (gameData.getPreviousState() == GameState::MULTIPLAYER_LOCAL) {
                    startMultiplayerLocal();
                } else if (gameData.getPreviousState() == GameState::MULTIPLAYER_ONLINE_GAME) {
                    // Reset online game
                    gameData.getPlayerBoard(0).reset();
                    gameData.getPlayerBoard(0).spawnPiece();
                    gameData.setGameOver(false);
                }
                break;
                
            case sf::Keyboard::Escape:
                returnToMainMenu();
                break;
                
            case sf::Keyboard::Enter:
            case sf::Keyboard::Space:
                // Restart single player or local multiplayer
                if (gameData.getPreviousState() == GameState::SINGLE_PLAYER) {
                    startSinglePlayer();
                } else if (gameData.getPreviousState() == GameState::MULTIPLAYER_LOCAL) {
                    startMultiplayerLocal();
                }
                break;
                
            default:
                break;
        }
    }
}

/**
 * Handles pause screen input.
 */
void GameController::handlePauseEvents(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        switch (event.key.code) {
            case sf::Keyboard::P:
                resumeGame();
                break;
                
            case sf::Keyboard::Escape:
                returnToMainMenu();
                break;
                
            default:
                break;
        }
    }
}

// ======================
// STATE TRANSITIONS
// ======================

/**
 * Initializes and starts single player mode.
 */
void GameController::startSinglePlayer() {
    gameData.transitionToState(GameState::SINGLE_PLAYER);
    gameData.getPlayerBoard(0).reset();
    gameData.getPlayerBoard(0).spawnPiece();
    gameData.setGameOver(false);
    gameData.setPaused(false);
    gameData.setTimeSinceLastFall(0.0f);
    std::cout << "Starting Single Player mode" << std::endl;
}

/**
 * Initializes and starts local multiplayer mode.
 */
void GameController::startMultiplayerLocal() {
    gameData.transitionToState(GameState::MULTIPLAYER_LOCAL);
    gameData.getPlayerBoard(0).reset();
    gameData.getPlayerBoard(1).reset();
    gameData.getPlayerBoard(0).spawnPiece();
    gameData.getPlayerBoard(1).spawnPiece();
    gameData.setGameOver(false);
    gameData.setPaused(false);
    player1FallTimer = 0.0f;
    player2FallTimer = 0.0f;
    std::cout << "Starting Local Multiplayer mode" << std::endl;
}

/**
 * Transitions to online multiplayer lobby.
 */
void GameController::enterOnlineLobby() {
    gameData.transitionToState(GameState::MULTIPLAYER_ONLINE_LOBBY);
    gameData.setInputtingIP(false);
    gameData.clearInputIP();
    std::cout << "Entering Online Lobby" << std::endl;
}

/**
 * Starts hosting an online game server.
 */
void GameController::hostOnlineGame() {
    std::cout << "Starting server..." << std::endl;
    
    if (networkManager.startServer()) {
        gameData.setHosting(true);
        std::cout << "Server started. Waiting for opponent..." << std::endl;
    } else {
        std::cerr << "Failed to start server" << std::endl;
    }
}

/**
 * Joins an online game at specified IP address.
 */
void GameController::joinOnlineGame(const std::string& ip) {
    std::cout << "Attempting to connect to " << ip << ":54000" << std::endl;
    
    if (networkManager.connectToServer(ip, 54000)) {
        gameData.setConnected(true);
        gameData.setHosting(false);
        std::cout << "Connected to server!" << std::endl;
        
        // Start online game
        gameData.transitionToState(GameState::MULTIPLAYER_ONLINE_GAME);
        gameData.getPlayerBoard(0).reset();
        gameData.getPlayerBoard(0).spawnPiece();
        gameData.setGameOver(false);
        gameData.setPaused(false);
        gameData.setTimeSinceLastFall(0.0f);
    } else {
        std::cerr << "Error: Could not connect to server" << std::endl;
        gameData.setInputIP("");
    }
}

/**
 * Returns to main menu, cleaning up network connections.
 */
void GameController::returnToMainMenu() {
    if (gameData.isConnected()) {
        disconnectFromServer();
    }
    
    gameData.setPaused(false);
    gameData.setGameOver(false);
    gameData.transitionToState(GameState::MAIN_MENU);
    std::cout << "Returning to Main Menu" << std::endl;
}

/**
 * Pauses the current game.
 */
void GameController::pauseGame() {
    if (gameData.isInGame() && !gameData.isGameOver()) {
        gameData.setPaused(true);
        std::cout << "Game Paused" << std::endl;
    }
}

/**
 * Resumes a paused game.
 */
void GameController::resumeGame() {
    if (gameData.isPausedState()) {
        gameData.setPaused(false);
        std::cout << "Game Resumed" << std::endl;
    }
}

// ======================
// GAME LOGIC UPDATES
// ======================

/**
 * Main update dispatch based on game state.
 */
void GameController::update(float deltaTime) {
    if (gameData.isPausedState() || gameData.isGameOver()) {
        return;
    }
    
    switch (gameData.getCurrentState()) {
        case GameState::SINGLE_PLAYER:
            updateSinglePlayer(deltaTime);
            break;
            
        case GameState::MULTIPLAYER_LOCAL:
            updateMultiplayerLocal(deltaTime);
            break;
            
        case GameState::MULTIPLAYER_ONLINE_GAME:
            updateMultiplayerOnline(deltaTime);
            break;
            
        default:
            break;
    }
}

/**
 * Updates single player game logic.
 */
void GameController::updateSinglePlayer(float deltaTime) {
    auto& board = gameData.getPlayerBoard(0);
    auto& stats = gameData.getPlayerStats(0);
    
    // Update fall timer
    gameData.setTimeSinceLastFall(gameData.getTimeSinceLastFall() + deltaTime);
    
    // Calculate fall speed based on level
    float currentFallSpeed = std::max(0.05f, 1.0f - (board.getLevel() * 0.1f));
    
    // Auto-fall logic
    if (gameData.getTimeSinceLastFall() > currentFallSpeed) {
        if (!board.moveDown()) {
            // Piece locked - check for game over
            if (board.isGameOver()) {
                gameOver();
            }
        }
        gameData.setTimeSinceLastFall(0.0f);
    }
    
    // Update player stats from board
    stats.score = board.getScore();
    stats.level = board.getLevel();
    stats.linesCleared = board.getLines();
    stats.fallSpeed = 1.0f / currentFallSpeed; // blocks per second
}

/**
 * Updates local multiplayer game logic for both players.
 */
void GameController::updateMultiplayerLocal(float deltaTime) {
    for (int player = 0; player < 2; player++) {
        auto& board = gameData.getPlayerBoard(player);
        auto& stats = gameData.getPlayerStats(player);
        
        // Player-specific timer
        float& playerTimer = (player == 0) ? player1FallTimer : player2FallTimer;
        playerTimer += deltaTime;
        
        // Calculate fall speed
        float currentFallSpeed = std::max(0.05f, 1.0f - (board.getLevel() * 0.1f));
        
        // Auto-fall for this player
        if (playerTimer > currentFallSpeed) {
            if (!board.moveDown()) {
                if (board.isGameOver()) {
                    gameOver();
                }
            }
            playerTimer = 0.0f;
        }
        
        // Update stats
        stats.score = board.getScore();
        stats.level = board.getLevel();
        stats.linesCleared = board.getLines();
        stats.fallSpeed = 1.0f / currentFallSpeed;
    }
}

/**
 * Updates online multiplayer logic including network communication.
 */
void GameController::updateMultiplayerOnline(float deltaTime) {
    // Check for client connections if hosting
    if (gameData.isHosting() && !gameData.isConnected()) {
        if (networkManager.waitForClient()) {
            gameData.setConnected(true);
            std::cout << "Client connected!" << std::endl;
            
            // Initialize game after connection
            gameData.getPlayerBoard(0).reset();
            gameData.getPlayerBoard(0).spawnPiece();
            gameData.setGameOver(false);
            gameData.setPaused(false);
        }
    }
    
    // Update local player
    updateSinglePlayer(deltaTime);
    
    // Network communication
    if (gameData.isConnected()) {
        sendGameState();
        receiveGameState();
        
        // Send game over if local player lost
        if (gameData.getPlayerBoard(0).isGameOver()) {
            NetworkMessage msg;
            msg.type = NetworkMessageType::GAME_OVER;
            msg.playerID = networkManager.getIsHost() ? 1 : 2;
            networkManager.sendMessage(msg);
        }
    }
}

// ======================
// NETWORK OPERATIONS
// ======================

/**
 * Disconnects from network server.
 */
void GameController::disconnectFromServer() {
    networkManager.disconnect();
    gameData.setConnected(false);
    gameData.setHosting(false);
    std::cout << "Disconnected from server" << std::endl;
}

/**
 * Sends local game state to opponent.
 */
void GameController::sendGameState() {
    if (!networkManager.getIsConnected()) return;
    
    auto& board = gameData.getPlayerBoard(0);
    NetworkMessage msg;
    msg.type = NetworkMessageType::BOARD_STATE;
    msg.playerID = networkManager.getIsHost() ? 1 : 2;
    msg.data1 = board.getScore();
    msg.data2 = board.getLines();
    
    networkManager.sendMessage(msg);
}

/**
 * Receives and processes game state from opponent.
 */
void GameController::receiveGameState() {
    if (!networkManager.getIsConnected()) return;
    
    NetworkMessage msg;
    while (networkManager.receiveMessage(msg)) {
        auto& opponentBoard = gameData.getPlayerBoard(1);
        
        switch (msg.type) {
            case NetworkMessageType::MOVE_LEFT:
                opponentBoard.moveLeft();
                break;
                
            case NetworkMessageType::MOVE_RIGHT:
                opponentBoard.moveRight();
                break;
                
            case NetworkMessageType::MOVE_DOWN:
                opponentBoard.moveDown();
                break;
                
            case NetworkMessageType::ROTATE:
                opponentBoard.rotate();
                break;
                
            case NetworkMessageType::HARD_DROP:
                opponentBoard.hardDrop();
                break;
                
            case NetworkMessageType::HOLD:
                opponentBoard.hold();
                break;
                
            case NetworkMessageType::BOARD_STATE:
                // Update opponent stats
                gameData.getPlayerStats(1).score = msg.data1;
                gameData.getPlayerStats(1).linesCleared = msg.data2;
                gameData.getPlayerStats(1).level = msg.data2 / 10 + 1;
                break;
                
            case NetworkMessageType::DISCONNECT:
                std::cout << "Opponent disconnected" << std::endl;
                returnToMainMenu();
                networkManager.disconnect();
                break;
                
            case NetworkMessageType::GAME_OVER:
                std::cout << "Opponent game over!" << std::endl;
                break;
                
            default:
                break;
        }
    }
}

// ======================
// GAME OVER HANDLING
// ======================

/**
 * Handles game over condition.
 */
void GameController::gameOver() {
    gameData.setGameOver(true);
    gameData.transitionToState(GameState::GAME_OVER);
    std::cout << "Game Over!" << std::endl;
    
    // Notify opponent if online
    if (gameData.isConnected()) {
        NetworkMessage msg;
        msg.type = NetworkMessageType::GAME_OVER;
        msg.playerID = networkManager.getIsHost() ? 1 : 2;
        networkManager.sendMessage(msg);
    }
}
