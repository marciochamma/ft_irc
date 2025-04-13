/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fde-alen <fde-alen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 18:22:56 by mchamma           #+#    #+#             */
/*   Updated: 2025/04/13 11:31:07 by fde-alen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

std::string Server::getCurrentLocalTime(void)
{
	time_t now = time(0);
	struct tm tstruct;
	char buf[80];
	tstruct = *localtime(&now);
	strftime(buf, sizeof(buf), "%H:%M", &tstruct);
	return buf;
}

std::string Server::intToStr(size_t num)
{
	std::stringstream ss;
	ss << num;
	return ss.str();
}

std::string Server::transfCmd(const std::string& arg)
{
	if (arg == "ADDBOT") return "/addbot";
	if (arg == "CAP") return "/cap";
	if (arg == "HELP") return "/help";
	if (arg == "INVITE") return "/invite";
	if (arg == "JOIN") return "/join";
	if (arg == "KICK") return "/kick";
	if (arg == "LIST") return "/list";
	if (arg == "MODE") return "/mode";
	if (arg == "MSG") return "/msg";
	if (arg == "PRIVMSG") return "/privmsg";
	if (arg == "NICK") return "/nick";
	if (arg == "PART") return "/part";
	if (arg == "QUIT") return "/quit";
	if (arg == "TOPIC") return "/topic";
	if (arg == "USER") return "/user";

	return (arg);
}


// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

Client*	Server::getClientByFd(int clientFd)
{
	for (size_t i = 0; i < this->_clients.size(); ++i)
		if (this->_clients[i]->getFd() == clientFd)
			return (this->_clients[i]);
	return (NULL);
}

Client*	Server::getClientByNick(std::string nick)
{
	for (size_t i = 0; i < this->_clients.size(); ++i)
		if (this->_clients[i]->getNick() == nick)
			return (this->_clients[i]);
	return (NULL);
}

Channel* Server::getChannelByName(const std::string& name)
{
	for (size_t i = 0; i < this->_channels.size(); ++i)
		if (this->_channels[i]->getName() == name)
			return this->_channels[i];
	return NULL;
}


// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

std::vector<std::string> Server::parseText(const std::string& str)
{
	std::vector<std::string> result;
	std::istringstream stream(str);
	std::string line;
	while (std::getline(stream, line))
		result.push_back(line);
	return (result);
}

std::string Server::parseQuotedStr(std::istringstream& stream, std::string& word)
{
	std::string temp = word.substr(1);
	std::string fragment;
	while (stream >> fragment)
	{
		temp += " " + fragment;
		if (fragment[fragment.size() - 1] == '"')
		{
			temp = temp.substr(0, temp.size() - 1);
			break;
		}
	}
	return temp;
}

std::vector<std::string> Server::parseLine(const std::string& str)
{
	std::istringstream stream(str);
	std::vector<std::string> result;
	std::string word;

	if (str.empty())
	{
		result.push_back("");
		return result;
	}

	while (stream >> word)
	{
		if (word[0] == '"')
			result.push_back(parseQuotedStr(stream, word));
		else
			result.push_back(word);
	}

	if (result.empty())
		result.push_back("");

	return (result);
}


// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

void Server::statusClient(Client* client)
{
	std::cout <<"FD = " <<client->getFd() <<"\n"
			  <<"IP ADD = " <<client->getIpAdd() <<"\n"
			  <<"USRNAME = " <<client->getUser() <<"\n"
			  <<"NICK = " <<client->getNick() <<"\n"
			  <<"PWD = " <<client->getPwd() <<"\n"
			  <<"CURRENT CHANNEL = " <<(client->getCurrentChannel() \
			  	? client->getCurrentChannel()->getName() : "N/A") <<"\n"
			  <<"STATUS = " << client->getStatus() <<"\n";
}

void Server::statusChannel(Channel* channel)
{
	std::cout <<"NAME = " <<channel->getName() <<"\n"
			  <<"TOPIC = " <<channel->getTopic() <<"\n"
			  <<"PWD = " <<channel->getPwd() <<"\n"
			  <<"LIMIT = " <<channel->getLimit() <<"\n"
			  <<"SIZE (#CLIENTS) = " <<channel->getClients().size() <<"\n";
	channel->printClients();
}

void Server::status()
{
	std::cout <<"\n";
	std::cout <<BLU <<"Qty clients = " <<this->_clients.size() <<WHI <<"\n";
	std::vector<Client*>::iterator itc;
	for (itc = this->_clients.begin(); itc != this->_clients.end(); ++itc)
	{
		statusClient(*itc);
		std::cout <<"\n";
	}

	std::cout <<BLU <<"Qty channels = " <<this->_channels.size() <<WHI <<"\n";
	std::vector<Channel*>::iterator ith;
	for (ith = this->_channels.begin(); ith != this->_channels.end(); ++ith)
	{
		statusChannel(*ith);
		std::cout <<"\n";
	}
}
