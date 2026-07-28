# include "../../includes/server.hpp"
# include "../../includes/channels.hpp"

void    Server::cmdQuit(int fd, std::vector<std::string> &params)
{
	std::vector<int>	history;
	Client			_client = cls[fd];
	std::set<int>	_members;

	history.clear();
	if (params.empty())
		params.push_back("");
	if (!_client.isRegistered())
		return sendToClient(fd, "451 " + cls[fd].getNick() + " :You have not registered\r\n");
	for (std::map<std::string, Channel>::iterator it = channels.begin();
	 it != channels.end(); it++) {
		if (it->second.isMember(fd)) {
			std::cout << "channel : " << it->second.getName() << std::endl;
			_members = it->second.getMembers();
			for (std::set<int>::iterator _memberIt = _members.begin();
				_memberIt != _members.end(); _memberIt++) {
				if (*_memberIt != fd && std::find(history.begin(), history.end(), *_memberIt) == history.end())
					history.push_back(*_memberIt);
			}
			if (it->second.isOperator(fd) && it->second.getNumOpera() == 1)
				it->second.assignNewOp(fd);
		}
	}
	for (std::vector<int>::iterator it = history.begin(); it != history.end(); it++)
	{
		sendToClient(*it, ":" + cls[fd].getNick() + "!user@" + cls[fd].getip() + " QUIT :" + params[0] + '\n');
	}
	throw(std::runtime_error("quit"));
}

