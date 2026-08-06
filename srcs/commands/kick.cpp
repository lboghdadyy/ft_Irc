#include "../../includes/server.hpp"
#include "../../includes/channels.hpp"

void howManyof(std::string argument, std::vector<std::string> &arr) {
    std::stringstream   sstream(argument);
    std::string         word;
    

    while (std::getline(sstream, word, ','))
        arr.push_back(word);
}

void Server::cmdKick(int fd, std::vector<std::string> &params) {
    std::vector<std::string>    _channelsVect;
    std::vector<std::string>    _users;

    howManyof(params[0], _channelsVect);
    howManyof(params[1], _users);
    if (!cls[fd].isRegistered())
        return sendToClient(fd, ":ircserv 451 " + cls[fd].getNick() + " :You have not registered\r\n");
    if (params.size() < 2)
        return sendToClient(fd, ":ircserv 461 " + cls[fd].getNick() + " KICK :Not enough parameters\r\n");
    int _channels = _channelsVect.size();
    int users = _users.size();
    std::string reason = (params.size() > 2) ? params[2] : "";
    if ((users != 1 && _channels != 1) && _channels != users)
        return sendToClient(fd, ":ircserv 461 " + cls[fd].getNick() + " KICK :Not enough parameters\r\n");
    std::vector<std::string>::iterator it_channels = _channelsVect.begin();
    std::vector<std::string>::iterator it_users = _users.begin();
    while (true) {
        std::map<std::string, Channel>::iterator _foundeChan = channels.find(*it_channels);
        if (_foundeChan == channels.end() || it_channels->at(0) != '#')
            return sendToClient(fd, ":ircserv 403 " + cls[fd].getNick() + " " + *it_channels + " :No such channel\r\n");
        Channel &chan = _foundeChan->second;
        if (!chan.isMember(fd))
            return sendToClient(fd, ":ircserv 442 " + cls[fd].getNick() + " " + *it_channels + " :You're not on that channel\r\n");
        if (!chan.isOperator(fd))
            return sendToClient(fd, ":ircserv 482 " + cls[fd].getNick() + " " + *it_channels + " :You're not channel operator\r\n");
        int targetFd = findFdByNick(*it_users);
        if (targetFd == -1)
            return sendToClient(fd, ":ircserv 401 " + cls[fd].getNick() + " " + _users[0] + " :No such nick/channel\r\n");
        if (!chan.isMember(targetFd))
            return sendToClient(fd, ":ircserv 441 " + cls[fd].getNick() + " " + *it_channels + " :They aren't on that channel\r\n");
        std::string kickMsg = ":" + cls[fd].getNick() + "!user@" + cls[fd].getip() + " KICK " + *it_channels + " " + *it_users + " :" + reason + "\r\n";
        std::set<int> &members = chan.getMembers();
        for (std::set<int>::iterator m = members.begin(); m != members.end(); ++m)
            sendToClient(*m, kickMsg);
        chan.removeMember(targetFd);
        if (chan.empty())
            channels.erase(*it_channels);
        if (users == 1 && users != _channels) {
            it_channels++;
            if (it_channels == _channelsVect.end())
                break;
        }
        else if (_channels == 1 && _channels != users) {
            it_users++;
            if (it_users == _users.end())
                break;
        }
        else {
            it_channels++, it_users++;
            if (it_users == _users.end() || it_channels == _channelsVect.end())
                break;
        }
    }
}