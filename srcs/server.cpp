#include "../includes/server.hpp"

bool run = true;

void    signalHandler(int sig)
{
    (void)sig;
    run = false;
}

Server::Server() {}

Server::Server(const Server &x)
{
    *this = x;
}

Server& Server::operator=(const Server &x) 
{
    (void)x;
    return *this;
}

void    Server::Check(std::string port, std::string pass)
{
    if (port.empty())
        throw std::runtime_error("port number cannot be empty\n");
    if (pass.empty())
        throw std::runtime_error("passeord cannot be empty\n");
    if (port.find_first_not_of("0123456789") != std::string::npos)
        throw (std::runtime_error("Port Number must be valid integer\n"));
    long n = std::strtol(port.c_str(), NULL, 10);
    if (n < 0 || n > 65535)
        throw std::runtime_error("Port must be in [0, 65535]\n");
    this->port = static_cast<int>(n);
    pw = pass;
}

Server::Server(std::string p, std::string s)
{
    Check(p, s);
    ServSock();
    std::cout<<"irc Server "<<ss<<" connetcted ...\n";
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);
    signal(SIGQUIT, signalHandler);
    signal(SIGPIPE, SIG_IGN);
    std::vector<struct epoll_event> evs(16);
    while (run)
    {
        int n = epoll_wait(epollFd, evs.data(), 16, 420);
        if (n < 0)
        {
            if (errno == EINTR)
            std::cerr<<"epoll_wait failed\n";
            break;
        }
        for (int i = 0; i < n; ++i)
        {
            int fd = evs[i].data.fd;
            int revs = evs[i].events;
            if (fd == ss)
            {
                if (revs & EPOLLIN)
                    AccClient();
            }
            else if (revs & (EPOLLHUP | EPOLLERR))
                rmvClient(fd);
            else
            {
                if (revs & EPOLLIN)
                    clientread(fd);
                if (revs & EPOLLOUT)
                    clientwrite(fd);
            }
        }
        if (n == (int)evs.size())
            evs.resize(evs.size() * 2);
    }
    std::cout<<"irc server shariott shutting down\n";
}

void   Server::sendToClient(int fd, std::string msg)
{
    std::map<int, Client>::iterator i = cls.find(fd);
    if (i == cls.end())
        return;
    i->second.getsend() += msg;
    clearEpoll(fd, true);
}

void    Server::AccClient()
{
    struct sockaddr_in x;
    socklen_t len = sizeof(x);
    int fd = accept(ss, (struct sockaddr*)&x, &len);
    if (fd < 0)
        return;
    if (fcntl(fd, F_SETFL, O_NONBLOCK) < 0)
    {
        std::cerr<<"fcntl failed\n";
        close(fd);
        return;
    }
    struct epoll_event evn;
    evn.events = EPOLLIN;
    evn.data.fd = fd;
    if (epoll_ctl(epollFd, EPOLL_CTL_ADD, fd, &evn) < 0)
    {
        std::cerr<<"epoll_ctl failed\n";
        close(fd);
        return;
    }
    std::string ip = inet_ntoa(x.sin_addr);
    cls.insert(std::make_pair(fd, Client(fd, ip)));
    std::cout<<"New client connected fd = "<<fd<<" ip= "<<ip<<std::endl;
}

void Server::clientwrite(int fd)
{
    std::map<int, Client>::iterator i = cls.find(fd);
    if (i == cls.end())
        return;
    std::string &buff = i->second.getsend();
    if (buff.empty())
    {
        clearEpoll(fd, false);
        return ;
    }
    ssize_t r = send(fd, buff.c_str(), buff.size(),0);
    if (r < 0)
    {
        if (errno == EAGAIN || errno == EWOULDBLOCK)
            return;
        rmvClient(fd);
        return;
    }
    buff.erase(0, r);
    if (buff.empty())
        clearEpoll(fd, false);
}

void Server::clearEpoll(int fd, bool x)
{
    struct epoll_event ev;
    ev.data.fd = fd;
    ev.events = EPOLLIN;
    if (x)
        ev.events = EPOLLIN | EPOLLOUT;
    epoll_ctl(epollFd, EPOLL_CTL_MOD, fd, &ev);
}

void Server::clientread(int fd)
{
    std::map<int, Client>::iterator i = cls.find(fd);
    if (i == cls.end())
        return;
    char buff[4096];
    ssize_t r = recv(fd, buff, sizeof(buff), 0);
    if (r == 0)
    {
        rmvClient(fd);
        return;
    }
    if (r < 0)
    {
        if (errno == EAGAIN || errno == EWOULDBLOCK)
            return;
        rmvClient(fd);
        return;
    }
    i->second.getrecv().append(buff, r);
    pars(fd);
}

void Server::rmvClient(int fd)
{
    std::map<int, Client>::iterator i = cls.find(fd);
    if (i == cls.end())
        return ;
    std::cout<<"client disconnected\n";
    epoll_ctl(epollFd, EPOLL_CTL_DEL, fd, NULL);
    close(fd);
    cls.erase(fd);
}

void Server::ServSock()
{
    struct sockaddr_in x;
    x.sin_family = AF_INET;
    x.sin_port = htons(port);
    x.sin_addr.s_addr = INADDR_ANY;
    ss = socket(AF_INET, SOCK_STREAM, 0);
    if (ss < 0)
        throw std::runtime_error("socket creation failed\n");
    int op = 1;
    setsockopt(ss, SOL_SOCKET, SO_REUSEADDR, &op, sizeof(op));
    if (fcntl(ss, F_SETFL, O_NONBLOCK) < 0)
        throw std::runtime_error("fcntl failed\n");
    if (bind(ss, (struct sockaddr *)&x, sizeof(x)) < 0)
        throw std::runtime_error("bind failed\n");
    if (listen(ss, SOMAXCONN) < 0)
        throw std::runtime_error("listen failed\n");
    epollFd = epoll_create1(0);
    if (epollFd < 0)
        throw std::runtime_error("epoll_create falied\n");
    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = ss;
    if (epoll_ctl(epollFd, EPOLL_CTL_ADD, ss, &ev) < 0)
        throw std::runtime_error("epoll_ctl failed\n");
}

void Server::pars(int fd)
{
    std::map<int, Client>::iterator i = cls.find(fd);
    if (i == cls.end())
        return;
    std::string &buf = i->second.getrecv();
    //parsing the buf and dispatch the cammands
    std::cout<<buf;
}

Server::~Server()
{
    for (std::map<int, Client>::iterator i = cls.begin(); i != cls.end(); ++i)
        close(i->first);
    if (ss)
        close(ss);
    if (epollFd)
        close(epollFd);
}