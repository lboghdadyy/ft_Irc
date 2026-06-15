#include "Server.hpp"
#include <cstdlib>   // atoi

void parseInput(std::string port, std::string password, int& p, std::string &pass) {
    std::string::size_type found = port.find_first_not_of("0123456789");
    if (port.empty() || found != std::string::npos)
        throw "Invalid port";
    int num = std::atoi(port.c_str());
    if (num <= 0 || num > 65535)
        throw "Invalid port";
    if (password.empty())
        throw "Invalid Password";
    for (size_t i = 0; i < password.length(); i++)
        if (!isprint(password[i]))
            throw "Invalid Password";
    p = num;
    pass = password;
}

int main(int ac, char **av)
{
    if (ac != 3) {
        std::cerr << "Usage: ./ircserv <port> <password>" << std::endl;
        return 1;
    }
    try {
        int port;
        std::string password;
        parseInput(av[1], av[2], port, password);
        // lkhedma d sbouaa . 9ad l OCF d server
        // Server server(port, password);
        // server.run();
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    } catch (char const *e) {
        std::cerr << "Error: " << e << std::endl;
        return 1;
    }

    return 0;
}