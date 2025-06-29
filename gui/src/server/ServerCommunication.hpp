#pragma once

#include <netinet/in.h>
#include <string>

namespace network {
  /**
   * @brief Handles communication with the Zappy server over TCP.
   *
   * Provides methods to connect, send, and receive messages, as well as check connection status.
   */
  class ServerCommunication {
    public:
      /**
       * @brief Constructs a ServerCommunication object.
       * @param port The port to connect to.
       * @param hostname The hostname or IP address to connect to (default: "127.0.0.1").
       */
      ServerCommunication(int port, const std::string &hostname = "127.0.0.1");
      /**
       * @brief Destructor. Closes the connection if open.
       */
      ~ServerCommunication();

      /**
       * @brief Checks if the client is currently connected to the server.
       * @return true if connected, false otherwise.
       */
      bool isConnected() const noexcept;
      /**
       * @brief Attempts to connect to the server.
       * @return true if connection is successful, false otherwise.
       */
      bool connectToServer();
      /**
       * @brief Sends a message to the server.
       * @param message The message to send.
       */
      void sendMessage(const std::string &message);
      /**
       * @brief Receives a message from the server (blocking until a full line is received).
       * @return The received message (including the newline character).
       */
      std::string receiveMessage();
      /**
       * @brief Checks if there is incoming data available to read from the server.
       * @return true if data is available, false otherwise.
       */
      bool hasIncomingData();

    private:
      /**
       * @brief Creates the client socket.
       * @throws std::runtime_error on failure.
       */
      void createSocket();
      /**
       * @brief Establishes the connection to the server.
       * @throws std::runtime_error on failure.
       */
      void establishConnection();

      int _clientFd;              ///< File descriptor for the client socket.
      int _port;                  ///< Server port.
      std::string _hostname;      ///< Server hostname or IP address.
      struct sockaddr_in _serverAddr; ///< Server address structure.
      bool _connected;            ///< Connection status flag.
      std::string _pendingData;   ///< Buffer for incomplete received data.
  };
}  // namespace network
