/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msg.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 00:29:07 by mchamma           #+#    #+#             */
/*   Updated: 2025/04/12 14:34:17 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void Server::sendMsg(const std::vector<std::string>& args, int receiverFd)
{
	for (size_t i = 1; i < args.size(); ++i)
	{
		std::string messagePart = args[i];

		if (i < args.size() - 1)
			messagePart += " ";

		send(receiverFd, messagePart.c_str(), messagePart.length(), 0);
	}
}

void Server::sendMsgPrivate(const std::vector<std::string>& args, int senderFd)
{
	Client* sender = getClientByFd(senderFd);
	Client* receiver = getClientByNick(args[0]);

	std::string header = WHI + TIME +"<" +sender->getNick() +"> says privately: ";
	std::string footer = WHI + "\n";

	send(receiver->getFd(), header.c_str(),header.length(), 0);
	sendMsg(args, receiver->getFd());
	send(receiver->getFd(), footer.c_str(),footer.length(), 0);
}

void Server::sendMsgChannel(const std::vector<std::string>& args, int senderFd)
{
	Channel* channel = getChannelByName(args[0]);
	Client* sender = getClientByFd(senderFd);

	std::string header = WHI + TIME +"[" +channel->getName() + "] " +"<" \
		+sender->getNick() +"> says: ";
	std::string footer = WHI + "\n";

	for (size_t i = 0; i < this->_clients.size(); ++i)
		if (this->_clients[i]->getFd() != sender->getFd() \
			&& channel->isClientOnChannel(this->_clients[i]))
		{
			send(this->_clients[i]->getFd(), header.c_str(),header.length(), 0);
			sendMsg(args, this->_clients[i]->getFd());
			send(this->_clients[i]->getFd(), footer.c_str(),footer.length(), 0);
		}
}

bool Server::cmdMsgCheckArgs(const std::vector<std::string>& args)
{
	if (args.size() == 0)
		return (false);

	if (!checkAlphanum(args[0], 1) && !checkAlphanum(args[0], 0))
		return (false);

	return (true);
}

bool Server::cmdMsgCheck(const std::vector<std::string>& args, int clientFd)
{
	if (!cmdMsgCheckArgs(args))
		return (notify(clientFd, WHI, 2, 1, 1, "error : check '/help msg'"));

	size_t argsChars = 0;
	std::vector<std::string>::const_iterator it;
	for (it = args.begin(); it != args.end(); ++it)
		argsChars += it->length();

	if (argsChars > 512)
		return (notify(clientFd, WHI, 2, 1, 1, "can't send msg; exceed limit of 512 chars"));

	if (args[0][0] == '#')
	{
		if (args[0][0] == '#' && !getChannelByName(args[0]))
			return (notify(clientFd, WHI, 2, 1, 1, "can't send msg; channel doesn't exist"));

		if (args[0][0] == '#' && !getChannelByName(args[0])->isClientOnChannel(getClientByFd(clientFd)))
			return (notify(clientFd, WHI, 2, 1, 1, "can't send msg; you haven't joined this channel"));
	}
	else
		if (!getClientByNick(args[0]))
			return (notify(clientFd, WHI, 2, 1, 1, "can't send msg; this user doesn't exist"));

	return (true);
}

void Server::cmdMsg(const std::vector<std::string>& args, int clientFd)
{
	if (!cmdMsgCheck(args, clientFd))
		return ;

	if (args[0][0] == '#')
	{
		sendMsgChannel(args, clientFd);
		notify(clientFd, WHI, 2, 0, 1, "  ↳ message delivered!");
	}

	else
	{
		sendMsgPrivate(args, clientFd);
		notify(clientFd, WHI, 2, 0, 1, "  ↳ private message delivered!");
	}
}
