#include "ServerCommunication.hpp"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cerrno>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string>

/**
 * @brief Constructs a ServerCommunication object and initializes member variables.
 *
 * @param port The port to connect to.
 * @param hostname The hostname or IP address to connect to.
 */
network::ServerCommunication::ServerCommunication(int port,
                                                  const std::string &hostname)
    : _clientFd(-1),
      _port(port),
      _hostname(hostname),
      _connected(false),
      _pendingData() {
  std::memset(&_serverAddr, 0, sizeof(_serverAddr));
}

/**
 * @brief Destructor. Closes the client socket if it is open and resets connection state.
 */
network::ServerCommunication::~ServerCommunication() {
  if (_clientFd != -1) {
    close(_clientFd);
    _clientFd = -1;
  }
  _connected = false;
}

/**
 * @brief Checks if the client is currently connected to the server.
 * @return true if connected, false otherwise.
 */
bool network::ServerCommunication::isConnected() const noexcept {
  return _connected;
}

/**
 * @brief Creates the client socket.
 * @throws std::runtime_error if socket creation fails.
 */
void network::ServerCommunication::createSocket() {
  _clientFd = socket(AF_INET, SOCK_STREAM, 0);
  if (_clientFd == -1)
    throw std::runtime_error("Failed to create socket: " +
                             std::string(strerror(errno)));
}

/**
 * @brief Establishes the connection to the server using the provided hostname and port.
 * @throws std::runtime_error if the address is invalid or connection fails.
 */
void network::ServerCommunication::establishConnection() {
  _serverAddr.sin_family = AF_INET;
  _serverAddr.sin_port = htons(_port);
  if (inet_pton(AF_INET, _hostname.c_str(), &_serverAddr.sin_addr) <= 0)
    throw std::runtime_error("Invalid address: " + _hostname);
  if (connect(_clientFd, (struct sockaddr *)&_serverAddr, sizeof(_serverAddr)) <
      0) {
    close(_clientFd);
    _clientFd = -1;
    throw std::runtime_error(std::string(strerror(errno)));
  }
  _connected = true;
}

/**
 * @brief Connects to the server by creating a socket and establishing a connection.
 * @return true if the connection was successful, false otherwise.
 */
bool network::ServerCommunication::connectToServer() {
  try {
    createSocket();
    establishConnection();
    sendMessage("GRAPHIC\n");
    return true;
  } catch (const std::exception &e) {
    std::cerr << "Connection failed: " << e.what() << std::endl;
    return false;
  }
}

/**
 * @brief Sends a message to the server.
 *
 * @param message The message to send.
 * @throws std::runtime_error if not connected to the server or if sending fails.
 */
void network::ServerCommunication::sendMessage(const std::string &message) {
  if (!_connected || _clientFd == -1)
    throw std::runtime_error("Not connected to server");

  size_t totalSent = 0;
  size_t messageLength = message.length();

  while (totalSent < messageLength) {
    ssize_t bytesSent = send(_clientFd, message.c_str() + totalSent,
                             messageLength - totalSent, 0);
    if (bytesSent == -1) {
      if (errno == EINTR)
        continue;
      throw std::runtime_error("Failed to send message: " +
                               std::string(strerror(errno)));
    }
    totalSent += bytesSent;
  }
  std::cout << "Sent to server: " << message;
}

/**
 * @brief Receives a message from the server. This function will block until a message is received.
 *
 * @return The received message.
 * @throws std::runtime_error if not connected to the server, if receiving fails,
 * or if the connection is closed by the server.
 */
std::string network::ServerCommunication::receiveMessage() {
  if (!_connected || _clientFd == -1)
    throw std::runtime_error("Not connected to server");

  size_t newlinePos = _pendingData.find('\n');
  if (newlinePos != std::string::npos) {
    std::string message = _pendingData.substr(0, newlinePos + 1);
    _pendingData.erase(0, newlinePos + 1);
    return message;
  }

  char buffer[1024];
  bool dataReceived = false;
  while (true) {
    ssize_t bytesReceived = recv(_clientFd, buffer, sizeof(buffer) - 1, 0);
    if (bytesReceived == -1) {
      if (errno == EAGAIN || errno == EWOULDBLOCK) {
        if (dataReceived) {
          newlinePos = _pendingData.find('\n');
          if (newlinePos != std::string::npos) {
            std::string message = _pendingData.substr(0, newlinePos + 1);
            _pendingData.erase(0, newlinePos + 1);
            return message;
          }
        }
        return "";
      }
      if (errno == EINTR)
        continue;
      throw std::runtime_error("Failed to receive message: " +
                               std::string(strerror(errno)));
    }
    if (bytesReceived == 0) {
      _connected = false;
      throw std::runtime_error("Connection closed by server");
    }
    buffer[bytesReceived] = '\0';
    _pendingData.append(buffer, bytesReceived);
    dataReceived = true;
    if (bytesReceived < static_cast<ssize_t>(sizeof(buffer) - 1))
      break;
  }
  newlinePos = _pendingData.find('\n');
  if (newlinePos != std::string::npos) {
    std::string message = _pendingData.substr(0, newlinePos + 1);
    _pendingData.erase(0, newlinePos + 1);
    return message;
  }
  return "";
}

/**
 * @brief Checks if there is incoming data from the server.
 * @return true if there is incoming data, false otherwise.
 */
bool network::ServerCommunication::hasIncomingData() {
  if (!_connected || _clientFd == -1)
    return false;
  if (!_pendingData.empty())
    return _pendingData.find('\n') != std::string::npos;

  struct pollfd pfd;
  pfd.fd = _clientFd;
  pfd.events = POLLIN;
  pfd.revents = 0;

  int result = poll(&pfd, 1, 0);
  if (result == -1) {
    if (errno != EINTR)
      std::cerr << "Poll error: " << strerror(errno) << std::endl;
    return false;
  }
  return result > 0 && (pfd.revents & POLLIN);
}

