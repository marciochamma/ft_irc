/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajuliao- <ajuliao-@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 16:37:23 by mchamma           #+#    #+#             */
/*   Updated: 2025/04/09 20:25:17 by ajuliao-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(const std::string& user): _user(user), _nick(user),
	_pwd("1234"), _currentChannel(NULL), _status(UNAUTHENTICATED) { }

Client::~Client(void) { }

int&	Client::getFd(void)
{
	return (this->_fd);
}

std::string&	Client::getIpAdd(void)
{
	return (this->_ipAdd);
}

std::string& Client::getUser(void)
{
	return (this->_user);
}

std::string&	Client::getNick(void)
{
	return (this->_nick);
}

std::string&	Client::getPwd(void)
{
	return (this->_pwd);
}

Channel* Client::getCurrentChannel(void) const
{
	return (this->_currentChannel);
}

status& 	Client::getStatus(void)
{
	return (this->_status);
}

std::string Client::getBuffer(void)
{
	return (this->_buffer);
}

bool Client::getCapNegotiationComplete(void) const
{
	return (this->_capNegotiationComplete);
}


void Client::setFd(const int& fd)
{
	this->_fd = fd;
}

void Client::setIpAdd(const std::string& ipAdd)
{
	this->_ipAdd = ipAdd;
}

void Client::setUser(const std::string& user)
{
	this->_user = user;
}

void Client::setNick(const std::string& nick)
{
	this->_nick = nick;
}

void Client::setPwd(const std::string& pwd)
{
	this->_pwd = pwd;
}

void Client::setCurrentChannel(Channel* channel)
{
	this->_currentChannel = channel;
}

void Client::setStatus(const status& status)
{
	this->_status = status;
}

void Client::setBuffer(std::string add)
{
	this->_buffer += add;
}

void Client::setCapNegotiationComplete(bool value)
{
	this->_capNegotiationComplete = value;
}

void Client::clearBuffer(void)
{
	this->_buffer.clear();
}

bool Client::isIRCClient() const {
	return _pwd == "HEXCHAT";
}
