#include "../../includes/server.hpp"
#include "../../includes/channels.hpp"

void Server::cmdKick(int fd, std::vector<std::string> &params) {
    if (!cls[fd].isRegistered())
        return sendToClient(fd, "451 " + cls[fd].getNick() + " :You have not registered\r\n");
    if (params.size() < 2)
        return sendToClient(fd, "461 " + cls[fd].getNick() + " KICK :Not enough parameters\r\n");
    std::string channelName = params[0];
    std::string targetNick = params[1];
    std::string reason = (params.size() > 2) ? params[2] : "";
    std::map<std::string, Channel>::iterator it = channels.find(channelName);
    if (it == channels.end() || channelName[0] != '#')
        return sendToClient(fd, "403 " + cls[fd].getNick() + " " + channelName + " :No such channel\r\n");
    Channel &chan = it->second;
    if (!chan.isMember(fd))
        return sendToClient(fd, "442 " + cls[fd].getNick() + " " + channelName + " :You're not on that channel\r\n");
    if (!chan.isOperator(fd))
        return sendToClient(fd, "482 " + cls[fd].getNick() + " " + channelName + " :You're not channel operator\r\n");
    int targetFd = findFdByNick(targetNick);
    if (targetFd == -1)
        return sendToClient(fd, "401 " + cls[fd].getNick() + " " + targetNick + " :No such nick/channel\r\n");
    if (!chan.isMember(targetFd))
        return sendToClient(fd, "441 " + cls[fd].getNick() + " " + channelName + " :They aren't on that channel\r\n");
    std::string kickMsg = ":" + cls[fd].getNick() + "!user@" + cls[fd].getip() + " KICK " + channelName + " " + targetNick + " :" + reason + "\r\n";
    std::set<int> &members = chan.getMembers();
    for (std::set<int>::iterator m = members.begin(); m != members.end(); ++m)
        sendToClient(*m, kickMsg);
    chan.removeMember(targetFd);
    if (chan.empty())
        channels.erase(channelName);
}