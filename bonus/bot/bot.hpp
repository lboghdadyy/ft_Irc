#ifndef BOT_HPP
#define BOT_HPP

#include <cstring>
#include <cerrno>
#include <fcntl.h>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>

class Bot {
    private:
        int port;
        int sfd;
        std::string host;
        std::string password;
        std::string nick;
        std::string user;
        std::string channel;
        std::string recvBuff;
        Bot(const Bot &x);
        Bot() {};
        Bot& operator=(const Bot &x);
    public:
        Bot(std::string &host, int port, std::string &pass, std::string &nick, std::string &channel);
        bool    run();
        void    auth();
        bool    connectToServer();
        bool    sendline(const std::string &x);
        bool    loop();
        ~Bot();
};

#endif
