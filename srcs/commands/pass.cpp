#include "../../includes/server.hpp"
#include "../../includes/channels.hpp"

void    Server::cmdPass(int fd, std::vector<std::string> &params) {
    if (cls[fd].isRegistered())
        return sendToClient(fd, ":ircserv 462 " + cls[fd].getNick() + " :You may not reregister\r\n");
    if (params.empty())
        return sendToClient(fd, ":ircserv 461 " + cls[fd].getNick() + " PASS :Not enough parameters\r\n");
    if (params[0] != pw)
        return sendToClient(fd, ":ircserv 464 " + cls[fd].getNick() + " :Password incorrect\r\n");
    cls[fd].setPassOk(true);
    sendWelcomeIfReady(fd);
}