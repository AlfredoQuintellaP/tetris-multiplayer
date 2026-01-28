#ifndef NETWORK_MANAGER_H
#define NETWORK_MANAGER_H

#include <SFML/Network.hpp>
#include <string>

/**
 * Network message types for Tetris multiplayer communication.
 */
enum class NetworkMessageType {
    CONNECT,        // Connection establishment
    DISCONNECT,     // Connection termination
    MOVE_LEFT,      // Move piece left
    MOVE_RIGHT,     // Move piece right
    MOVE_DOWN,      // Move piece down
    ROTATE,         // Rotate piece
    HARD_DROP,      // Hard drop piece
    HOLD,           // Hold piece
    BOARD_STATE,    // Board statistics update
    GAME_OVER,      // Game over notification
    PING            // Keep-alive ping
};

/**
 * Structure for network messages between Tetris clients.
 */
struct NetworkMessage {
    NetworkMessageType type;  // Message type
    int playerID;             // Sender player identifier
    int data1;                // Additional data (score, etc.)
    int data2;                // Additional data (lines, etc.)
};

/**
 * Handles network communication for Tetris multiplayer.
 * Manages TCP connections for both hosting and joining games.
 */
class NetworkManager {
public:
    /**
     * Constructs a NetworkManager with non-blocking sockets.
     */
    NetworkManager();
    
    /**
     * Cleans up network resources.
     */
    ~NetworkManager();
    
    /**
     * Starts a server listening for client connections.
     * 
     * @param serverPort Port to listen on (default: 54000)
     * @return True if server started successfully
     */
    bool startServer(unsigned short serverPort = 54000);
    
    /**
     * Waits for and accepts an incoming client connection.
     * Non-blocking - returns immediately if no connection available.
     * 
     * @return True if client connected successfully
     */
    bool waitForClient();
    
    /**
     * Connects to a remote Tetris server.
     * 
     * @param ip Server IP address
     * @param serverPort Server port (default: 54000)
     * @return True if connection successful
     */
    bool connectToServer(const std::string& ip, unsigned short serverPort = 54000);
    
    /**
     * Disconnects from current connection and closes sockets.
     */
    void disconnect();
    
    /**
     * Sends a network message to the connected peer.
     * 
     * @param message Message to send
     * @return True if message sent successfully
     */
    bool sendMessage(const NetworkMessage& message);
    
    /**
     * Receives a network message from the connected peer.
     * Non-blocking - returns false immediately if no message available.
     * 
     * @param message Reference to store received message
     * @return True if message received successfully
     */
    bool receiveMessage(NetworkMessage& message);
    
    /**
     * Checks if this instance is hosting a game.
     * 
     * @return True if acting as server/host
     */
    bool getIsHost() const { return isHost; }
    
    /**
     * Checks if connected to a network peer.
     * 
     * @return True if connected to server/client
     */
    bool getIsConnected() const { return isConnected; }
    
private:
    sf::TcpSocket socket;      // Communication socket
    sf::TcpListener listener;  // Server listener (host only)
    bool isHost;               // Host/client flag
    bool isConnected;          // Connection status
    std::string serverIP;      // Server address (client only)
    unsigned short port;       // Connection port
};

#endif
