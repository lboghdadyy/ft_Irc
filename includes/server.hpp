#ifndef SERVER_HPP
#define SERVER_HPP

class Channel;
// # include "channels.hpp"
#include "client.hpp"

class Server {
    private :
        int ss;
        int port;
        int epollFd;
        std::string pw;
        std::map <int, Client> cls;
    public :
        Server();
        Server(std::string p, std::string s);
        Server(const Server &x);
        Server& operator=(const Server &x);
        void    Check(std::string p, std::string s);
        void    ServSock();
        void    AccClient();
        void    rmvClient(int fd);
        void    clientread(int fd);
        void    clientwrite(int fd);
        void    clearEpoll(int fd, bool x);
        void    sendToClient(int fd, std::string msg);
        // what i add to Channels and my command parts
        int     findFdByNick(std::string nick);
        void    removeFromAllChannels(int fd);
        void    handleCommand(int fd, std::string line);
        void    cmdJoin(int fd, std::vector<std::string> &params);
        void    cmdInvite(int fd, std::vector<std::string> &params);
        void    cmdKick(int fd, std::vector<std::string> &params);
        void    cmdPass(int fd, std::vector<std::string> &params);
        void    cmdUser(int fd, std::vector<std::string> &params);
        void    cmdNick(int fd, std::vector<std::string> &params);
        std::map<std::string, Channel> channels;
        void    pars(int fd);
        ~Server();
};

#endif