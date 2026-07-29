# include "../../includes/server.hpp"
# include "../../includes/channels.hpp"


void    Server::cmdMode(int fd, std::vector<std::string> &params)
{
	int											modes;
	char										sign;
	size_t										paramIndex;
	std::map<std::string, Channel>::iterator	it;
	int											_targetFd;
	std::string									_charModes = "itklo";

	paramIndex = 2;
	if (params.size() < 2)
		return (sendToClient(fd, "461 " + cls[fd].getNick() + " MODE :Not enough parameters\n"));
	it = channels.find(params[0]);
	if (it == channels.end())
		return sendToClient(fd, "403 " + cls[fd].getNick() + " " + params[0] + " :No such channel\n");
	if (!it->second.isMember(fd))
		return (sendToClient(fd, "442 " + cls[fd].getNick() + " " + params[0] + " :You're not on that channel\n"));
	if (!it->second.isOperator(fd))
		return (sendToClient(fd, "482 " + cls[fd].getNick() + " " + params[0] + " :You're not channel operator\n"));
	sign = params[1][0];
	modes = 0;
	for (size_t index = 1; index < params[1].size(); index++)
	{
		if (modes == 3)
			return ;
		if (_charModes.find(params[1][index]) != std::string::npos)
			modes++;
		switch (params[1][index])
		{
			case 'i':
				if (sign == '+')
					it->second.setInviteOnly(true);
				else
					it->second.setInviteOnly(false);
				break;
			case 't':
				if (sign == '+')
					it->second.setTopicRestrict(true);
				else
					it->second.setTopicRestrict(false);
				break;
			case 'k':
				if (params.size() < 1 + paramIndex)
					return (sendToClient(fd, "461 " + cls[fd].getNick() + " MODE :Not enough parameters\n"));
				if (sign == '+')
					it->second.setKey(params[paramIndex++]);
				else
				{
					it->second.setKey("");
					paramIndex++;
				}
				break;
			case 'o':
				if (params.size() < 1 + paramIndex)
					return (sendToClient(fd, "461 " + cls[fd].getNick() + " MODE :Not enough parameters\n"));
				_targetFd = findFdByNick(params[paramIndex++]);
				if (_targetFd == -1)
					return sendToClient(fd, "401 " + cls[fd].getNick() + " " + params[paramIndex] + " :No such nick/channel\r\n");
				if (!it->second.isMember(_targetFd))
					return sendToClient(fd, "441 " + cls[fd].getNick() + " " + params[paramIndex] + " " + params[0] + " :They aren't on that channel\r\n");
				if (sign == '+')
					it->second.addOperator(_targetFd);
				else
					it->second.removeOperator(_targetFd);
				break;
			case 'l':
				if (sign == '+')
				{
					if (params.size() < 1 + paramIndex)
						return (sendToClient(fd, "461 " + cls[fd].getNick() + " MODE :Not enough parameters\n"));
					it->second.setUserLimit(std::atoi(params[paramIndex++].c_str()));
				}
				else
					it->second.setUserLimit(0);
				break;
			case '-':
				sign = '-';
				break ;
			case '+' :
				sign = '+';
				break;
			default:
				sendToClient(fd, "472 " + cls[fd].getNick() + " "  + params[1][index] + " :is unknown mode char to me for " + params[0] + '\n');
				break;
		}
	}
}