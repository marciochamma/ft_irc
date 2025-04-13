/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   addbot.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fde-alen <fde-alen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 23:13:28 by fde-alen          #+#    #+#             */
/*   Updated: 2025/04/12 23:20:59 by fde-alen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

bool Server::cmdAddBotCheckArgs(const std::vector<std::string>& args)
{
    if (args.size() != 1)
        return false;

    if (!checkAlphanum(args[0], 1))
        return false;

    return true;
}

bool Server::cmdAddBotCheck(const std::vector<std::string>& args, int clientFd)
{
    if (!cmdAddBotCheckArgs(args))
        return notify(clientFd, WHI, 2, 1, 1, "error : check '/help addbot'");

    Channel* channel = getChannelByName(args[0]);
    if (!channel)
        return notify(clientFd, WHI, 2, 1, 1, "can't add bot; channel doesn't exist");

    Client* client = getClientByFd(clientFd);
    if (!channel->isClientOnChannel(client))
        return notify(clientFd, WHI, 2, 1, 1, "can't add bot; you haven't joined this channel");

    if (!channel->isOperatorOnChannel(client) && channel->getOneMode("o"))
        return notify(clientFd, WHI, 2, 1, 1, "can't add bot; you're not channel's operator");

		if (channel->isClientOnChannel(static_cast<Client*>(_bot)))
		return notify(clientFd, WHI, 2, 1, 1, "bot is already in this channel");

    return true;
}

void Server::cmdAddBot(const std::vector<std::string>& args, int clientFd)
{
    if (!cmdAddBotCheck(args, clientFd))
        return;

    Channel* channel = getChannelByName(args[0]);
    if (!_bot) {
        notify(clientFd, WHI, 2, 1, 1, "error: bot is not initialized");
        return;
    }
    
    _bot->joinChannel(this, channel->getName());
    notify(clientFd, WHI, 2, 1, 1, "added bot to " + channel->getName());
    notify(clientFd, WHI, 4, 1, 1, "added bot to channel", channel);
}
