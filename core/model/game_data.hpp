#ifndef GAME_DATA_H
#define GAME_DATA_H

#include <string>
#include <memory>
#include "board.hpp"
#include "../utils/game_state.hpp"

class GameData {
public:
    // Estrutura para estatísticas do jogador
    struct PlayerStats {
        int score;
        int level;
        int linesCleared;
        float fallSpeed; // blocos por segundo
        
        PlayerStats() : score(0), level(1), linesCleared(0), fallSpeed(1.0f) {}
    };
    
private:
    // Boards dos jogadores
    ClassicBoard player1Board;
    ClassicBoard player2Board;
    
    // Estatísticas
    PlayerStats player1Stats;
    PlayerStats player2Stats;
    
    // Estado atual do jogo
    GameState previousState;
    GameState currentState;
    MenuOption selectedMenuOption;
    OnlineMenuOption selectedOnlineOption;
    
    // Timing
    float timeSinceLastFall;
    bool gameOver;
    bool isPaused;
    
    // Networking
    std::string inputIP;
    bool inputtingIP;
    bool connected;
    bool isHost;
    
    // Configurações de jogo
    bool ghostPieceEnabled;
    bool holdPieceEnabled;
    
    // Multiplayer local - qual jogador está ativo
    int currentActivePlayer; // 0 = player1, 1 = player2 (apenas para input)
    
public:
    // Construtor
    GameData();
    
    // === RESET METHODS ===
    void resetForSinglePlayer();
    void resetForMultiplayerLocal();
    void resetForMultiplayerOnline();
    void resetAll();
    
    // === STATE MANAGEMENT ===
    void transitionToState(GameState newState);
    
    // === BOARD ACCESS ===
    ClassicBoard& getPlayerBoard(int playerIndex);
    const ClassicBoard& getPlayerBoard(int playerIndex) const;
    
    ClassicBoard& getCurrentPlayerBoard();
    const ClassicBoard& getCurrentPlayerBoard() const;
    
    // === STATS ACCESS ===
    PlayerStats& getPlayerStats(int playerIndex);
    const PlayerStats& getPlayerStats(int playerIndex) const;
    
    PlayerStats& getCurrentPlayerStats();
    const PlayerStats& getCurrentPlayerStats() const;
    
    // === GETTERS ===
    GameState getPreviousState() const { return previousState; } 
    GameState getCurrentState() const { return currentState; }
    MenuOption getSelectedMenuOption() const { return selectedMenuOption; }
    OnlineMenuOption getSelectedOnlineOption() const { return selectedOnlineOption; }
    
    float getTimeSinceLastFall() const { return timeSinceLastFall; }
    void setTimeSinceLastFall(float time) { timeSinceLastFall = time; }
    
    bool isGameOver() const { return gameOver; }
    void setGameOver(bool gameOver) { 
        this->gameOver = gameOver; 
        if (gameOver) {
            currentState = GameState::GAME_OVER;
        }
    }
    
    bool isPausedState() const { return isPaused; }
    void setPaused(bool paused) { 
        isPaused = paused; 
        if (paused) {
            previousState = currentState;
            currentState = GameState::PAUSED;
        } else {
            currentState = previousState;
        }
    }
    
    const std::string& getInputIP() const { return inputIP; }
    void setInputIP(const std::string& ip) { inputIP = ip; }
    
    bool isInputtingIP() const { return inputtingIP; }
    void setInputtingIP(bool value) { inputtingIP = value; }
    
    bool isConnected() const { return connected; }
    void setConnected(bool value) { connected = value; }
    
    bool isHosting() const { return isHost; }
    void setHosting(bool value) { isHost = value; }
    
    bool hasGhostPiece() const { return ghostPieceEnabled; }
    void setGhostPieceEnabled(bool value) { ghostPieceEnabled = value; }
    
    bool hasHoldPiece() const { return holdPieceEnabled; }
    void setHoldPieceEnabled(bool value) { holdPieceEnabled = value; }
    
    int getCurrentActivePlayer() const { return currentActivePlayer; }
    void setCurrentActivePlayer(int player) { 
        if (player == 0 || player == 1) currentActivePlayer = player; 
    }
    
    void toggleCurrentActivePlayer() { 
        currentActivePlayer = (currentActivePlayer + 1) % 2; 
    }
    
    // === MENU NAVIGATION ===
    void moveMenuUp();
    void moveMenuDown();
    void moveOnlineMenuUp();
    void moveOnlineMenuDown();
    
    // === NETWORK HELPERS ===
    void appendToInputIP(char c);
    void backspaceInputIP();
    void clearInputIP() { inputIP.clear(); }
    
    // === GAME STATE CHECKS ===
    bool isInGame() const;
    bool isInMenu() const;
    bool isOnlineState() const;
    
private:
    // Helper para sincronizar stats do board com GameData
    void syncBoardStatsToGameData();
};

#endif
