/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchamma <mchamma@student.42sp.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 21:57:01 by mchamma           #+#    #+#             */
/*   Updated: 2025/02/08 20:34:14 by mchamma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

bool Server::cmdTopicCheckArgs(const std::vector<std::string>& args)
{
	if (args.size() < 1 || args.size() > 2)
		return (false);

	if (args.size() == 1)
		if (!checkAlphanum(args[0], 1))
			return (false);

	if (args.size() == 2)
		if (!checkPrintable(args[0]))
			return (false);

	return (true);
}

bool Server::cmdTopicCheck(const std::vector<std::string>& args, int clientFd)
{
	if (!cmdTopicCheckArgs(args))
		return (notify(clientFd, WHI, 2, 1, 1, "error : check '/help topic'"));

	Client* client = getClientByFd(clientFd);
	Channel* channel = getChannelByName(args[0]);

	if (args.size() == 1 && !channel)
		return (notify(clientFd, WHI, 2, 1, 1, "can't see topic; channel doesn't exist"));

	if (args.size() == 1 && !channel->isClientOnChannel(client))
		return (notify(clientFd, WHI, 2, 1, 1, "can't see topic; you haven't joined this channel"));

	if (args.size() == 2 && !channel)
		return (notify(clientFd, WHI, 2, 1, 1, "can't set topic; channel doesn't exist"));

	if (args.size() == 2 && !channel->isClientOnChannel(client))
		return (notify(clientFd, WHI, 2, 1, 1, "can't set topic; you haven't joined this channel"));

	if (args.size() == 2 && !channel->isOperatorOnChannel(client) && channel->getOneMode("t"))
		return (notify(clientFd, WHI, 2, 1, 1, "can't set topic; the topic can't be changed"));

	if (args.size() == 2 && !channel->isOperatorOnChannel(client) && channel->getOneMode("o"))
		return (notify(clientFd, WHI, 2, 1, 1, "can't set topic; you're not channel's operator"));

	return (true);
}

void Server::cmdTopic(const std::vector<std::string>& args, int clientFd)
{
	if (!cmdTopicCheck(args, clientFd))
		return ;

	Channel* channel = getChannelByName(args[0]);

	if (args.size() == 1)
		notify(clientFd, WHI, 2, 1, 1, "channel's topic: " +channel->getTopic());

	if (args.size() == 2)
	{
		channel->setTopic(args[1]);
		notify(clientFd, WHI, 2, 1, 1, "set new topic to this channel");
		notify(clientFd, WHI, 4, 1, 1, "set new topic to this channel", channel);
	}
}
