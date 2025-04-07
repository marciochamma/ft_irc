/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 19:10:00 by mchamma           #+#    #+#             */
/*   Updated: 2025/02/12 10:40:22 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

std::string Server::cmdFlagAction(const std::string& flag)
{
	std::map<std::string, std::string> cmdDetail;
	cmdDetail["-i"] ="removed invite-only to channel";
	cmdDetail["+i"] ="set invite-only to channel";
	cmdDetail["-t"] ="removed restrictions of the topic";
	cmdDetail["+t"] ="set restrictions of the topic";
	cmdDetail["-k"] ="removed mandatory channel password";
	cmdDetail["+k"] ="set mandatory channel password";
	cmdDetail["-o"] ="removed channel's operator privilege";
	cmdDetail["+o"] ="set channel's operator privilege";
	cmdDetail["-l"] ="removed user limit to channel";
	cmdDetail["+l"] ="set user limit to channel";

	std::map<std::string, std::string>::const_iterator it;
	it = cmdDetail.find(flag);
	if (it != cmdDetail.end())
		return (it->second);
	return ("unknown");
}

bool Server::cmdModeCheckArgs(const std::vector<std::string>& args)
{
	if (args.size() < 2)
		return (false);

	if (!checkAlphanum(args[0], 1))
		return (false);

	for (size_t i = 1; i < args.size(); i++)
	{
		if (args[i] == "+k" || args[i] == "+o" || args[i] == "-o")
		{
			if (i + 1 < args.size() && checkAlphanum(args[i + 1], 0))
				i++;
		}

		else if (args[i] == "+l")
		{
			if (i + 1 < args.size() && checkNumeric(args[i + 1]))
				i++;
		}

		else if (args[i] != "-i" && args[i] != "+i" && args[i] != "-t"
			&& args[i] != "+t" && args[i] != "-k" && args[i] != "-l")
			return (false);
	}

	return (true);
}

bool Server::cmdModeCheck(const std::vector<std::string>& args, int clientFd)
{
	if (!cmdModeCheckArgs(args))
		return (notify(clientFd, WHI, 2, 1, 1, "error : check '/help mode'"));

	Channel *channel = getChannelByName(args[0]);
	if (!channel)
		return (notify(clientFd, WHI, 2, 1, 1, "can't set mode; channel doesn't exist"));

	Client* client = getClientByFd(clientFd);
	if (!channel->isClientOnChannel(client))
		return (notify(clientFd, WHI, 2, 1, 1, "can't set mode; you haven't joined this channel"));

	if (!channel->isOperatorOnChannel(client) && channel->getOneMode("o"))
		return (notify(clientFd, WHI, 2, 1, 1, "can't set mode; you're not channel's operator"));

	for (size_t i = 1; i < args.size(); i++)
	{
		if (args[i] == "+o" && i + 1 < args.size() && checkAlphanum(args[i + 1], 0))
		{
			if (!channel->isClientOnChannel(getClientByNick(args[i + 1])))
				return (notify(clientFd, WHI, 2, 1, 1, "can't add operator; this user didn't join to channel"));
		}
	}

	return (true);
}

void Server::cmdMode(const std::vector<std::string>& args, int clientFd)
{
	if (!cmdModeCheck(args, clientFd))
		return;

	Channel* channel = getChannelByName(args[0]);
	for (size_t i = 1; i < args.size(); i++)
	{
		if (args[i] == "+k")
		{
			channel->setOneMode(args[i]);
			notify(clientFd, WHI, 6, 1, 1, cmdFlagAction(args[i]), channel);
			if (i + 1 < args.size() && checkAlphanum(args[i + 1], 0))
			{
				channel->setPwd(args[i + 1]);
				i++;
			}
		}

		// else if (args[i] == "+o" || args[i] == "-o")
		// {
			// channel->setOneMode(args[i]);
			// notify(clientFd, WHI, 6, 1, 1, cmdFlagAction(args[i]), channel);
			// if (i + 1 < args.size() && checkAlphanum(args[i + 1], 0))
			// {
				// if (args[i] == "+o")
					// channel->addOperator(getClientByNick(args[i + 1]));
				// else if (args[i] == "-o")
					// channel->removeOperator(getClientByNick(args[i + 1]));
				// i++;
			// }
		// }


		else if (args[i] == "+o" || args[i] == "-o")
		{
			notify(clientFd, WHI, 6, 1, 1, cmdFlagAction(args[i]), channel);
			if (args[i] == "+o" && i + 1 < args.size() && checkAlphanum(args[i + 1], 0))
			{
				channel->setOneMode(args[i]);
				channel->addOperator(getClientByNick(args[i + 1]));
				i++;
			}
			else if (args[i] == "-o" && i + 1 < args.size() && checkAlphanum(args[i + 1], 0))
			{
				channel->removeOperator(getClientByNick(args[i + 1]));
				i++;
			}
			else
				channel->setOneMode(args[i]);
		}


		else if (args[i] == "+l")
		{
			channel->setOneMode(args[i]);
			notify(clientFd, WHI, 6, 1, 1, cmdFlagAction(args[i]), channel);
			if (i + 1 < args.size() && checkNumeric(args[i + 1]))
			{
				channel->setLimit(atoi(args[i + 1].c_str()));
				i++;
			}
		}

		else
		{
			channel->setOneMode(args[i]);
			notify(clientFd, WHI, 6, 1, 1, cmdFlagAction(args[i]), channel);
		}
	}

}

