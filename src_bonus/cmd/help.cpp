/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   help.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fde-alen <fde-alen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 16:00:17 by mchamma           #+#    #+#             */
/*   Updated: 2025/04/12 23:31:09 by fde-alen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

std::string Server::cmdDescription(const std::string& command)
{
	std::map<std::string, std::string> descriptionMap;
	descriptionMap["addbot"] = "adds a bot to the specified channel";
	descriptionMap["help"] = "helps understanding the commands";
	descriptionMap["invite"] = "invites a user to a channel";
	descriptionMap["join"] = "joins the channel";
	descriptionMap["kick"] = "removes a user from a channel";
	descriptionMap["list"] = "lists all available channel";
	descriptionMap["mode"] = "sets the modes for a channel";
	descriptionMap["msg"] = "send a msg to channel or to user";
	descriptionMap["nick"] = "sets the nickname";
	descriptionMap["part"] = "leaves a channel";
	descriptionMap["quit"] = "disconnects from the server";
	descriptionMap["topic"] = "sets a channel's topic";
	descriptionMap["user"] = "sets the username";
	descriptionMap["who"] = "lists all available users";

	std::map<std::string, std::string>::const_iterator it;
	it = descriptionMap.find(command);
	if (it != descriptionMap.end())
		return (it->second);
	return ("unknown");
}

std::string Server::cmdUsage(const std::string& command)
{
	std::map<std::string, std::string> usageMap;
	usageMap["addbot"] = "/addbot <#channel>";
	usageMap["help"] = "/help [<command>]";
	usageMap["invite"] = "/invite <nickname> <channel>";
	usageMap["join"] = "/join <channel> [<password>]";
	usageMap["kick"] = "/kick <nickname> <channel>";
	usageMap["list"] = "/list";
	usageMap["mode"] = "/mode <channel> <flag> [<complement>]";
	usageMap["msg"] = "/msg <channel or nickname> <message>";
	usageMap["nick"] = "/nick <new nickname>";
	usageMap["part"] = "/part <channel>";
	usageMap["quit"] = "/quit [<quit message>]";
	usageMap["topic"] = "/topic <channel> [<description or message>]";
	usageMap["user"] = "/user <new username>";
	usageMap["who"] = "/who";

	std::map<std::string, std::string>::const_iterator it;
	it = usageMap.find(command);
	if (it != usageMap.end())
		return (it->second);
	return ("unknown");
}

std::string Server::cmdDetail(const std::string& command)
{
	std::map<std::string, std::string> cmdDetail;
	cmdDetail["addbot"] = "channel #alphanum; adds a bot to the channel";
	cmdDetail["help"] ="command alphanum";
	cmdDetail["invite"] ="nick alphanum; channel #alphanum";
	cmdDetail["join"] ="channel #alphanum; password alphanum; max 9 dig";
	cmdDetail["kick"] ="nick alphanum; channel #alphanum";
	cmdDetail["list"] ="no args";
	cmdDetail["mode"] ="channel #alphanum; flags: +/- i, t, k, o, l";
	cmdDetail["msg"] ="channel #alphanum or nick alphanum; message printable";
	cmdDetail["nick"] ="nickname alphanum; max 9 dig";
	cmdDetail["part"] ="channel #alphanum";
	cmdDetail["quit"] ="message printable";
	cmdDetail["topic"] ="channel #alphanum; message printable chars; max 15 dig";
	cmdDetail["user"] ="username alphanum; max 9 dig";
	cmdDetail["who"] ="no args";

	std::map<std::string, std::string>::const_iterator it;
	it = cmdDetail.find(command);
	if (it != cmdDetail.end())
		return (it->second);
	return ("unknown");
}

bool Server::cmdHelpCheck(const std::vector<std::string>& args, int clientFd)
{
	if (args.empty())
		return (notify(clientFd, WHI, 2, 1, 1, "use : '/help <command>'\n"
			" available commands: join, part, msg, invite, kick, \n"
			"                     list, mode, , who"));

	else if (args.size() == 1 && cmdUsage(args[0]) == "unknown")
		return (notify(clientFd, WHI, 2, 1, 1, "error : check '/help help'"));

	else if (args.size() > 1)
		return (notify(clientFd, WHI, 2, 1, 1, "error : check '/help help'"));

	return (true);
}

void Server::cmdHelp(const std::vector<std::string>& args, int clientFd)
{
	if (!cmdHelpCheck(args, clientFd))
		return ;

	notify(clientFd, WHI, 2, 1, 1, "\n"
		" Usage: " +cmdUsage(args[0]) +"\n"
		" Description: " + cmdDescription(args[0]) +"\n"
		" Detail: " +cmdDetail(args[0]));
}
