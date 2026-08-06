#include "../includes/server.hpp"
#include "../includes/channels.hpp"

Client::Client() {}

Client::Client(const Client &x)
{
    *this = x;
}

Client::Client(int f, std::string i) : fd(f), ip(i), passOk(false), hasNick(false), hasUser(false), welcomed(false) {
    std::stringstream ss;
    ss << "user" << f;
    std::string s = ss.str();
    nick = s;
}

Client& Client::operator=(const Client &x)
{
    if (this != &x) {
        fd = x.fd;
        ip = x.ip;
        recv = x.recv;
        send = x.send;
        nick = x.nick;
        passOk = x.passOk;
        hasNick = x.hasNick;
        hasUser = x.hasUser;
        welcomed = x.welcomed;
    }
    return *this;
}

int Client::getfd() { return fd; }

std::string& Client::getip() {return ip; }
std::string& Client::getrecv() {return recv; }
std::string& Client::getsend() {return send; }
Client::~Client() {}

std::string Client::getNick() const {
    return nick;
}

void Client::setNick(std::string n) {
    nick = n;
}

bool Client::isRegistered() const {
    return (passOk && hasNick && hasUser);
}

std::string Client::getUser() const {
    return user;
}
std::string Client::getRealname() const {
    return realname;
}
bool Client::getPassOk() const {
    return passOk;
}
bool Client::getHasNick() const {
    return hasNick;
}
bool Client::getHasUser() const {
    return hasUser;
}

bool Client::getWelcomed() const {
    return welcomed;
}
void Client::setWelcomed(bool w) {
    welcomed = w;
}

void Client::setUser(std::string u) {
    user = u;
}
void Client::setRealname(std::string r) {
    realname = r;
}
void Client::setPassOk(bool b) {
    passOk = b;
}
void Client::setHasNick(bool b) {
    hasNick = b;
}
void Client::setHasUser(bool b) {
    hasUser = b;
}
