#include "../includes/server.hpp"

Client::Client() {}

Client::Client(const Client &x)
{
    *this = x;
}

Client::Client(int f, std::string i) : fd(f), ip(i) {}

Client& Client::operator=(const Client &x)
{
    if (this != &x)
    {
        fd = x.fd;
        ip = x.ip;
        recv = x.recv;
        send = x.send;
    }
    return *this;
}

int Client::getfd() { return fd; }

std::string& Client::getip() {return ip; }

std::string& Client::getrecv() {return recv; }

std::string& Client::getsend() {return send; }

Client::~Client() {}