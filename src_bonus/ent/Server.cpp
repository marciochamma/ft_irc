/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fde-alen <fde-alen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/12 16:38:26 by mchamma           #+#    #+#             */
/*   Updated: 2025/04/13 00:03:09 by fde-alen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Bot.hpp"

Server::Server(void) { }

Server::~Server(void)
{
	closeAllFds();
	for (size_t i = 0; i < this->_channels.size(); i++)
		delete this->_channels[i];
	if (this->_bot)
        delete this->_bot;
	return ;
}

bool	Server::_signal = false;

void	Server::signalHandler(int signal)
{
	(void) signal;
	std::cout <<YEL <<" >> Interrupt Signal" <<WHI <<std::endl;
	Server::_signal = true;
}

void	Server::initServer(int port, const std::string& pwd)
{
	this->_port = port;
	this->_pwd = pwd;

	struct pollfd stdinPoll;
	stdinPoll.fd = fileno(stdin);
	stdinPoll.events = POLLIN;
	stdinPoll.revents = 0;
	this->_fds.push_back(stdinPoll);

	std::cout <<TIME <<"Starting the Server.. " <<std::endl;
	initServerSocket();
}

void	Server::initServerSocket(void)
{
	sockaddr_in serverAdd;
	serverAdd.sin_family = AF_INET;
	serverAdd.sin_addr.s_addr = INADDR_ANY;
	serverAdd.sin_port = htons(_port);

	size_t aux = 1;
	this->_servFd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_servFd < 0)
		throw(std::runtime_error(TIME +"failed to create the serv socket"));
	if(setsockopt(this->_servFd, SOL_SOCKET, SO_REUSEADDR, &aux, sizeof(aux)) < 0)
		throw(std::runtime_error(TIME +"failed to set SO_REUSEADDR on serv socket"));
	if (fcntl(this->_servFd, F_SETFL, O_NONBLOCK) < 0)
		throw(std::runtime_error(TIME +"failed to set O_NONBLOCK on serv socket"));
	if (bind(this->_servFd, (struct sockaddr *)&serverAdd, sizeof(serverAdd)) < 0)
		throw(std::runtime_error(TIME +"failed to bind serv socket"));
	if (listen(this->_servFd, SOMAXCONN) < 0)
		throw(std::runtime_error(TIME +"failed to set serv socket to listen"));

	struct pollfd newPoll;
	newPoll.fd = this->_servFd;
	newPoll.events = POLLIN;
	newPoll.revents = 0;
	this->_fds.push_back(newPoll);
}

void Server::runServer(void)
{
	std::cout <<TIME <<GRE <<"<Serv" <<this->_servFd <<"> connected" <<WHI <<std::endl;

	while (!Server::_signal)
	{
		if ((poll(&this->_fds[0], this->_fds.size(), -1) < 0) && !Server::_signal)
			throw std::runtime_error(TIME +"failed on monitoring the file descriptors");

		for (size_t i = 0; i < this->_fds.size() && !Server::_signal; ++i)
			if (this->_fds[i].revents & POLLIN)
				monitorPollEvent(i);
	}

	closeAllFds();
}

void Server::monitorPollEvent(int fd)
{
	if (this->_fds[fd].fd == this->_servFd)
		connectClient();
	else if (this->_fds[fd].fd == fileno(stdin))
		serverInput();
	else
		handleClient(this->_fds[fd].fd);
}

void Server::connectClient(void)
{
	struct sockaddr_in clientAdd;
	socklen_t len = sizeof(clientAdd);

	int clientFd = accept(this->_servFd, (sockaddr *)&(clientAdd), &len);
	if (clientFd < 0)
		{std::cout <<TIME <<"<User" <<clientFd <<"> fail acceptance" <<std::endl; return;}
	if (fcntl(clientFd, F_SETFL, O_NONBLOCK) < 0)
		{std::cout <<TIME <<"<User" <<clientFd <<"> fail non-blocking mode" <<std::endl; return;}

	struct pollfd newPoll;
	newPoll.fd = clientFd;
	newPoll.events = POLLIN;
	newPoll.revents = 0;
	this->_fds.push_back(newPoll);

	Client* client = new Client("User" + intToStr(clientFd));
	client->setFd(clientFd);
	client->setIpAdd(inet_ntoa((clientAdd.sin_addr)));
	this->_clients.push_back(client);

	std::cout <<TIME <<"<User" <<clientFd <<"> requested connection.." <<std::endl;
	send (clientFd, "Insert password: \n", strlen("Insert password: \n"), 0);
}

void	Server::serverInput(void)
{
	std::string input;
	std::getline(std::cin, input);
	if (input == "/quit")
		this->_signal = true;
	else if (input == "/status")
		status();
	else
		std::cout <<TIME <<"<Serv" <<this->_servFd <<"> " <<input <<": command not found\n";
}

ssize_t Server::initBuffer(int clientFd, char* buff, size_t buff_size)
{
	memset(buff, 0, buff_size);
	ssize_t bytesReceived = recv(clientFd, buff, buff_size - 1, 0);
	if (bytesReceived > 0)
		buff[bytesReceived] = '\0';
	return (bytesReceived);
}

void Server::handleClient(int clientFd)
{
	char buff[1024];
	Client* client = getClientByFd(clientFd);
	ssize_t bytesReceived = initBuffer(clientFd, buff, sizeof(buff));

	if (bytesReceived <= 0)
	{
		std::cout <<TIME <<"<User" <<clientFd <<"> fail receive data" <<std::endl;
		disconnectClient(clientFd);
		return;
	}

	client->setBuffer(buff);
	if (client->getBuffer().find_first_of("\r\n") == std::string::npos)
		return;

	driveActions(client->getBuffer(), clientFd);

	if (getClientByFd(clientFd))
		client->clearBuffer();
}

void Server::driveActions(const std::string& input, int clientFd)
{
    std::vector<std::string> lines = parseText(input);
    for (std::vector<std::string>::iterator itLine = lines.begin(); itLine != lines.end(); ++itLine)
    {
        Client* client = getClientByFd(clientFd);
        if (!client)
            break;

        std::vector<std::string> tokens = parseLine(*itLine);
        if (tokens.empty())
            continue;

        tokens[0] = transfCmd(tokens[0]);

        if (tokens[0] == "/cap")
            execCmd(tokens, clientFd);
        else if (client->getStatus() == UNAUTHENTICATED)
            authenticate(tokens, clientFd);
        else if (tokens[0][0] == '/')
            execCmd(tokens, clientFd);
        else
        {
            Channel* currentChannel = client->getCurrentChannel();
            if (currentChannel && _bot)
            {
                if (currentChannel->isClientOnChannel(_bot))
                {
                    if (_bot->isCommandForMe(*itLine))
                    {
                        std::vector<std::string> botTokens = parseLine(*itLine);
                        _bot->handleMessage(this, botTokens, clientFd, currentChannel->getName());
                        continue;
                    }
                }
            }
            directMsg(tokens, clientFd);
        }
    }
}

void Server::authenticate(const std::vector<std::string>& tokens, int clientFd)
{
	std::string password = "";
	std::string note;

	if (tokens.size() == 2 && tokens[0] == "PASS")
		password = tokens[1];
	else if (tokens.size() == 1)
		password = tokens[0];

	if (password != this->_pwd)
	{
		note = TIME + "<User" + intToStr(clientFd) + "> set wrong password\n";
		std::cout << note;
		send(clientFd, note.c_str(), strlen(note.c_str()), 0);
		disconnectClient(clientFd);
		return;
	}

	note = GRE + TIME + "<User" + intToStr(clientFd) + "> connected" + WHI + "\n";
	std::cout << note;
	send(clientFd, note.c_str(), strlen(note.c_str()), 0);
	getClientByFd(clientFd)->setStatus(AUTHENTICATED);
}

std::map<std::string, Server::CommandFunc> Server::initCommandMap(void)
{
	typedef void (Server::*CommandFunc)(const std::vector<std::string>&, int clientFd);
	std::map<std::string, CommandFunc> commandMap;
	commandMap["/cap"] = &Server::cmdCap;
	commandMap["/help"] = &Server::cmdHelp;
	commandMap["/invite"] = &Server::cmdInvite;
	commandMap["/join"] = &Server::cmdJoin;
	commandMap["/kick"] = &Server::cmdKick;
	commandMap["/list"] = &Server::cmdList;
	commandMap["/mode"] = &Server::cmdMode;
	commandMap["/msg"] = &Server::cmdMsg;
	commandMap["/privmsg"] = &Server::cmdMsg;
	commandMap["/nick"] = &Server::cmdNick;
	commandMap["/part"] = &Server::cmdPart;
	commandMap["/quit"] = &Server::cmdQuit;
	commandMap["/topic"] = &Server::cmdTopic;
	commandMap["/user"] = &Server::cmdUser;
	commandMap["/who"] = &Server::cmdWho;
	commandMap["/addbot"] = &Server::cmdAddBot;

	return (commandMap);
}

void Server::execCmd(const std::vector<std::string>& tokens, int clientFd)
{
	typedef void (Server::*CommandFunc)(const std::vector<std::string>&, int clientFd);
	std::map<std::string, CommandFunc> commandMap = initCommandMap();

	std::string command = tokens[0];
	std::vector<std::string> args(tokens.begin() + 1, tokens.end());
	std::map<std::string, CommandFunc>::iterator it = commandMap.find(command);

	if (it != commandMap.end())
	{
		CommandFunc cmdFunc = it->second;
		(this->*cmdFunc)(args, clientFd);
	}
	else
		unknownCommand(command, clientFd);
}

void Server::unknownCommand(std::string command, int clientFd)
{
	std::string note;
	Client *client = getClientByFd(clientFd);

	note = TIME +"<" +client->getNick() +"> " +command +": command not found\n";
	send(clientFd, note.c_str(), strlen(note.c_str()), 0);
}

void Server::directMsg(const std::vector<std::string>& tokens, int clientFd)
{
	Client* client = getClientByFd(clientFd);

	if (!client->getCurrentChannel() || client->getCurrentChannel()->getName().empty())
	{
		notify(clientFd, WHI, 2, 1, 1, "can't send msg; you're not active in any channel ");
		return ;
	}

	Channel* channel = client->getCurrentChannel();

	std::vector<std::string> modifiedTokens = tokens;
	modifiedTokens.insert(modifiedTokens.begin(), channel->getName());
	modifiedTokens.insert(modifiedTokens.begin(), "/msg");

	execCmd(modifiedTokens, clientFd);
}

void Server::disconnectClient(int clientFd)
{
	if (clientFd == -1) return;

	std::string note;
	note = RED + TIME + "<User" + intToStr(clientFd) + "> disconnected" + WHI + "\n";
	std::cout << note;
	send(clientFd, note.c_str(), strlen(note.c_str()), 0);

	clearClientFromVector(clientFd);
	close(clientFd);

	Client* client = getClientByFd(clientFd);
	if (client != NULL)
	{
		for (size_t i = 0; i < this->_channels.size(); i++)
			this->_channels[i]->removeClient(client);

		for (size_t i = 0; i < this->_clients.size(); i++)
		{
			if (this->_clients[i]->getFd() == clientFd)
			{
				delete this->_clients[i];
				this->_clients.erase(this->_clients.begin() + i);
				break;
			}
		}
	}
}

void Server::clearClientFromVector(int clientFd)
{
	for (size_t i = 0; i < this->_fds.size(); i++)
	{
		if (this->_fds[i].fd == clientFd)
		{
			this->_fds.erase(this->_fds.begin() + i);
			break;
		}
	}
}

void Server::closeAllFds(void)
{
	std::string note;

	for (size_t i = 0; i < this->_clients.size(); i++)
	{
		if (this->_clients[i] == NULL)
			continue ;
		note = RED +TIME +"<User" +intToStr(_clients[i]->getFd()) +"> disconnected" +WHI +"\n";
		std::cout <<note; send(_clients[i]->getFd(), note.c_str(), strlen(note.c_str()), 0);
		close(this->_clients[i]->getFd());
		delete this->_clients[i];
	}
	this->_clients.clear();

	if (this->_servFd != -1)
	{
		std::cout <<RED <<TIME <<"<Serv" <<this->_servFd <<"> disconnected" <<WHI <<std::endl;
		close(this->_servFd);
		this->_servFd = -1;
	}
}

void Server::initBot()
{
    this->_bot = new Bot("*42*");
    this->_bot->setFd(-1);
    this->_clients.push_back(this->_bot);
}

Bot* Server::getBot() const
{
    return this->_bot;
}
