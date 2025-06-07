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

Network::ClientCommunication::ClientCommunication(int port,
                                                  const std::string &hostname)
    : _clientFd(-1), _port(port), _hostname(hostname), _connected(false) {
  std::memset(&_serverAddr, 0, sizeof(_serverAddr));
}

Network::ClientCommunication::~ClientCommunication() {
  if (_clientFd != -1) {
    close(_clientFd);
    _clientFd = -1;
  }
  _connected = false;
}

bool Network::ClientCommunication::isConnected() const noexcept {
  return _connected;
}

bool Network::ClientCommunication::connectToServer() {
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

void Network::ClientCommunication::createSocket() {
  _clientFd = socket(AF_INET, SOCK_STREAM, 0);
  if (_clientFd == -1)
    throw std::runtime_error("Failed to create socket: " +
                             std::string(strerror(errno)));
}

void Network::ClientCommunication::establishConnection() {
  _serverAddr.sin_family = AF_INET;
  _serverAddr.sin_port = htons(_port);
  if (inet_pton(AF_INET, _hostname.c_str(), &_serverAddr.sin_addr) <= 0)
    throw std::runtime_error("Invalid address: " + _hostname);
  if (connect(_clientFd, (struct sockaddr *)&_serverAddr, sizeof(_serverAddr)) <
      0) {
    close(_clientFd);
    _clientFd = -1;
    throw std::runtime_error("Connection failed: " +
                             std::string(strerror(errno)));
  }
  _connected = true;
}

void Network::ClientCommunication::sendMessage(const std::string &message) {
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

std::string Network::ClientCommunication::receiveMessage() {
  if (!_connected || _clientFd == -1)
    throw std::runtime_error("Not connected to server");

  std::string message;
  const size_t BUFFER_SIZE = 1024;
  const size_t MAX_MESSAGE_SIZE = 65536;
  char buffer[BUFFER_SIZE];

  while (true) {
    if (message.size() > MAX_MESSAGE_SIZE)
      throw std::runtime_error("Message too large");

    ssize_t bytesReceived = recv(_clientFd, buffer, BUFFER_SIZE - 1, 0);

    if (bytesReceived == -1) {
      if (errno == EINTR)
        continue;
      throw std::runtime_error("Failed to receive message: " +
                               std::string(strerror(errno)));
    }

    if (bytesReceived == 0) {
      _connected = false;
      throw std::runtime_error("Server disconnected");
    }

    buffer[bytesReceived] = '\0';
    message += std::string(buffer, bytesReceived);

    size_t newlinePos = message.find('\n');
    if (newlinePos != std::string::npos)
      return message.substr(0, newlinePos + 1);
  }
}
