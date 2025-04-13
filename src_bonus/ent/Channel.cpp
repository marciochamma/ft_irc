/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchamma <mchamma@student.42sp.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 18:14:00 by mchamma           #+#    #+#             */
/*   Updated: 2025/02/11 23:18:39 by mchamma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "Client.hpp"

Channel::Channel() : _name(""), _topic(""), _pwd(""), _limit(0) { }

Channel::Channel(const std::string& name) : _name(name)
{
	this->_topic = "Sono Pazzi Questi Romani!";
	this->_pwd = "";
	this->_limit = 2;
	this->_modes["i"] = false;
	this->_modes["t"] = false;
	this->_modes["k"] = false;
	this->_modes["o"] = false;
	this->_modes["l"] = false;
}

Channel::~Channel(void) { }

Channel::Channel(const Channel& copy)
{
	*this = copy;
	return ;
}

Channel& Channel::operator=(const Channel& other)
{
	if (this != &other)
	{
		this->_name = other._name;
		this->_topic = other._topic;;
		this->_pwd = other._pwd;
		this->_limit = other._limit;
		this->_clients = other._clients;
		this->_operators = other._operators;
		this->_modes = other._modes;
		this->_invitees = other._invitees;
	}
	return (*this);
}

std::string& Channel::getName(void)
{
	return _name;
}

std::string& Channel::getTopic(void)
{
	return _topic;
}

std::string& Channel::getPwd(void)
{
	return _pwd;
}

size_t&	Channel::getLimit(void)
{
	return _limit;
}

std::vector<Client*> Channel::getClients(void)
{
	return this->_clients;
}

std::vector<Client*> Channel::getOperators(void)
{
	return this->_operators;
}

bool Channel::getOneMode(const std::string& flag) const
{
	if (this->_modes.find(flag) != this->_modes.end())
		return (this->_modes.at(flag));
	return (false);
}

std::map<std::string, bool> Channel::getModes(void) const
{
	return (this->_modes);
}


void Channel::setName(std::string& name)
{
	this->_name = name;
}

void Channel::setTopic(const std::string& topic)
{
	this->_topic = topic;
}

void Channel::setPwd(const std::string& pwd)
{
	this->_pwd = pwd;
}

void Channel::setLimit(size_t limit)
{
	this->_limit = limit;
}

void Channel::setOneMode(const std::string& flag)
{
	char firstChar = flag[0];
	std::string mode = flag.substr(1);

	if (firstChar == '-')
		this->_modes[mode] = false;
	else if (firstChar == '+')
		this->_modes[mode] = true;
}

void Channel::addClient(Client* client)
{
	if (std::find(_clients.begin(), _clients.end(), client) == _clients.end())
		_clients.push_back(client);
}

void Channel::removeClient(Client* client)
{
	_clients.erase(std::remove(_clients.begin(), _clients.end(), client), _clients.end());
	removeOperator(client);
}

bool Channel::isClientOnChannel(Client* client)
{
	return (std::find(_clients.begin(), _clients.end(), client) != _clients.end());
}

void Channel::printClients(void)
{
	std::cout <<"Clients in [" <<this->getName() <<"]: ";
	for (size_t i = 0; i < _clients.size(); ++i)
		std::cout <<this->_clients[i]->getFd() <<" ";
	std::cout <<std::endl;
}

void Channel::addOperator(Client* operat)
{
	if (std::find(_operators.begin(), _operators.end(), operat) == _operators.end())
		_operators.push_back(operat);
}

void Channel::removeOperator(Client* operat)
{
	_operators.erase(std::remove(_operators.begin(), _operators.end(), operat), _operators.end());
}

bool Channel::isOperatorOnChannel(Client* operat)
{
	return (std::find(_operators.begin(), _operators.end(), operat) != _operators.end());
}

void Channel::printOperators(void)
{
	std::cout <<"Operators in [" <<this->getName() <<"]: ";
	for (size_t i = 0; i < _operators.size(); ++i)
		std::cout <<this->_operators[i]->getFd() <<" ";
	std::cout <<std::endl;
}


void Channel::addInvitee(std::string invitee)
{
	if (std::find(_invitees.begin(), _invitees.end(), invitee) == _invitees.end())
		this->_invitees.push_back(invitee);
}

void Channel::removeInvitee(std::string invitee)
{
	_invitees.erase(std::remove(_invitees.begin(), _invitees.end(), invitee), _invitees.end());
}

void Channel::printInvitees(void)
{
	std::cout <<"Invitees in [" <<this->getName() <<"]: ";
	std::cout <<"size: " <<this->_invitees.size() <<std::endl;

	for (size_t i = 0; i < this->_invitees.size(); i++)
		std::cout <<this->_invitees[i] <<" ";
	std::cout <<std::endl;
}

bool Channel::isInvitee(std::string invitee)
{
	return (std::find(_invitees.begin(), _invitees.end(), invitee) != _invitees.end());
}
