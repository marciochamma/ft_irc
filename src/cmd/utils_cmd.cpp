/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_cmd.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajuliao- <ajuliao-@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 18:41:27 by mchamma           #+#    #+#             */
/*   Updated: 2025/04/09 20:53:34 by ajuliao-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

bool Server::checkAlphanum(const std::string& str, int isChannel)
{
	if (str.length() < 2 || str.length() > 9)
		return (false);

	if (isChannel)
	{
		if (str[0] != '#')
			return (false);

		for (size_t i = 1; i < str.size(); i++)
			if (!std::isalnum(str[i]))
				return (false);
	}

	else
		for (size_t i = 0; i < str.size(); i++)
			if (!std::isalnum(str[i]))
				return (false);

	return (true);
}

bool Server::checkNumeric(const std::string& str)
{
	for (size_t i = 0; i < str.size(); i++)
		if (!std::isdigit(str[i]))
			return (false);

	if (atoi(str.c_str()) > 10000)
		return (false);

	return (true);
}

bool Server::checkPrintable(const std::string& str)
{
	if (str.length() < 2 || str.length() > 9)
		return (false);

	for (size_t i = 0; i < str.size(); i++)
		if (!std::isprint(str[i]))
			return (false);

	return (true);
}


// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

std::string Server::forgeNote(int fd, std::string color, int toWhom, int header, int footer, \
	const std::string& msg, Channel *channel)
{
	(void)toWhom;
	std::string note = color;

	if (header == 1)
	{
		note += TIME;

		if (channel)
			note += "[" +channel->getName() +"] ";

		if (fd == this->_servFd)
			note += "<Serv" +intToStr(this->_servFd) +"> ";
		else
			note += +"<" + getClientByFd(fd)->getNick() +"> ";
	};

	note += msg +WHI(false);

	if (footer == 1)
		note += "\r\n";
	else
		note += " ";

	return (note);
}

bool Server::notify(int fd, std::string color, int toWhom, int header, int footer, \
	const std::string& msg, Channel *channel)
{
	std::string note = forgeNote(fd, color, toWhom, header, footer, msg, channel);
	if (toWhom % 2 == 1) // server
		std::cout <<note;

	if (toWhom == 2 || toWhom == 3 || toWhom == 6 || toWhom == 7) // self
	{
			send(fd, note.c_str(), strlen(note.c_str()), 0);
	}

	if (toWhom >= 4) // others
		for (size_t i = 0; i < this->_clients.size(); ++i)
			if (channel->isClientOnChannel(this->_clients[i]) && this->_clients[i]->getFd() != fd)
				send(this->_clients[i]->getFd(), note.c_str(), strlen(note.c_str()), 0);

	return (0);
}
