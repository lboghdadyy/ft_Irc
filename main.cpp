#include "includes/server.hpp"

int main(int ac, char **av)
{
    if (ac != 3) {
        std::cerr << "Usage: ./ircserv <port> <password>" << std::endl;
        return 1;
    }
    try {
        Server serv(av[1], av[2]);
    } catch (std::exception &e) {
        std::cerr << "Error: " << e.what();
    }
    return 0;
}