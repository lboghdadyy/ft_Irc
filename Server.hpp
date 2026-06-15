#ifndef SERVER_HPP
# define SERVER_HPP

# include <vector>
# include <iostream>
# include "Client.hpp"
# include "Channel.hpp"

class Server {
    private:
        /*
        int                             serverFd;
        std::string                     password;
        std::vector<pollfd>             pollfds;
        std::map<int, Client *>         clients;    // fd → Client*
        std::map<std::string, Channel*> channels;   // "#name" → Channel*
        void _acceptNewClient();
        void _handleClientData(int fd);
        void _removeClient(int fd);
        dber rassek m3a hado ajmi ana mafhmthomch
        */

        void _dispatchCommand(Client &client, const std::string &line); // I will call handleJoin(), handleInvite(), handleKick() oussama functions
    public:
        // Sbouaa Part + OCF
        Server(int port, const std::string &password);
        ~Server();
        void run();
        // an7taj darori hado (sendMessage, findClient, findChannel)
        void sendMessage(int fd, const std::string &message);
        void broadcastToChannel(Channel &channel, const std::string &message, Client *exclude = NULL); // oussama ay7tajeha f JION, KICK

        Client *findClientByNick(const std::string &nick); // INVITE , KICK
        Channel *findChannel(const std::string &name); // JOIN, INVITE, KICK
        Channel *createChannel(const std::string &name, Client *firstOperator); // JION
};

#endif