#include "../includes/server.hpp"
#include "../includes/channels.hpp"

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

void Server::clearEpoll(int fd, bool x)
{
    struct epoll_event ev;
    std::memset(&ev, 0, sizeof(ev));
    ev.data.fd = fd;
    ev.events = EPOLLIN;
    if (x)
        ev.events = EPOLLIN | EPOLLOUT;
    if (epoll_ctl(epollFd, EPOLL_CTL_MOD, fd, &ev) < 0)
        std::cerr << "epoll_ctl mod failed\n";
}

void Server::rmvClient(int fd)
{
    std::map<int, Client>::iterator i = cls.find(fd);
    if (i == cls.end())
        return;
    std::cout << "client disconnected\n";
    if (epoll_ctl(epollFd, EPOLL_CTL_DEL, fd, NULL) < 0)
        std::cerr << "epoll_ctl del failed\n";
    removeFromAllChannels(fd);
    close(fd);
    cls.erase(fd);
}

void   Server::sendToClient(int fd, std::string msg)
{
    std::map<int, Client>::iterator i = cls.find(fd);
    if (i == cls.end())
        return;
    i->second.getsend() += msg;
    clearEpoll(fd, true);
}