#include "../../includes/server.hpp"
#include "../../includes/channels.hpp"

void    Server::cmdUser(int fd, std::vector<std::string> &params) {
    if (cls[fd].isRegistered())
        return sendToClient(fd, ":ircserv 462 " + cls[fd].getNick() + " :You may not reregister\r\n");
    if (params.size() < 4)
        return sendToClient(fd, ":ircserv 461 " + cls[fd].getNick() + " USER :Not enough parameters\r\n");
    cls[fd].setUser(params[0]);
    cls[fd].setRealname(params[3]);
    cls[fd].setHasUser(true);
    sendWelcomeIfReady(fd);
}
