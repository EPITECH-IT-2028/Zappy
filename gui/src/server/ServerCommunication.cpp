#include "ServerCommunication.hpp"
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cerrno>
#include <stdexcept>
#include <string>

size_t Network::PollManager::getSize() const {
  return _pollFds.size();
}

size_t Network::PollManager::addSocket(int fd, short events) {
  for (size_t i = 0; i < _pollFds.size(); ++i) {
    if (_pollFds[i].fd == -1) {
      _pollFds[i] = {fd, events, 0};
      return i;
    }
  }

  _pollFds.emplace_back(fd, events, 0);
  return _pollFds.size() - 1;
}

void Network::PollManager::removeSocket(size_t index) {
  if (index >= _pollFds.size())
    throw std::out_of_range("Index out of range in PollManager");
  _pollFds[index].fd = -1;
}

Network::ServerCommunication::ServerCommunication(int port,
                                                  std::string hostname)
    : _serverFd(-1), _port(port), _hostname(hostname) {
  createSocket();
  bindSocket();
  listenSocket();
  _pollManager.addSocket(_serverFd, POLLIN);
}

Network::ServerCommunication::~ServerCommunication() {
  if (_serverFd >= 0) {
    close(_serverFd);
  }
}

void Network::ServerCommunication::createSocket() {
  _serverFd = socket(AF_INET, SOCK_STREAM, 0);
  if (_serverFd < 0)
    throw std::runtime_error("Failed to create socket: " +
                             std::to_string(errno));
  int opt = 1;
  if (setsockopt(_serverFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
    throw std::runtime_error("Failed to set socket options: " +
                             std::to_string(errno));
}

void Network::ServerCommunication::bindSocket() {
  sockaddr_in serverAddr;
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(_port);
  serverAddr.sin_addr.s_addr = INADDR_ANY;

  if (bind(_serverFd, reinterpret_cast<sockaddr *>(&serverAddr),
           sizeof(serverAddr)) < 0)
    throw std::runtime_error("Failed to bind socket: " + std::to_string(errno));
}

void Network::ServerCommunication::listenSocket() {
  if (listen(_serverFd, SOMAXCONN) < 0)
    throw std::runtime_error("Failed to listen on socket: " +
                             std::to_string(errno));
}

int Network::ServerCommunication::acceptSocket() {
  sockaddr_in clientAddr;
  socklen_t clientAddrLen = sizeof(clientAddr);
  int clientFd = accept(_serverFd, reinterpret_cast<sockaddr *>(&clientAddr),
                        &clientAddrLen);
  if (clientFd < 0)
    throw std::runtime_error("Failed to accept connection: " +
                             std::to_string(errno));
  return clientFd;
}
