# include "../../includes/server.hpp"
# include "../../includes/channels.hpp"

void	Server::cmdTopic(int fd, std::vector<std::string> &params)
{
    if (params.size() < 1)
        return (sendToClient(fd, "TOPIC :Not enough parameters\n"));
    std::map<std::string, Channel>::iterator chan = this->channels.find(params[0]);
    if (chan == channels.end())
        return (sendToClient(fd, "403 " + cls[fd].getNick() + " " + params[0] + " :No such channel\n"));
    if (!chan->second.isMember(fd))
        return (sendToClient(fd,  "442 " + cls[fd].getNick() + " " + params[0] + " :You're not on that channel\n"));
    if (params.size() == 1)
    {
        if (chan->second.getTopic().empty())
            return (sendToClient(fd, "331 " + cls[fd].getNick() + " " + chan->first + " :No topic is set\n"));
        return (sendToClient(fd, "332 " + cls[fd].getNick() + " " +  \
        chan->first + " :" + chan->second.getTopic() + "\n"));
    }
    else
    {
        if (chan->second.getTopicRestrict() && !chan->second.isOperator(fd))
            return (sendToClient(fd, "482" + cls[fd].getNick() + " " + chan->first + " :You're not channel operator\n"));
        chan->second.setTopic(params[1]);
    }
}
