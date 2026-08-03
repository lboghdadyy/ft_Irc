#ifndef SERVER_HPP
#define SERVER_HPP

class Channel;
// # include "channels.hpp"
#include "client.hpp"
#include <string>


class Server {
	private :
		int ss;
		int port;
		int epollFd;
		std::string pw;
		std::map <int, Client> cls;
		Server(const Server &x);
		Server& operator=(const Server &x);
	public :
		Server();
		Server(std::string p, std::string s);
		void	Check(std::string p, std::string s);
		int		run();
		void	ServSock();
		void	AccClient();
		void	rmvClient(int fd);
		int		clientread(int fd);
		void	clientwrite(int fd);
		void	clearEpoll(int fd, bool x);
		void	sendToClient(int fd, std::string msg);

		// what i add to Channels and my command parts
		int		findFdByNick(std::string nick);
		void	removeFromAllChannels(int fd);
		void	handleCommand(int fd, std::string line);
		void	cmdJoin(int fd, std::vector<std::string> &params);
		void	cmdInvite(int fd, std::vector<std::string> &params);
		void	cmdKick(int fd, std::vector<std::string> &params);
		void	cmdPass(int fd, std::vector<std::string> &params);
		void	cmdUser(int fd, std::vector<std::string> &params);
		void	cmdNick(int fd, std::vector<std::string> &params);
		void	cmdPrivmsg(int fd, std::vector<std::string> &params);
		void	cmdQuit(int fd, std::vector<std::string> &params);
		void	cmdTopic(int fd, std::vector<std::string> &params);
		void    cmdMode(int fd, std::vector<std::string> &params);
		void	pars(int fd);
		std::string						findNickByfd(int fd);
		std::map<std::string, Channel>	channels;
		~Server();
};

#endif