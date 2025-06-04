#pragma once

#include <netinet/in.h>
#include <poll.h>
#include <vector>

namespace Network {
  class PollManager {
    public:
      struct pollfd *data();
      size_t getSize() const;
      const struct pollfd &getSocket(size_t index) const;

      size_t addSocket(int fd, short events = POLLIN);
      void removeSocket(size_t index);

    private:
      std::vector<struct pollfd> _pollFds;
  };

  class ServerCommunication {
    public:
      ServerCommunication(int port);
      ServerCommunication(int port, const std::string &hostname);
      ~ServerCommunication();

      void run();
      static void requestShutdown();

    private:
      PollManager _pollManager;

      void createSocket();
      void bindSocket();
      void listenSocket();
      int acceptSocket();
      std::string handleIncomingMessage(int clientFd);

      int _serverFd;
      int _port;
      std::string _hostname;
      static bool _shutdownRequested;
  };
}  // namespace Network
