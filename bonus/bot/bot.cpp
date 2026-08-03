#include "bot.hpp"

Bot::Bot(std::string &h, int p, std::string &pass, std::string &n, std::string &c)
{
    host = h;
    port = p;
    password = pass;
    nick = n;
    channel = c;
    user = nick;
}

Bot::Bot(const Bot &x)
{
    *this = x;
}

Bot& Bot::operator=(const Bot &x)
{
    (void)x;
    return *this;
}

bool    Bot::sendline(const std::string &x)
{
    std::string buf = x + "\r\n";
    ssize_t s = send(sfd, buf.c_str(), buf.size(), 0);
    if (s < 0)
    {
        if (errno == EAGAIN || errno == EWOULDBLOCK)
            return true;
        std::cerr<<"Bot : send() failed\n";
        return false;
    }
    return true;
}

void    Bot::auth()
{
    sendline("PASS " + password);
    sendline("NICK " + nick);
    sendline("USER " + user + "0 * :" + user + " bot");
    sendline("JOIN " + channel);
}

bool Bot::loop()
{
    std::cout<<"Bot is running ...\n";
    std::cout<<"Bot commands: ................?...?..?..?.\n";

    char buff[4096];
    while (1)
    {
        ssize_t r = recv(sfd, buff, sizeof(buff), 0);
        if (r == 0)
        {
            std::cerr << "Bot : server closed connection\n";
            return false;
        }
        if (r < 0)
        {
            std::cerr << "Bot : recv() failed\n";
            return false;
        }
        recvBuff.append(buff, r);
        //do parsing and next implemment bot utilties  !!!!!!!!!!!!!!!!!!!!!!
    }
    return true;
}

bool Bot::run()
{
    struct sockaddr_in x;
    std::memset(&x, 0, sizeof(x));
    x.sin_family = AF_INET;
    x.sin_port = htons(port);
    x.sin_addr.s_addr = inet_addr(host.c_str());
    if (x.sin_addr.s_addr == INADDR_NONE)
    {
        std::cerr<<"Bot : host must be a valid IPv4 address\n";
        return false;
    }
    sfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sfd < 0)
    {
        std::cerr<<"Bot : socket failed\n";
        return false;
    }
    if (connect(sfd, (struct sockaddr *)&x, sizeof(x)) < 0)
    {
        std::cerr<<"Bot : connect() failed\n";
        return false;
    }
    std::cout << "Bot: connected to " <<host<<std::endl;
    auth();
    return loop();
}

Bot::~Bot()
{
    if (sfd >= 0)
        close(sfd);
    std::cout<<"Bot disconnected\n";
}