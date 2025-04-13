/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchamma <mchamma@student.42sp.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 17:52:06 by mchamma           #+#    #+#             */
/*   Updated: 2025/02/08 20:32:24 by mchamma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Headers.hpp"

class Client;

class Channel
{
private:
	std::string _name;
	std::string _topic;
	std::string _pwd;
	size_t _limit;
	std::vector<Client*> _clients;
	std::vector<Client*> _operators;
	std::map<std::string, bool> _modes;
	std::vector<std::string> _invitees;

public:
	Channel(void);
	Channel(const std::string& name);
	Channel(const Channel& copy);
	~Channel(void);
	Channel& operator=(const Channel& other);

	std::string& getName(void);
	std::string& getTopic(void);
	std::string& getPwd(void);
	size_t& getLimit(void);
	std::vector<Client*> getClients(void);
	std::vector<Client*> getOperators(void);
	bool getOneMode(const std::string& flag) const;
	std::map<std::string, bool> getModes(void) const;

	void setName(std::string& name);
	void setTopic(const std::string& topic);
	void setPwd(const std::string& pwd);
	void setLimit(size_t limit);
	void setOneMode(const std::string& flag);

	void addClient(Client* client);
	void removeClient(Client* client);
	bool isClientOnChannel(Client* client);
	void printClients(void);

	void addOperator(Client* operat);
	void removeOperator(Client* operat);
	bool isOperatorOnChannel(Client* operat);
	void printOperators(void);

	void addInvitee(std::string invitee);
	void removeInvitee(std::string invitee);
	bool isInvitee(std::string invitee);
	void printInvitees(void);
};

#endif
