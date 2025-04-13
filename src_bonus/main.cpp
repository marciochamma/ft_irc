/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fde-alen <fde-alen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/01 12:31:58 by mchamma           #+#    #+#             */
/*   Updated: 2025/04/12 23:10:29 by fde-alen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

bool errorUsage(size_t index)
{
	if (index == 1)
		std::cout <<"USAGE: ./ircserv <port number> <password>" <<std::endl;
	else if (index == 2)
		std::cout <<"USAGE: ./ircserv <port number> <password>; Port max 5 digits." <<std::endl;
	else if (index == 3)
		std::cout <<"USAGE: ./ircserv <port number> <password>; Port only digits." <<std::endl;
	else if (index == 4)
		std::cout <<"USAGE: ./ircserv <port number> <password>; Port from 1024 to 65535>" <<std::endl;
	return (false);
}

bool validArgs(int argc, char **argv)
{
	if (argc != 3)
		return (errorUsage(1));

	std::string port = argv[1];
	if (port.empty() || port.length() > 5)
		return (errorUsage(2));

	for (size_t i = 0; i < port.length(); ++i)
		if (!std::isdigit(port[i]))
			return (errorUsage(3));

	int port_num = std::atoi(port.c_str());
	if (port_num <= 1024 || port_num > 65535)
		return (errorUsage(4));

	return (true);
}

int main(int argc, char **argv)
{
	if (!validArgs(argc, argv))
		return (1);

	signal(SIGINT, Server::signalHandler);
	signal(SIGQUIT, Server::signalHandler);

	Server obelix;
	try
	{
		obelix.initServer(std::atoi(argv[1]), argv[2]);
		obelix.initBot();
		obelix.runServer();
	}
	catch(const std::exception& e)
	{
		obelix.closeAllFds();
		std::cerr << e.what() << std::endl;
	}
	return (0);
}
