# include "../../includes/server.hpp"
# include "../../includes/channels.hpp"

void    Server::cmdMode(int fd, std::vector<std::string> &params)
{
    if (params.size() < 2)
        return (sendToClient(fd, "461 " + cls[fd].getNick() + " MODE :Not enough parameters\n"));
    std::map<std::string, Channel>::iterator it = channels.find(params[0]);
    if (it == channels.end())
        return sendToClient(fd, "403 " + cls[fd].getNick() + " " + params[0] + " :No such channel\n");
    if (!it->second.isMember(fd))
        return (sendToClient(fd, "442 " + cls[fd].getNick() + " " + params[0] + " :You're not on that channel\n"));
    if (!it->second.isOperator(fd))
        return (sendToClient(fd, "482 " + cls[fd].getNick() + " " + params[0] + " :You're not channel operator\n"));
    if (params[1] == "+i")
        it->second.setInviteOnly(true);
    else if (params[1] == "-i")
        it->second.setInviteOnly(false);
    else if (params[1] == "+t")
        it->second.setTopicRestrict(true);
    else if (params[1] == "-t")
        it->second.setTopicRestrict(false);
    else if (params[1] == "+k")
    {
        if (params.size() != 3)
            return (sendToClient(fd, "461 " + cls[fd].getNick() + " MODE :Not enough parameters\n"));
        it->second.setKey(params[2]);
    }
    else if (params[1] == "-k")
        it->second.setKey("");
    else if (params[1] == "+o")
    {
        if (params.size() != 3)
            return (sendToClient(fd, "461 " + cls[fd].getNick() + " MODE :Not enough parameters\n"));
        int _targetFd = findFdByNick(params[2]);
        if (_targetFd == -1)
            return sendToClient(fd, "401 " + cls[fd].getNick() + " " + params[2] + " :No such nick/channel\r\n");
        if (!it->second.isMember(_targetFd))
            return sendToClient(fd, "441 " + cls[fd].getNick() + " " + params[2] + " " + params[0] + " :They aren't on that channel\r\n");
        it->second.addOperator(_targetFd);          
    }
    else if (params[1] == "-o")
    {
        if (params.size() != 3)
            return (sendToClient(fd, "461 " + cls[fd].getNick() + " MODE :Not enough parameters\n"));
        int _targetFd = findFdByNick(params[2]);
        if (_targetFd == -1)
            return sendToClient(fd, "401 " + cls[fd].getNick() + " " + params[2] + " :No such nick/channel\r\n");
        if (!it->second.isMember(_targetFd))
            return sendToClient(fd, "441 " + cls[fd].getNick() + " " + params[2] + " " + params[0] + " :They aren't on that channel\r\n");
        it->second.removeOperator(_targetFd);          
    }
    else if (params[1] == "+l")
    {
        if (params.size() != 3)
            return (sendToClient(fd, "461 " + cls[fd].getNick() + " MODE :Not enough parameters\n"));
        it->second.setUserLimit(std::atoi(params[2].c_str()));
    }
    else if (params[1] == "-l")
    {
        it->second.setUserLimit(0);
    }
}