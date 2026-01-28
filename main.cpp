#include <SFML/Graphics.hpp>
#include <iostream>

#include "core/model/game_data.hpp"
#include "core/controller/game_controller.hpp"
#include "core/view/game_renderer.hpp"

/**
 * Configures the view (camera) for a specific game state.
 * Different states use different view sizes and centers.
 * 
 * @param window The SFML render window
 * @param state The current game state
 */
void setupViewForState(sf::RenderWindow& window, GameState state) {
    sf::View view;
    sf::Vector2u windowSize = window.getSize();
    
    switch (state) {
        case GameState::SINGLE_PLAYER:
            // Single player: fixed 600x600 view area
            view.setSize(600, 600);
            view.setCenter(300, 300);
            break;
            
        case GameState::MULTIPLAYER_LOCAL:
        case GameState::MULTIPLAYER_ONLINE_GAME:
            // Multiplayer: full window view
            view.setSize(windowSize.x, windowSize.y);
            view.setCenter(windowSize.x / 2.0f, windowSize.y / 2.0f);
            break;
            
        case GameState::MAIN_MENU:
        case GameState::MULTIPLAYER_ONLINE_LOBBY:
        case GameState::PAUSED:
        case GameState::GAME_OVER:
            // Menus and overlays: proportional to window size
            view.setSize(windowSize.x, windowSize.y);
            view.setCenter(windowSize.x / 2.0f, windowSize.y / 2.0f);
            break;
            
        default:
            view.setSize(windowSize.x, windowSize.y);
            view.setCenter(windowSize.x / 2.0f, windowSize.y / 2.0f);
            break;
    }
    
    window.setView(view);
}

/**
 * Adjusts window size and view based on game state.
 * Handles special cases for overlay states (PAUSED, GAME_OVER).
 * 
 * @param window The SFML render window
 * @param state Current game state
 * @param previousState Previous game state
 */
void adjustForGameState(sf::RenderWindow& window, GameState state, GameState previousState) {
    // For overlay states, use the underlying game state for window sizing
    GameState sizeState = state;
    if (state == GameState::PAUSED || state == GameState::GAME_OVER) {
        sizeState = previousState;
    }
    
    // Determine target window size based on state
    sf::Vector2u targetSize;
    switch (sizeState) {
        case GameState::SINGLE_PLAYER:
            targetSize = sf::Vector2u(600, 600);
            break;
            
        case GameState::MULTIPLAYER_LOCAL:
        case GameState::MULTIPLAYER_ONLINE_GAME:
            targetSize = sf::Vector2u(1100, 600);
            break;
            
        case GameState::MAIN_MENU:
        case GameState::MULTIPLAYER_ONLINE_LOBBY:
            targetSize = sf::Vector2u(800, 600);
            break;
            
        default:
            targetSize = sf::Vector2u(800, 600);
            break;
    }
    
    // Resize window if needed and center it
    if (window.getSize() != targetSize) {
        window.setSize(targetSize);
        
        sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
        window.setPosition(sf::Vector2i(
            (desktop.width - targetSize.x) / 2,
            (desktop.height - targetSize.y) / 2
        ));
    }
    
    // Configure view for the current state
    setupViewForState(window, state);
}

/**
 * Main entry point for the Tetris MVC application.
 * Initializes the game window, game components, and runs the main loop.
 * 
 * @return Exit code (0 for success, 1 for error)
 */
int main() {
    try {
        // Initialize window (non-resizable)
        sf::RenderWindow window(
            sf::VideoMode(800, 600),
            "Tetris MVC",
            sf::Style::Titlebar | sf::Style::Close
        );
        window.setFramerateLimit(60);
        
        // Center window on screen
        sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
        window.setPosition(sf::Vector2i(
            (desktop.width - 800) / 2,
            (desktop.height - 600) / 2
        ));
        
        // Initialize MVC components
        GameData gameData;                    // Model - game state and data
        GameRenderer renderer;                // View - rendering system
        if (!renderer.loadResources()) {
            std::cerr << "Failed to load game resources" << std::endl;
            return 1;
        }
        
        GameController controller(gameData);  // Controller - input and game logic
        
        // Game loop variables
        sf::Clock gameClock;
        GameState previousState = gameData.getCurrentState();
        setupViewForState(window, previousState);
        
        // Main game loop
        while (window.isOpen()) {
            float deltaTime = gameClock.restart().asSeconds();
            
            // Process window events
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                    continue;
                }
                controller.handleEvent(event, window);
            }
            
            // Update game logic
            controller.update(deltaTime);
            
            // Check for state changes and adjust window/view if needed
            GameState currentState = gameData.getCurrentState();
            if (currentState != previousState) {
                adjustForGameState(window, currentState, previousState);
                previousState = currentState;
            }
            
            // Render the frame
            window.clear(sf::Color(10, 10, 40));  // Dark blue background
            renderer.render(window, gameData);
            window.display();
        }
        
        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "FATAL ERROR: " << e.what() << std::endl;
        return 1;
    }
}
