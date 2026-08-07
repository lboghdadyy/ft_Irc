#include "../includes/server.hpp"
#include "../includes/channels.hpp"

int Server::findFdByNick(std::string nick) 
{
    for (std::map<int, Client>::iterator i = cls.begin(); i != cls.end(); ++i) {
        if (i->second.getNick() == nick)
            return i->first;
    }
    return -1;
}

std::string Server::findNickByfd(int fd) 
{
    std::map<int, Client>::iterator i = cls.begin();
    while (i != cls.end())
    {
        if (i->second.getfd() == fd)
            return (i->second.getNick());
        i++;
    }
    throw (std::runtime_error(""));
    return ("");
}

void Server::removeFromAllChannels(int fd) 
{
    for (std::map<std::string, Channel>::iterator it = channels.begin(); it != channels.end(); ) {
        it->second.removeMember(fd);
        if (it->second.empty())
            channels.erase(it++);
        else
            ++it;
    }
}

void Server::pars(int fd) {
    std::map<int, Client>::iterator i = cls.find(fd);
    std::string &buf = i->second.getrecv();
    size_t pos;
    while ((pos = buf.find('\n')) != std::string::npos)
    {
        std::string line = buf.substr(0, pos);
        buf.erase(0, pos + 1);
        if (!line.empty() && line[line.size() - 1] == '\r')
            line.erase(line.size() - 1);
        if (!line.empty())
            handleCommand(fd, line);
    }
}