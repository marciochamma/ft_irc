/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchamma <mchamma@student.42sp.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 21:31:04 by mchamma           #+#    #+#             */
/*   Updated: 2025/02/11 22:40:48 by mchamma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

bool Server::cmdQuitCheckArgs(const std::vector<std::string>& args)
{
	if (args.size() != 1)
		return (false);

	if (!checkAlphanum(args[0], 0))
		return (false);

	return (true);
}

bool Server::cmdQuitCheck(const std::vector<std::string>& args, int clientFd)
{
	if (!cmdNickCheckArgs(args))
		return (notify(clientFd, WHI, 2, 1, 1, "error : check '/help quit'"));

	return (true);
}

void Server::cmdQuit(const std::vector<std::string>& args, int clientFd)
{
	if (args.size() == 1)
		notify(clientFd, WHI, 2, 1, 1, "quit msg: " + args[0]);

	disconnectClient(clientFd);
}