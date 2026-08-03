#include "bot.hpp"

int checkNick(const std::string &nick)
{
    for (std::string::const_iterator it = nick.begin(); it != nick.end(); it++)
        if (!isspace(*it))
            return (0);
    return (1);
}

int main(int ac, char **av)
{
    if (ac < 4 || ac > 6)
    {
        std::cerr << "Usage: ./irc_bot <host> <port> <password> <nickname> <channel>\n";
        return 1;
    }
    int port = atoi(av[2]);
    if (port <= 0 || port > 65535)
    {
        std::cerr << "Bot : Invalid Port\n";
        return 1;
    }
    std::string host = av[1];
    std::string password = av[3];
    std::string nickname = (ac >= 5) ? av[4] : "bot";
    std::string channel = (ac >= 6 ? av[5] : "#general");
    if (checkNick(nickname))
    {
        std::cerr << "Bot : invalid nickname";
        return (1);
    }
    if (channel.empty() || channel[0] != '#')
    {
        std::cerr << "Bot : channel must start with #\n";
        return 1;
    }
    Bot bot(host, port, password, nickname, channel);
    if (bot.run() != 0)
        return 1;
    return 0;
}