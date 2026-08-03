# include "../../includes/server.hpp"
# include "../../includes/channels.hpp"

void    Server::sendMultClient(int fd, std::string nick, std::string message)
{
    int reciver = findFdByNick(nick);
    if (reciver == -1)
        return (sendToClient(fd, "401 " + cls[fd].getNick() + " " + nick + " :No such nick\n"));
    sendToClient(reciver, ":" + nick + " PRIVMSG " + cls[fd].getNick() + " :" + message + '\n');
}

void    Server::cmdPrivmsg(int fd, std::vector<std::string> &arguments)
{
    std::map<std::string, Channel>::iterator    it;
    std::set<int>                               _members;
    std::string                                 users;


    if (!cls.find(fd)->second.isRegistered())
        return sendToClient(fd, "451 " + cls[fd].getNick() + " :You have not registered\r\n");
    if (arguments.empty())
        return (sendToClient(fd, "411 " + cls[fd].getNick() + " :No recipient given (PRIVMSG)\n"));
    if (arguments.size() != 2)
        return (sendToClient(fd, "412 " + cls[fd].getNick() + " :No text to send\n"));
    if (arguments.at(0).at(0) == '#')
    {
        it = channels.find(arguments.at(0));
        if (it == channels.end())
            return (sendToClient(fd, "403 " + cls[fd].getNick() + " " + arguments[0] + " :No such channel\n"));
        if (!it->second.isMember(fd))
            return (sendToClient(fd, "404 " + cls[fd].getNick() + " " + arguments[0] + " :Cannot send to channel\n"));
        _members = it->second.getMembers();
        for (std::set<int>::iterator it = _members.begin();
         it != _members.end();
         it++)
        {
            if (*it != fd)
                sendToClient(*it, ":" + cls[fd].getNick() + " PRIVMSG " + arguments[0] + " :" + arguments[1] + '\n');
        }
    }
    else
    {
        std::stringstream stringStream(arguments[0]);
        while (getline(stringStream, users, ','))
            sendMultClient(fd, users, arguments[1]);
    }
}