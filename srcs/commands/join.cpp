#include "../../includes/server.hpp"
#include "../../includes/channels.hpp"

void    Server::joinChannel(int fd, const std::string &channelName, const std::vector<std::string> &params, int &paramsIndex)
{
    std::cout << channelName << '\n';
        if (channelName[0] != '#')
            return sendToClient(fd, "403 " + cls[fd].getNick() + " " + channelName + " :No such channel\r\n");
        std::map<std::string, Channel>::iterator it = channels.find(channelName);
        if (it != channels.end() && it->second.isMember(fd))
            return ;
        if (it == channels.end()) {
            Channel newChannel(channelName, fd);
            channels.insert(std::make_pair(channelName, newChannel));
            it = channels.find(channelName);
            it->second.addOperator(fd);
        } else {
            Channel &chan = it->second;
            if (!chan.getKey().empty() && chan.getKey() != params[paramsIndex++])
                return sendToClient(fd, "475 * " + channelName + " :Cannot join channel (+k)\r\n");
            if (chan.getInviteOnly() && !chan.isInvited(fd))
                return sendToClient(fd, "473 * " + channelName + " :Cannot join channel (+i)\r\n");
            if (chan.getUserLimit() != 0 && chan.getMembers().size() >= chan.getUserLimit())
                return sendToClient(fd, "471 * " + channelName + " :Cannot join channel (+l)\r\n");
            chan.addMember(fd);
            chan.removeInvite(fd);
        }
        Channel &chan = channels.find(channelName)->second;
        std::string joinMsg = ":" + cls[fd].getNick() + "!user@" + cls[fd].getip() + " JOIN " + channelName + "\r\n";
    
        std::set<int> &members = chan.getMembers();
        for (std::set<int>::iterator m = members.begin(); m != members.end(); ++m)
            sendToClient(*m, joinMsg);
    
        if (!chan.getTopic().empty())
            sendToClient(fd, "332 " + cls[fd].getNick() + " " + channelName + " :" + chan.getTopic() + "\r\n");
        else
            sendToClient(fd, "331 " + cls[fd].getNick() + " " + channelName + " :No topic is set\r\n");
    
        std::string names;
        for (std::set<int>::iterator m = members.begin(); m != members.end(); ++m) {
            if (chan.isOperator(*m))
                names += "@";
            names += cls[*m].getNick() + " ";
        }
        sendToClient(fd, "353 " + cls[fd].getNick() + " = " + channelName + " :" + names + "\r\n");
        sendToClient(fd, "366 " + cls[fd].getNick() + " " + channelName + " :End of /NAMES list\r\n");
}

void Server::cmdJoin(int fd, std::vector<std::string> &params) {
    int _paramIndex = 1;
    if (!cls[fd].isRegistered())
        return sendToClient(fd, "451 " + cls[fd].getNick() + " :You have not registered\r\n");
    if (params.empty())
        return sendToClient(fd, "461 " + cls[fd].getNick() + " JOIN :Not enough parameters\r\n");
    std::stringstream sStream(params[0]);
    std::string channelName;
    while (std::getline(sStream, channelName, ','))
        joinChannel(fd, channelName, params, _paramIndex);
}
