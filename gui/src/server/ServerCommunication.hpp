#pragma once

#include <netinet/in.h>
#include <poll.h>
#include <vector>

namespace Network {
  class PollManager {
    public:
      size_t getSize() const;

      size_t addSocket(int fd, short events = POLLIN);
      void removeSocket(size_t index);

    private:
      std::vector<struct pollfd> _pollFds;
  };

  class ServerCommunication {
    public:
      ServerCommunication(int port);
      ~ServerCommunication();

    private:
      PollManager _pollManager;
      void createSocket();
      void bindSocket();
      void listenSocket();
      int acceptSocket();
      int _serverFd;
      int _port;
      std::string _hostname;
  };
}  // namespace Network
