#include "game_data.hpp"
#include <algorithm>

GameData::GameData() 
    : currentState(GameState::MAIN_MENU)
    , selectedMenuOption(MenuOption::SINGLE_PLAYER)
    , selectedOnlineOption(OnlineMenuOption::HOST_GAME)
    , timeSinceLastFall(0.0f)
    , gameOver(false)
    , isPaused(false)
    , inputtingIP(false)
    , connected(false)
    , isHost(false)
    , ghostPieceEnabled(true)
    , holdPieceEnabled(true)
    , currentActivePlayer(0) {
    
    resetAll();
}

void GameData::resetAll() {
    player1Board.reset();
    player2Board.reset();
    
    player1Stats = PlayerStats();
    player2Stats = PlayerStats();
    
    timeSinceLastFall = 0.0f;
    gameOver = false;
    isPaused = false;
    
    inputIP.clear();
    inputtingIP = false;
    connected = false;
    isHost = false;
    
    currentActivePlayer = 0;
}

void GameData::resetForSinglePlayer() {
    resetAll();
    currentState = GameState::SINGLE_PLAYER;
    player1Board.reset();
    player1Stats = PlayerStats();
}

void GameData::resetForMultiplayerLocal() {
    resetAll();
    currentState = GameState::MULTIPLAYER_LOCAL;
    player1Board.reset();
    player2Board.reset();
    player1Stats = PlayerStats();
    player2Stats = PlayerStats();
}

void GameData::resetForMultiplayerOnline() {
    resetAll();
    currentState = GameState::MULTIPLAYER_ONLINE_LOBBY;
    // Não resetamos os boards ainda - só quando o jogo começar
}

void GameData::transitionToState(GameState newState) {
    if (currentState == newState) return;
    
    // Lógica de transição
    switch (newState) {
        case GameState::MAIN_MENU:
            resetAll();
            break;
            
        case GameState::SINGLE_PLAYER:
            resetForSinglePlayer();
            break;
            
        case GameState::MULTIPLAYER_LOCAL:
            resetForMultiplayerLocal();
            break;
            
        case GameState::MULTIPLAYER_ONLINE_LOBBY:
            resetForMultiplayerOnline();
            break;
            
        case GameState::MULTIPLAYER_ONLINE_GAME:
            // Mantém os dados atuais
            break;
            
        default:
            break;
    }
    previousState = currentState; 
    currentState = newState;
}

ClassicBoard& GameData::getPlayerBoard(int playerIndex) {
    return (playerIndex == 0) ? player1Board : player2Board;
}

const ClassicBoard& GameData::getPlayerBoard(int playerIndex) const {
    return (playerIndex == 0) ? player1Board : player2Board;
}

ClassicBoard& GameData::getCurrentPlayerBoard() {
    // Em single player, sempre retorna player1
    if (currentState == GameState::SINGLE_PLAYER) {
        return player1Board;
    }
    return getPlayerBoard(currentActivePlayer);
}

const ClassicBoard& GameData::getCurrentPlayerBoard() const {
    if (currentState == GameState::SINGLE_PLAYER) {
        return player1Board;
    }
    return getPlayerBoard(currentActivePlayer);
}

GameData::PlayerStats& GameData::getPlayerStats(int playerIndex) {
    return (playerIndex == 0) ? player1Stats : player2Stats;
}

const GameData::PlayerStats& GameData::getPlayerStats(int playerIndex) const {
    return (playerIndex == 0) ? player1Stats : player2Stats;
}

GameData::PlayerStats& GameData::getCurrentPlayerStats() {
    if (currentState == GameState::SINGLE_PLAYER) {
        return player1Stats;
    }
    return getPlayerStats(currentActivePlayer);
}

const GameData::PlayerStats& GameData::getCurrentPlayerStats() const {
    if (currentState == GameState::SINGLE_PLAYER) {
        return player1Stats;
    }
    return getPlayerStats(currentActivePlayer);
}

void GameData::moveMenuUp() {
    int current = static_cast<int>(selectedMenuOption);
    int count = static_cast<int>(MenuOption::COUNT);
    selectedMenuOption = static_cast<MenuOption>((current - 1 + count) % count);
}

void GameData::moveMenuDown() {
    int current = static_cast<int>(selectedMenuOption);
    int count = static_cast<int>(MenuOption::COUNT);
    selectedMenuOption = static_cast<MenuOption>((current + 1) % count);
}

void GameData::moveOnlineMenuUp() {
    int current = static_cast<int>(selectedOnlineOption);
    int count = static_cast<int>(OnlineMenuOption::COUNT);
    selectedOnlineOption = static_cast<OnlineMenuOption>((current - 1 + count) % count);
}

void GameData::moveOnlineMenuDown() {
    int current = static_cast<int>(selectedOnlineOption);
    int count = static_cast<int>(OnlineMenuOption::COUNT);
    selectedOnlineOption = static_cast<OnlineMenuOption>((current + 1) % count);
}

void GameData::appendToInputIP(char c) {
    // Aceita apenas dígitos e ponto
    if ((c >= '0' && c <= '9') || c == '.') {
        inputIP += c;
    }
}

void GameData::backspaceInputIP() {
    if (!inputIP.empty()) {
        inputIP.pop_back();
    }
}

bool GameData::isInGame() const {
    return currentState == GameState::SINGLE_PLAYER ||
           currentState == GameState::MULTIPLAYER_LOCAL ||
           currentState == GameState::MULTIPLAYER_ONLINE_GAME;
}

bool GameData::isInMenu() const {
    return currentState == GameState::MAIN_MENU ||
           currentState == GameState::MULTIPLAYER_ONLINE_LOBBY;
}

bool GameData::isOnlineState() const {
    return currentState == GameState::MULTIPLAYER_ONLINE_LOBBY ||
           currentState == GameState::MULTIPLAYER_ONLINE_GAME;
}

void GameData::syncBoardStatsToGameData() {
    // Sincroniza as estatísticas do board com o GameData
    player1Stats.score = player1Board.getScore();
    player1Stats.level = player1Board.getLevel();
    player1Stats.linesCleared = player1Board.getLines();
    
    player2Stats.score = player2Board.getScore();
    player2Stats.level = player2Board.getLevel();
    player2Stats.linesCleared = player2Board.getLines();
}

