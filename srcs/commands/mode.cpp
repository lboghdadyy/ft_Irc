# include "../../includes/server.hpp"
# include "../../includes/channels.hpp"


void    Server::cmdMode(int fd, std::vector<std::string> &params)
{
    size_t paramIndex = 2;
    if (params.size() < 2)
        return (sendToClient(fd, "461 " + cls[fd].getNick() + " MODE :Not enough parameters\n"));
    std::map<std::string, Channel>::iterator it = channels.find(params[0]);
    if (it == channels.end())
        return sendToClient(fd, "403 " + cls[fd].getNick() + " " + params[0] + " :No such channel\n");
    if (!it->second.isMember(fd))
        return (sendToClient(fd, "442 " + cls[fd].getNick() + " " + params[0] + " :You're not on that channel\n"));
    if (!it->second.isOperator(fd))
        return (sendToClient(fd, "482 " + cls[fd].getNick() + " " + params[0] + " :You're not channel operator\n"));
    char sign = params[1][0];
    for (size_t index = 1; index < params[1].size(); index++)
    {
        if (index == 4)
            return ;
        if (params[1][index] == 'i' && sign == '+')
            it->second.setInviteOnly(true);
        else if (params[1][index] == 'i' && sign == '-')
            it->second.setInviteOnly(false);
        else if (params[1][index] == 't' && sign == '+')
            it->second.setTopicRestrict(true);
        else if (params[1][index] == 't' && sign == '-')
            it->second.setTopicRestrict(false);
        else if (params[1][index] == 'k' && sign == '+')
        {
            if (params.size() < 1 + paramIndex)
                return (sendToClient(fd, "461 " + cls[fd].getNick() + " MODE :Not enough parameters\n"));
            it->second.setKey(params[paramIndex++]);
        }
        else if (params[1][index] == 'k' && sign == '-')
            it->second.setKey("");
        else if (params[1][index] == 'o' && sign == '+')
        {
            if (params.size() < 1 + paramIndex)
                return (sendToClient(fd, "461 " + cls[fd].getNick() + " MODE :Not enough parameters\n"));
            int _targetFd = findFdByNick(params[paramIndex++]);
            if (_targetFd == -1)
                return sendToClient(fd, "401 " + cls[fd].getNick() + " " + params[paramIndex] + " :No such nick/channel\r\n");
            if (!it->second.isMember(_targetFd))
                return sendToClient(fd, "441 " + cls[fd].getNick() + " " + params[paramIndex] + " " + params[0] + " :They aren't on that channel\r\n");
            it->second.addOperator(_targetFd);          
        }
        else if (params[1][index] == 'o' && sign == '-')
        {
            if (params.size() < 1 + paramIndex)
                return (sendToClient(fd, "461 " + cls[fd].getNick() + " MODE :Not enough parameters\n"));
            int _targetFd = findFdByNick(params[paramIndex++]);
            if (_targetFd == -1)
                return sendToClient(fd, "401 " + cls[fd].getNick() + " " + params[paramIndex] + " :No such nick/channel\r\n");
            if (!it->second.isMember(_targetFd))
                return sendToClient(fd, "441 " + cls[fd].getNick() + " " + params[paramIndex] + " " + params[0] + " :They aren't on that channel\r\n");
            it->second.removeOperator(_targetFd);          
        }
        else if (params[1][index] == 'l' && sign == '+')
        {
            if (params.size() < 1 + paramIndex)
                return (sendToClient(fd, "461 " + cls[fd].getNick() + " MODE :Not enough parameters\n"));
            it->second.setUserLimit(std::atoi(params[paramIndex++].c_str()));
        }
        else if (params[1][index] == 'l' && sign == '-')
            it->second.setUserLimit(0);
        else
            sendToClient(fd, "472 " + cls[fd].getNick() + " "  + params[1][index] + " :is unknown mode char to me for " + params[0] + '\n');
    }
}