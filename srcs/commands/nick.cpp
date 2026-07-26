#include "../../includes/server.hpp"
#include "../../includes/channels.hpp"

void Server::cmdNick(int fd, std::vector<std::string> &params) {
    if (params.empty())
        return sendToClient(fd, "431 " + cls[fd].getNick() + " :No nickname given\r\n");
    std::string newNickname = params[0];
    for (std::map<int, Client>::iterator i = cls.begin(); i != cls.end(); ++i) {
        if (i->first != fd && i->second.getNick() == newNickname)
            return sendToClient(fd, "433 " + cls[fd].getNick() + " " + newNickname + " :Nickname is already in use\r\n");
    }
    std::string oldNickname = cls[fd].getNick();
    cls[fd].setNick(newNickname);
    cls[fd].setHasNick(true);
    if (cls[fd].isRegistered())
        sendToClient(fd, ":" + oldNickname + "!user@" + cls[fd].getip() + " NICK :" + newNickname + "\r\n");
}
