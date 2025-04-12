/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   who.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 22:43:31 by mchamma           #+#    #+#             */
/*   Updated: 2025/04/12 14:35:03 by user42           ###   ########.fr       */
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
	if (!cmdWhoCheckArgs(args))
		return (notify(clientFd, WHI, 2, 1, 1, "error : check '/help who'"));

	std::vector<Client*>::iterator it;
	for (it = this->_clients.begin(); it != this->_clients.end(); ++it)
	{

		notify(clientFd, WHI, 2, 1, 1, (*it)->getNick());
	}

	return (true);
}

void Server::cmdWho(const std::vector<std::string>& args, int clientFd)
{

	
	Client* client = getClientByFd(clientFd);
	
	if (!client->getCurrentChannel() || client->getCurrentChannel()->getName().empty())
	{
		notify(clientFd, WHI, 2, 1, 1, "can't verify users; you're not active in any channel ");
		return ;
	}
	
	if (!cmdWhoCheck(args, clientFd))
		return ;
}