/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajuliao- <ajuliao-@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 19:48:36 by mchamma           #+#    #+#             */
/*   Updated: 2025/04/07 22:10:18 by ajuliao-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

bool Server::cmdUserCheckArgs(const std::vector<std::string>& args)
{
	if (args.size() == 0)
		return (false);

	if (!checkAlphanum(args[0], 0))
		return (false);

	return (true);
}

bool Server::cmdUserCheck(const std::vector<std::string>& args, int clientFd)
{
	if (!cmdUserCheckArgs(args))
	{
		Client* client = getClientByFd(clientFd);
		std::string currentUser = client->getUser();
		return(notify(clientFd, WHI, 2, 1, 1, "Your user is: " + currentUser));
	}

	std::vector<Client*>::iterator it;
	for (it = this->_clients.begin(); it != this->_clients.end(); ++it)
		if ((*it)->getUser() == args[0])
			return (notify(clientFd, WHI, 2, 1, 1, "can't set user; this user was already taken"));

	return (true);
}

void Server::cmdUser(const std::vector<std::string>& args, int clientFd)
{
	if (!cmdUserCheck(args, clientFd))
		return ;

	Client* client = getClientByFd(clientFd);
	std::string oldUser = client->getUser();

	client->setUser(args[0]);
	notify (clientFd, WHI, 2, 1, 1, "set new user: " +args[0]);

	std::vector<Channel*>::iterator it;
	for (it = this->_channels.begin(); it != this->_channels.end(); ++it)
		if ((*it)->isClientOnChannel(client))
			notify(clientFd, WHI, 4, 1, 1, "changed the user from " +oldUser +" to " +args[0], *it);
}
