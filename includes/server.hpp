#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <vector>
#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <fcntl.h> 
#include <poll.h>
#include <sys/epoll.h>
#include <signal.h>
#include <arpa/inet.h>
#include <stdexcept>
#include <cstdlib>
#include <errno.h>
#include <map>

class Channel;
// # include "channels.hpp"

class Client
{
    private :
        int fd;
        std::string ip;
        std::string recv;
        std::string send;
        std::string nick; //
    public :
        Client();
        Client(int f, std::string i);
        Client(const Client &x);
        Client& operator=(const Client &x);
        int getfd();
        std::string& getrecv();
        std::string& getNick(); // 
        std::string& getsend();
        std::string& getip();
        ~Client();
};


class Server
{
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
        void    handleCommand(int fd, std::string line);
        void    cmdJoin(int fd, std::vector<std::string> &params);
        void    cmdInvite(int fd, std::vector<std::string> &params);
        void    cmdKick(int fd, std::vector<std::string> &params);
        std::map<std::string, Channel> channels;
        void    removeFromAllChannels(int fd);
        void    pars(int fd);
        ~Server();
};


#endif