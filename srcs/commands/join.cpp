#include "../../includes/server.hpp"
#include "../../includes/channels.hpp"

void Server::cmdJoin(int fd, std::vector<std::string> &params) {
    if (params.empty())
        return sendToClient(fd, "461 ...ERR_NEEDMOREPARAMS...\n");
    std::string channelName = params[0];
    std::string key = (params.size() > 1) ? params[1] : "";
    if (channelName[0] != '#')
        return sendToClient(fd, "403 ...ERR_NOSUCHCHANNEL...\n");
    std::map<std::string, Channel>::iterator it = channels.find(channelName);
    if (it == channels.end()) {
        Channel newChannel(channelName, fd);
        channels.insert(std::make_pair(channelName, newChannel));
        it = channels.find(channelName);
    } else {
        Channel &chan = it->second;
        if (!chan.getKey().empty() && chan.getKey() != key)
            return sendToClient(fd, "475 * " + channelName + " :Cannot join channel (+k)\r\n");
        if (chan.getInviteOnly() && !chan.isInvited(fd))
            return sendToClient(fd, "473 * " + channelName + " :Cannot join channel (+i)\r\n");
        if (chan.getUserLimit() != 0 && chan.getMembers().size() >= chan.getUserLimit())
            return sendToClient(fd, "471 * " + channelName + " :Cannot join channel (+l)\r\n");
        chan.addMember(fd);
        chan.removeInvite(fd);
    }
    Channel &chan = channels.find(channelName)->second;
    std::string joinMsg = ":" + cls[fd].getNick() + "!" + "user" + "@" + cls[fd].getip() + " JOIN " + channelName + "\r\n";

    std::set<int> &members = chan.getMembers();
    for (std::set<int>::iterator m = members.begin(); m != members.end(); ++m)
        sendToClient(*m, joinMsg);

    if (!chan.getTopic().empty())
        sendToClient(fd, "332 " + cls[fd].getNick() + " " + channelName + " :" + chan.getTopic() + "\r\n");
    else
        sendToClient(fd, "331 " + cls[fd].getNick() + " " + channelName + " :No topic is set\r\n");

    std::string names;
    for (std::set<int>::iterator m = members.begin(); m != members.end(); ++m)
    {
        if (chan.isOperator(*m))
            names += "@";
        names += cls[*m].getNick() + " ";
    }
    sendToClient(fd, "353 " + cls[fd].getNick() + " = " + channelName + " :" + names + "\r\n");
    sendToClient(fd, "366 " + cls[fd].getNick() + " " + channelName + " :End of /NAMES list\r\n");

    std::cout << "JOIN [" << channelName << "][" << key << "]\n";
}
