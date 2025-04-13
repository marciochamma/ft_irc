/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchamma <mchamma@student.42sp.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 16:37:02 by mchamma           #+#    #+#             */
/*   Updated: 2025/02/11 16:34:21 by mchamma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Headers.hpp"
#include "Channel.hpp"

typedef enum {
	UNAUTHENTICATED,
	AUTHENTICATED
} status;

class	Client
{
private:
	int			_fd;
	std::string	_ipAdd;
	std::string	_user;
	std::string	_nick;
	std::string	_pwd;
	Channel* 	_currentChannel;
	status		_status;
	std::string _buffer;

	bool _capNegotiationComplete;


public:
	Client(const std::string& nick);
	~Client(void);

	int&			getFd(void);
	std::string&	getIpAdd(void);
	std::string&	getUser(void);
	std::string&	getNick(void);
	std::string&	getPwd(void);
	Channel*		getCurrentChannel(void) const;
	status& 		getStatus(void);
	std::string		getBuffer(void);
	bool 			getCapNegotiationComplete(void) const;

	void	setFd(const int& fd);
	void	setIpAdd(const std::string& ipAdd);
	void	setUser(const std::string& usrname);
	void	setNick(const std::string& nick);
	void	setPwd(const std::string& pwd);
	void	setCurrentChannel(Channel* channel);
	void	setStatus(const status& status);
	void 	setBuffer(std::string recived);
	void 	setCapNegotiationComplete(bool value);

	void 	clearBuffer(void);
};

#endif