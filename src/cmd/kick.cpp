/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajuliao- <ajuliao-@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 21:13:51 by mchamma           #+#    #+#             */
/*   Updated: 2025/04/09 20:44:56 by ajuliao-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

bool Server::cmdKickCheckArgs(const std::vector<std::string>& args)
{
	if (args.size() != 2)
		return (false);

	if (!checkAlphanum(args[0], 0))
		return (false);

	if (!checkAlphanum(args[1], 1))
		return (false);

	return (true);
}

bool Server::cmdKickCheck(const std::vector<std::string>& args, int clientFd)
{
	bool isIRC = getClientByFd(clientFd)->isIRCClient();
	if (!cmdKickCheckArgs(args))
		return (notify(clientFd, WHI(isIRC), 2, 1, 1, "error : check '/help kick'"));

	Client* kicker = getClientByFd(clientFd);
	Client* kicked = getClientByNick(args[0]);
	Channel* channel = getChannelByName(args[1]);

	if (!channel)
		return (notify(clientFd, WHI(isIRC), 2, 1, 1, "can't kick; channel doesn't exist"));

	if (!channel->isClientOnChannel(kicker))
		return (notify(clientFd, WHI(isIRC), 2, 1, 1, "can't kick; you haven't joined this channel"));

	if (!channel->isOperatorOnChannel(kicker) && channel->getOneMode("o"))
		return (notify(clientFd, WHI(isIRC), 2, 1, 1, "can't kick; you're not channel's operator"));

	if (!kicked)
		return (notify(clientFd, WHI(isIRC), 2, 1, 1, "can't kick; this user doesn't exist"));

	if (!channel->isClientOnChannel(kicked))
		return (notify(clientFd, WHI(isIRC), 2, 1, 1, "can't kick; this user does not joined to channel"));

	return (true);
}

void Server::cmdKick(const std::vector<std::string>& args, int clientFd)
{
	if (!cmdKickCheck(args, clientFd))
		return ;

	Client* kicked = getClientByNick(args[0]);
	Channel* channel = getChannelByName(args[1]);
	bool isIRC = getClientByFd(clientFd)->isIRCClient();
	channel->removeClient(kicked);

	notify(clientFd, WHI(isIRC), 2, 1, 1, "kicked " +kicked->getNick() +" from this channel");
	notify(clientFd, WHI(isIRC), 4, 1, 1, "kicked " +kicked->getNick(), channel);

	std::string note = forgeNote(clientFd, WHI(isIRC), 4, 1, 1, "kicked you from " +channel->getName(), channel);
	send(kicked->getFd(), note.c_str(), note.length(), 0);

}
