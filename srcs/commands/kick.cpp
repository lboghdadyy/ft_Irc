#include "../../includes/server.hpp"

void Server::cmdKick(int fd, std::vector<std::string> &params) {
    if (params.size() < 2)
        return sendToClient(fd, "461 ...ERR_NEEDMOREPARAMS...\n");
    std::string channelName = params[0];
    std::string targetNick = params[1];
    std::string reason = (params.size() > 2) ? params[2] : targetNick;
    if (channelName[0] != '#')
        return sendToClient(fd, "403 ...ERR_NOSUCHCHANNEL...\n");

    // still

    std::cout << "KICK [" << channelName << "][" << targetNick << "][" << reason << "]\n";
}