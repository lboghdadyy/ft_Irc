# include "../../includes/server.hpp"
# include "../../includes/channels.hpp"

void    Server::cmdQuit(int fd, std::vector<std::string> &params)
{
	std::set<int>	history;
	Client			_client = cls[fd];
	std::set<int>	_members;

	if (_client.isRegistered())
		return sendToClient(fd, "451 " + cls[fd].getNick() + " :You have not registered\r\n");
	for (std::map<std::string, Channel>::iterator it = channels.begin();
	 it != channels.end(); it++)
	{
		if (it->second.isMember(fd))
		{
			_members = it->second.getMembers();
			for (std::set<int>::iterator _memberIt = _members.begin();
				_memberIt != _members.end(); _memberIt++)
			{
				if (*_memberIt != fd && history.find(*_memberIt) == _members.end())
				{
					sendToClient(*_memberIt, ":" + cls[fd].getNick() + " QUIT :" + params[1] + '\n');
					history.insert(*_memberIt);
				}
			}
			it->second.removeMember(fd);
		}
	}
}