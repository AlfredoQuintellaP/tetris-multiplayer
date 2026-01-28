#ifndef GAME_STATE_H
#define GAME_STATE_H

/**
 * Main game states defining different screens and modes.
 */
enum class GameState {
    MAIN_MENU,                 // Main menu screen
    SINGLE_PLAYER,             // Single player game
    MULTIPLAYER_LOCAL,         // Local multiplayer (two players)
    MULTIPLAYER_ONLINE_LOBBY,  // Online multiplayer lobby
    MULTIPLAYER_ONLINE_GAME,   // Online multiplayer game
    PAUSED,                    // Game paused overlay
    GAME_OVER                  // Game over screen
};

/**
 * Main menu navigation options.
 */
enum class MenuOption {
    SINGLE_PLAYER,
    MULTIPLAYER_LOCAL,
    MULTIPLAYER_ONLINE,
    EXIT,
    COUNT  // Sentinel value for menu option count
};

/**
 * Online multiplayer menu options.
 */
enum class OnlineMenuOption {
    HOST_GAME,
    JOIN_GAME,
    BACK,
    COUNT  // Sentinel value for menu option count
};

#endif
