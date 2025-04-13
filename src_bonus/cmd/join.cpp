/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 12:36:34 by mchamma           #+#    #+#             */
/*   Updated: 2025/04/12 14:31:41 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void Server::createChannel(const std::string& name)
{
	Channel* newChannel = new Channel(name);
	this->_channels.push_back(newChannel);
}

bool Server::cmdJoinCheckArgs(const std::vector<std::string>& args)
{
	if (args.size() < 1 || args.size() > 2)
		return (false);

	if (!checkAlphanum(args[0], 1))
		return (false);

	if (args.size() == 2)
		if (!checkAlphanum(args[1], 0))
			return (false);

	return (true);
}

bool Server::cmdJoinCheck(const std::vector<std::string>& args, int clientFd)
{
	if (!cmdJoinCheckArgs(args))
		return (notify(clientFd, WHI, 2, 1, 1, "error : check '/help join'"));

	Channel* channel = getChannelByName(args[0]);
	Client* client = getClientByFd(clientFd);

	if (!channel)
		return (true);

	if (channel->isClientOnChannel(client))
	{
		client->setCurrentChannel(channel);
		return (notify(clientFd, WHI, 2, 1, 1, "is active on this channel"));
	}

	if (!channel->getPwd().empty() && (args.size() != 2 || channel->getPwd() != args[1]) && channel->getOneMode("k"))
		return (notify(clientFd, WHI, 2, 1, 1, "can't join this channel; password incorrect"));

	if (args.size() == 2 && !channel->getOneMode("k"))
		return (notify(clientFd, WHI, 2, 1, 1, "can't join this channel; password not necessary"));

	if (!channel->isInvitee(client->getNick()) && channel->getOneMode("i"))
		return (notify(clientFd, WHI, 2, 1, 1, "can't join this channel; you're not in guest list"));

	if (channel->getClients().size() >= channel->getLimit() && channel->getOneMode("l"))
		return (notify(clientFd, WHI, 2, 1, 1, "can't join this channel; channel full"));

	return (true);
}

void Server::cmdJoin(const std::vector<std::string>& args, int clientFd)
{
	if (!cmdJoinCheck(args, clientFd))
		return ;

	Channel* channel = getChannelByName(args[0]);
	Client* client = getClientByFd(clientFd);
	bool isCreator = false;

	if (!channel)
	{
		createChannel(args[0]);
		isCreator = true;
		channel = getChannelByName(args[0]);
		if (args.size() == 2)
		{
			channel->setPwd(args[1]);
			channel->setOneMode("+k");
		}
		notify(clientFd, WHI, 2, 1, 1, "created " +channel->getName());
	}

	channel->addClient(client);
	notify(clientFd, WHI, 2, 1, 1, "joined to " + channel->getName());
	notify(clientFd, WHI, 4, 1, 1, "joined to channel", channel);

	if (isCreator)
	{
		channel->addOperator(client);
		notify(clientFd, WHI, 2, 1, 1, "named operator of " + channel->getName());
	}

	client->setCurrentChannel(channel);
	notify(clientFd, WHI, 2, 1, 1, "is now active on " + channel->getName());
}
