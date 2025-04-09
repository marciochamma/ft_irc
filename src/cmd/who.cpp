/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   who.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajuliao- <ajuliao-@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 22:43:31 by mchamma           #+#    #+#             */
/*   Updated: 2025/04/09 20:54:27 by ajuliao-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

bool Server::cmdWhoCheckArgs(const std::vector<std::string>& args)
{
	if (args.size() > 1)
		return (false);

	return (true);
}

bool Server::cmdWhoCheck(const std::vector<std::string>& args, int clientFd)
{
	bool isIRC = getClientByFd(clientFd)->isIRCClient();

	if (!cmdWhoCheckArgs(args))
		return (notify(clientFd, WHI(isIRC), 2, 1, 1, "error : check '/help Who'"));

	// Client* client = getClientByFd(clientFd);

	std::vector<Client*>::iterator it;
	for (it = this->_clients.begin(); it != this->_clients.end(); ++it)
	{

		notify(clientFd, WHI(isIRC), 2, 1, 1, (*it)->getNick()); //colocar mais coisas ipaddres username
	}

	return (true);
}

void Server::cmdWho(const std::vector<std::string>& args, int clientFd)
{


	Client* client = getClientByFd(clientFd);
	bool isIRC = client->isIRCClient();

	if (!client->getCurrentChannel() || client->getCurrentChannel()->getName().empty())
	{
		notify(clientFd, WHI(isIRC), 2, 1, 1, "can't verify users; you're not active in any channel ");
		return ;
	}

	if (!cmdWhoCheck(args, clientFd))
		return ;
}
