#include "../../includes/server.hpp"

void Server::cmdInvite(int fd, std::vector<std::string> &params) {

    if (params.size() < 2)
        return sendToClient(fd, "461 ...ERR_NEEDMOREPARAMS...\n");
    std::string targetNick = params[0];
    std::string channelName = params[1];
    if (channelName[0] != '#')
        return sendToClient(fd, "403 ...ERR_NOSUCHCHANNEL...\n");
    // still
    std::cout << "INVITE [" << targetNick << "][" << channelName << "]";
}
