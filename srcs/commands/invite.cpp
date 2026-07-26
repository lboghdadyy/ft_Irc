#include "../../includes/server.hpp"
#include "../../includes/channels.hpp"

void Server::cmdInvite(int fd, std::vector<std::string> &params) {
    if (!cls[fd].isRegistered())
        return sendToClient(fd, "451 " + cls[fd].getNick() + " :You have not registered\r\n");
    if (params.size() < 2)
        return sendToClient(fd, "461 " + cls[fd].getNick() + " INVITE :Not enough parameters\r\n");
    std::string targetNick = params[0];
    std::string channelName = params[1];
    std::map<std::string, Channel>::iterator it = channels.find(channelName);
    if (it == channels.end() || channelName[0] != '#')
        return sendToClient(fd, "403 " + cls[fd].getNick() + " " + channelName + " :No such channel\r\n");
    Channel &chan = it->second;
    if (!chan.isMember(fd))
        return sendToClient(fd, "442 " + cls[fd].getNick() + " " + channelName + " :You're not on that channel\r\n");
    if (chan.getInviteOnly() && !chan.isOperator(fd))
        return sendToClient(fd, "482 " + cls[fd].getNick() + " " + channelName + " :You're not channel operator\r\n");
    int targetFd = findFdByNick(targetNick);
    if (targetFd == -1)
        return sendToClient(fd, "401 " + cls[fd].getNick() + " " + targetNick + " :No such nick/channel\r\n");
    if (chan.isMember(targetFd))
        return sendToClient(fd, "443 " + cls[fd].getNick() + " " + channelName + " :is already on channel\r\n");
    chan.invite(targetFd);
    sendToClient(fd, "341 " + cls[fd].getNick() + " " + channelName + " " + targetNick + "\r\n");
    std::string inviteMsg = ":" + cls[fd].getNick() + "!user@" + cls[fd].getip() + " INVITE " + targetNick + " " + channelName + "\r\n";
    sendToClient(targetFd, inviteMsg);
}
