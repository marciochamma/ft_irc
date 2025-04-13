/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchamma <mchamma@student.42sp.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 21:40:10 by mchamma           #+#    #+#             */
/*   Updated: 2025/02/08 20:34:02 by mchamma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

bool Server::cmdPartCheckArgs(const std::vector<std::string>& args)
{
	if (args.size() != 1)
		return (false);

	if (!checkAlphanum(args[0], 1))
		return (false);

	return (true);
}

bool Server::cmdPartCheck(const std::vector<std::string>& args, int clientFd)
{
	if (!cmdPartCheckArgs(args))
		return (notify(clientFd, WHI, 2, 1, 1, "error : check '/help part'"));

	Client * client = getClientByFd(clientFd);
	Channel* channel = getChannelByName(args[0]);

	if (!channel)
		return (notify(clientFd, WHI, 2, 1, 1, "can't leave; channel doesn't exist"));

	if (!channel->isClientOnChannel(client))
		return (notify(clientFd, WHI, 2, 1, 1, "can't leave; you haven't joined this channel"));

	return (true);
}

void Server::cmdPart(const std::vector<std::string>& args, int clientFd)
{
	if (!cmdPartCheck(args, clientFd))
		return ;

	Client* client = getClientByFd(clientFd);
	Channel* channel = getChannelByName(args[0]);

	if (client->getCurrentChannel() && client->getCurrentChannel()->getName() == channel->getName())
		client->setCurrentChannel(0);
	channel->removeClient(client);

	notify(clientFd, WHI, 2, 1, 1, "left the channel");
	notify(clientFd, WHI, 4, 1, 1, "left the channel", channel);
}
