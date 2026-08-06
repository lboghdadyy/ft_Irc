#ifndef CLIENT_HPP
# define CLIENT_HPP

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
#include <algorithm>
#include <map>
#include <sstream>
#include <set>

class Client {
    private :
        int                         fd;
        std::string                 ip;
        std::string                 recv;
        std::string                 send;
        std::string                 nick; //
        std::string                 user;
        std::string                 realname;
        bool                        passOk;
        bool                        hasNick;
        bool                        hasUser;
        bool                        welcomed;
    public :
        Client();
        Client(int f, std::string i);
        Client(const Client &x);
        Client& operator=(const Client &x);
        int getfd();
        std::string& getrecv();
        std::string& getsend();
        std::string& getip();
        ~Client();
        std::string getNick() const ; // down
        void        setNick(std::string n);
        std::string getUser() const;
        std::string getRealname() const;
        bool        getPassOk() const;
        bool        getHasNick() const;
        bool        getHasUser() const;
        bool        getWelcomed() const;
        void        setWelcomed(bool w);
        void        setUser(std::string u);
        void        setRealname(std::string r);
        void        setPassOk(bool b);
        void        setHasNick(bool b);
        void        setHasUser(bool b);
        bool        isRegistered() const;
        std::vector<std::string> &getChannels(void);
};

#endif