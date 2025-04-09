/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajuliao- <ajuliao-@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 17:45:02 by mchamma           #+#    #+#             */
/*   Updated: 2025/04/09 20:45:39 by ajuliao-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

std::string Server::cmdListBody(Channel* channel, Client* client, \
	Channel* currentChannel, bool hasCurrentChannel)
{
	std::string isJoined = channel->isClientOnChannel(client) ? "yes" : "no";
	std::string isOperator = channel->isOperatorOnChannel(client) ? "yes" : "no";

	std::string isActive;
	if (channel->isClientOnChannel(client) && hasCurrentChannel && \
		channel->getName() == currentChannel->getName())
	{isActive = "yes";}
		else {isActive = "no";}

	std::string modeInvite = channel->getOneMode("i") ? "+" : "-";
	std::string modeTopic = channel->getOneMode("t") ? "+" : "-";
	std::string modeKey = channel->getOneMode("k") ? "+" : "-";
	std::string modeOperator = channel->getOneMode("o") ? "+" : "-";
	std::string modeLimit = channel->getOneMode("l") ? "+" : "-";

	std::stringstream ss;
	ss	<<std::left
		<<std::setw(6) <<channel->getName().substr(0, 6)
		<<"  " <<std::setw(3) <<(intToStr(channel->getClients().size())).substr(0, 3)
		<<"  " <<std::setw(4) <<isJoined.substr(0, 4)
		<<"  " <<std::setw(4) <<isOperator.substr(0, 4)
		<<"  " <<std::setw(4) <<isActive.substr(0, 4)
		<<"  " <<std::setw(1) <<modeInvite.substr(0, 1)
		<<"  " <<std::setw(1) <<modeTopic.substr(0, 1)
		<<"  " <<std::setw(1) <<modeKey.substr(0, 1)
		<<"  " <<std::setw(1) <<modeOperator.substr(0, 1)
		<<"  " <<std::setw(1) <<modeLimit.substr(0, 1)
		<<"  " <<std::setw(20) <<channel->getTopic().substr(0, 20);

	return ss.str();
}

std::string cmdListHeader(void)
{
	std::stringstream ss;
	ss	<< std::left << std::setw(6) << "Name"
		<<"  " <<std::setw(3) << "Usr"
		<<"  " <<std::setw(4) << "Join"
		<<"  " <<std::setw(4) << "Oper"
		<<"  " <<std::setw(4) << "Actv"
		<<"  " <<std::setw(1) << "i"
		<<"  " <<std::setw(1) << "t"
		<<"  " <<std::setw(1) << "k"
		<<"  " <<std::setw(1) << "o"
		<<"  " <<std::setw(1) << "l"
		<<"  " <<std::setw(20) << "Topic";
	return ss.str();
}

bool Server::cmdListCheck(const std::vector<std::string>& args, int clientFd)
{
	bool isIRC = getClientByFd(clientFd)->isIRCClient();

	if (!args.empty())
		return (notify(clientFd, WHI(isIRC), 2, 1, 1, "error : check '/help list'"));

	return (true);
}

void Server::cmdList(const std::vector<std::string>& args, int clientFd)
{
	if (!cmdListCheck(args, clientFd))
		return ;

	Client* client = getClientByFd(clientFd);
	bool isIRC = client->isIRCClient();

	Channel* currentChannel = client->getCurrentChannel();
	bool hasCurrentChannel = (currentChannel != NULL);

	notify(clientFd, WHI(isIRC), 2, 1, 1, "List of channels");
	notify(clientFd, WHI(isIRC), 2, 0, 1, cmdListHeader().c_str());

	std::vector<Channel*>::iterator it;
	for (it = this->_channels.begin(); it != this->_channels.end(); ++it)
		notify(clientFd, WHI(isIRC), 2, 0, 1, cmdListBody(*it, client, \
			currentChannel, hasCurrentChannel).c_str());
}
