#include "../includes/server.hpp"

int main(int ac, char **av)
{
    if (ac != 3)
    {
        std::cout<<"Usage ./irc Port Password\n";
        return 1;
    }
    try 
    {
        Server serv(av[1], av[2]);
    }
    catch (std::exception &x)
    {
        std::cout<<x.what();
    }
    return 0;
}