/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajuliao- <ajuliao-@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 22:43:31 by mchamma           #+#    #+#             */
/*   Updated: 2025/04/09 20:49:51 by ajuliao-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

bool Server::cmdNickCheckArgs(const std::vector<std::string>& args)
{
	if (args.size() != 1)
		return (false);

	if (!checkAlphanum(args[0], 0))
		return (false);

	return (true);
}

bool Server::cmdNickCheck(const std::vector<std::string>& args, int clientFd)
{
	bool isIRC = getClientByFd(clientFd)->isIRCClient();

	if (!cmdNickCheckArgs(args))
	{
		Client* client = getClientByFd(clientFd);
		std::string currentNick = client->getNick();
		return (notify(clientFd, WHI(isIRC), 2, 1, 1, "Your nick is: " + currentNick));
	}

	std::vector<Client*>::iterator it;
	for (it = this->_clients.begin(); it != this->_clients.end(); ++it)
		if ((*it)->getNick() == args[0])
			return (notify(clientFd, WHI(isIRC), 2, 1, 1, "can't set nick; this nick was already taken"));

	return (true);
}

void Server::cmdNick(const std::vector<std::string>& args, int clientFd)
{
	bool isIRC = getClientByFd(clientFd)->isIRCClient();

	if (!cmdNickCheck(args, clientFd))
		return ;

	Client* client = getClientByFd(clientFd);
	std::string oldNick = client->getNick();

	client->setNick(args[0]);
	notify (clientFd, WHI(isIRC), 2, 1, 1, "set new nick: " +args[0]);

	std::vector<Channel*>::iterator it;
	for (it = this->_channels.begin(); it != this->_channels.end(); ++it)
		if ((*it)->isClientOnChannel(client))
			notify(clientFd, WHI(isIRC), 4, 1, 1, "changed the nick from " +oldNick +" to " +args[0], *it);
}
