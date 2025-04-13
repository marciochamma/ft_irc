/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cap.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchamma <mchamma@student.42sp.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 19:14:16 by mchamma           #+#    #+#             */
/*   Updated: 2025/02/11 17:16:42 by mchamma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void Server::cmdCap(const std::vector<std::string>& args, int clientFd)
{
	(void)args;
	(void)clientFd;
	Client* client = getClientByFd(clientFd);
	std::cout <<TIME <<"<" <<client->getNick() <<"> capability negotiation..\n";
	return ;
}

