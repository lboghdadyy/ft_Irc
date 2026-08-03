#include "../includes/server.hpp"
#include "../includes/channels.hpp"

bool running = true;

void    signalHandler(int sig)
{
    (void)sig;
    running = false;
}

Server::Server() : ss(-1), port(0), epollFd(-1) {}

Server::Server(const Server &x)
{
    *this = x;
}

Server& Server::operator=(const Server &x)
{
    (void)x;
    return *this;
}

Server::Server(std::string p, std::string s) : ss(-1), port(0), epollFd(-1)
{
    Check(p, s);
    ServSock();
    std::cout << "irc Server " << ss << " connected...\n";
    signal(SIGINT, signalHandler);
    signal(SIGQUIT, signalHandler);
    run();
    std::cout << "\nIRC Server shutting down\n";
}

int    Server::run()
{
    std::vector<struct epoll_event> evs(64);
    while (running)
    {
        int n = epoll_wait(epollFd, evs.data(), evs.size(), -1);
        if (n < 0)
        {
            if (errno != EINTR)
            {
                std::cerr << "epoll_wait failed\n";
                break;
            }
            continue;
        }
        for (int i = 0; i < n; ++i)
        {
            int fd = evs[i].data.fd;
            int revs = evs[i].events;
            if (fd == ss)
            {
                if (revs & EPOLLIN)
                    AccClient();
                continue;
            }
            if (revs & (EPOLLHUP | EPOLLERR))
            {
                rmvClient(fd);
                continue;
            }
            if (revs & EPOLLIN)
            {
                if (!clientread(fd))
                    continue;
            }
            if (revs & EPOLLOUT)
                clientwrite(fd);
        }
        if (n == static_cast<int>(evs.size()))
            evs.resize(evs.size() * 2);
    }
    return 0;
}

void    Server::AccClient()
{
    struct sockaddr_in x;
    socklen_t len = sizeof(x);
    std::memset(&x, 0, sizeof(x));
    int fd = accept(ss, (struct sockaddr*)&x, &len);
    if (fd < 0)
    {
        std::cerr<<"accept failed\n";
        return;
    }
    if (fcntl(fd, F_SETFL, O_NONBLOCK) < 0)
    {
        std::cerr<<"fcntl failed\n";
        close(fd);
        return;
    }
    struct epoll_event evn;
    std::memset(&evn, 0, sizeof(evn));
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
        return;
    }
    ssize_t r = send(fd, buff.c_str(), buff.size(), 0);
    if (r <= 0)
    {
        rmvClient(fd);
        return;
    }
    buff.erase(0, r);
    if (buff.empty())
        clearEpoll(fd, false);
}

int Server::clientread(int fd)
{
    std::map<int, Client>::iterator i = cls.find(fd);
    if (i == cls.end())
        return 0;
    char buff[4096];
    ssize_t r = recv(fd, buff, sizeof(buff), 0);
    if (r <= 0)
    {
        rmvClient(fd);
        return 0;
    }
    i->second.getrecv().append(buff, r);
    try
    {
        pars(fd);
    }
    catch(const std::exception& e)
    {
        rmvClient(fd);
        return (0);
    }
    return 1;
}

void Server::ServSock()
{
    struct sockaddr_in x;
    std::memset(&x, 0, sizeof(x));
    x.sin_family = AF_INET;
    x.sin_port = htons(port);
    x.sin_addr.s_addr = INADDR_ANY;
    ss = socket(AF_INET, SOCK_STREAM, 0);
    if (ss < 0)
        throw std::runtime_error("socket creation failed\n");
    int op = 1;
    if (setsockopt(ss, SOL_SOCKET, SO_REUSEADDR, &op, sizeof(op)) < 0)
        throw std::runtime_error("setsockopt failed\n");
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
    std::memset(&ev, 0, sizeof(ev));
    ev.events = EPOLLIN;
    ev.data.fd = ss;
    if (epoll_ctl(epollFd, EPOLL_CTL_ADD, ss, &ev) < 0)
        throw std::runtime_error("epoll_ctl failed\n");
}

Server::~Server()
{
    for (std::map<int, Client>::iterator i = cls.begin(); i != cls.end(); ++i)
        close(i->first);
    if (ss >= 0)
        close(ss);
    if (epollFd >= 0)
        close(epollFd);
}
