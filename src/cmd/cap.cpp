/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cap.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajuliao- <ajuliao-@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 19:14:16 by mchamma           #+#    #+#             */
/*   Updated: 2025/04/09 20:13:30 by ajuliao-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void Server::cmdCap(const std::vector<std::string>& args, int clientFd)
{
	(void)args;
	(void)clientFd;
	Client* client = getClientByFd(clientFd);
	client->setPwd("HEXCHAT");
	std::cout <<TIME <<"<" <<client->getNick() <<"> capability negotiation..\n";
	return ;
}

