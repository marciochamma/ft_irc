/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchamma <mchamma@student.42sp.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 20:37:08 by mchamma           #+#    #+#             */
/*   Updated: 2025/02/08 20:33:11 by mchamma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

bool Server::cmdInviteCheckArgs(const std::vector<std::string>& args)
{
	if (args.size() != 2)
		return (false);

	if (!checkAlphanum(args[0], 0))
		return (false);

	if (!checkAlphanum(args[1], 1))
		return (false);

	return (true);
}

bool Server::cmdInviteCheck(const std::vector<std::string>& args, int clientFd)
{
	if (!cmdInviteCheckArgs(args))
		return (notify(clientFd, WHI, 2, 1, 1, "error : check '/help invite'"));

	Client* invitor = getClientByFd(clientFd);
	Client* invitee = getClientByNick(args[0]);
	Channel* channel = getChannelByName(args[1]);

	if (!channel)
		return (notify(clientFd, WHI, 2, 1, 1, "can't invite; channel doesn't exist"));

	if (!channel->isClientOnChannel(invitor))
		return (notify(clientFd, WHI, 2, 1, 1, "can't invite; you haven't joined this channel"));

	if (!channel->isOperatorOnChannel(invitor) && channel->getOneMode("o"))
		return (notify(clientFd, WHI, 2, 1, 1, "can't invite; you're not channel's operator"));

	if (!invitee)
		return (notify(clientFd, WHI, 2, 1, 1, "can't invite; this user doesn't exist"));

	return (true);
}

void Server::cmdInvite(const std::vector<std::string>& args, int clientFd)
{
	if (!cmdInviteCheck(args, clientFd))
		return ;

	Client* invitee = getClientByNick(args[0]);
	Channel* channel = getChannelByName(args[1]);

	channel->addInvitee(args[0]);
	notify (clientFd, WHI, 2, 1, 1, "invited " +args[0] +" to the channel");
	notify (clientFd, WHI, 4, 1, 1, "invited " +args[0] +" to the channel", channel);

	std::string note = forgeNote(clientFd, WHI, 4, 1, 1, "invited you to join " +channel->getName(), channel);
	send(invitee->getFd(), note.c_str(), note.length(), 0);
}
