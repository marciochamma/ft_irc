/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 23:22:55 by mchamma           #+#    #+#             */
/*   Updated: 2025/04/07 20:58:30 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "Headers.hpp"
#include "Client.hpp"
#include "Channel.hpp"

class Server
{
private:
	int _port;
	std::string _pwd;
	static bool _signal;
	int	_servFd;
	std::vector<struct pollfd> _fds;
	std::vector<Client*> _clients;
	std::vector<Channel*> _channels;

public:
	Server(void);
	~Server(void);

	// Serv
	static void signalHandler(int signal);
	void initServer(int port, const std::string& pwd);
	void initServerSocket(void);
	void runServer(void);
	void monitorPollEvent(int clientFd);
	void connectClient(void);
	void serverInput(void);
	ssize_t initBuffer(int clientFd, char* buff, size_t buff_size);
	void handleClient(int clientFd);
	void driveActions(const std::string& input, int clientFd);
	void authenticate(const std::vector<std::string>& tokens, int clientFd);
	typedef void (Server::*CommandFunc)(const std::vector<std::string>&, int clientFd);
	std::map<std::string, CommandFunc> initCommandMap(void);
	void execCmd(const std::vector<std::string>& args, int clientFd);
	void unknownCommand(std::string command, int clientFd);
	void directMsg(const std::vector<std::string>& args, int clientFd);
	void disconnectClient(int clientFd);
	void clearClientFromVector(int clientFd);
	void closeAllFds(void);

	
	// Utils
	std::string getCurrentLocalTime(void);
	std::string intToStr(size_t num);
	std::string transfCmd(const std::string& arg);

	Client* getClientByFd(int clientFd);
	Client*	getClientByNick(std::string nick);
	Channel* getChannelByName(const std::string& name);

	std::vector<std::string> parseText(const std::string& str);
	std::string parseQuotedStr(std::istringstream& stream, std::string& word);
	std::vector<std::string> parseLine(const std::string& str);

	void status(void);
	void statusClient(Client* client);
	void statusChannel(Channel* channel);


	// Cmd Cap
	void cmdCap(const std::vector<std::string>& args, int clientFd);

	// Cmd Help
	std::string cmdDescription(const std::string& command);
	std::string cmdUsage(const std::string& command);
	std::string cmdDetail(const std::string& command);
	bool cmdHelpCheck(const std::vector<std::string>& args, int clientFd);
	void cmdHelp(const std::vector<std::string>& args, int clientFd);

	// Cmd Invite
	bool cmdInviteCheckArgs(const std::vector<std::string>& args);
	bool cmdInviteCheck(const std::vector<std::string>& args, int clientFd);
	void cmdInvite(const std::vector<std::string>& args, int clientFd);

	// Cmd Join
	void createChannel(const std::string& name);
	bool cmdJoinCheckArgs(const std::vector<std::string>& args);
	bool cmdJoinCheck(const std::vector<std::string>& args, int clientFd);
	void cmdJoin(const std::vector<std::string>& args, int clientFd);

	// Cmd Kick
	bool cmdKickCheckArgs(const std::vector<std::string>& args);
	bool cmdKickCheck(const std::vector<std::string>& args, int clientFd);
	void cmdKick(const std::vector<std::string>& args, int clientFd);

	// Cmd List
	std::string cmdListBody(Channel* channel, Client* client, \
		Channel* currentChannel, bool hasCurrentChannel);
	bool cmdListCheck(const std::vector<std::string>& args, int clientFd);
	void cmdList(const std::vector<std::string>& args, int clientFd);

	// Cmd Mode
	std::string cmdFlagAction(const std::string& flag);
	bool cmdModeCheckArgs(const std::vector<std::string>& args);
	bool cmdModeCheck(const std::vector<std::string>& args, int clientFd);
	void cmdMode(const std::vector<std::string>& args, int clientFd);

	// Cmd Msg
	void sendMsg(const std::vector<std::string>& args, int clientFd);
	void sendMsgPrivate(const std::vector<std::string>& args, int clientFd);
	void sendMsgChannel(const std::vector<std::string>& args, int clientFd);
	bool cmdMsgCheckArgs(const std::vector<std::string>& args);
	bool cmdMsgCheck(const std::vector<std::string>& args, int clientFd);
	void cmdMsg(const std::vector<std::string>& args, int clientFd);

	//Cmd Nick
	bool cmdNickCheckArgs(const std::vector<std::string>& args);
	bool cmdNickCheck(const std::vector<std::string>& args, int clientFd);
	void cmdNick(const std::vector<std::string>& args, int clientFd);

	// Cmd Part
	bool cmdPartCheckArgs(const std::vector<std::string>& args);
	bool cmdPartCheck(const std::vector<std::string>& args, int clientFd);
	void cmdPart(const std::vector<std::string>& args, int clientFd);

	// Cmd Quit
	bool cmdQuitCheckArgs(const std::vector<std::string>& args);
	bool cmdQuitCheck(const std::vector<std::string>& args, int clientFd);
	void cmdQuit(const std::vector<std::string>& args, int clientFd);

	// Cmd Topic
	bool cmdTopicCheckArgs(const std::vector<std::string>& args);
	bool cmdTopicCheck(const std::vector<std::string>& args,int clientFd);
	void cmdTopic(const std::vector<std::string>& args, int clientFd);

	//Cmd User
	bool cmdUserCheckArgs(const std::vector<std::string>& args);
	bool cmdUserCheck(const std::vector<std::string>& args, int clientFd);
	void cmdUser(const std::vector<std::string>& args, int clientFd);

	// Utils Cmd
	bool checkAlphanum(const std::string& str, int isChannel);
	bool checkNumeric(const std::string& str);
	bool checkPrintable(const std::string& str);

	std::string forgeNote(int fd, std::string color, int toWhom, int header, int footer, \
		const std::string& msg, Channel *channel = 0);
	bool notify(int fd, std::string color, int toWhom, int header, int footer, \
		const std::string& msg, Channel *channel = 0);

	bool cmdWhoCheckArgs(const std::vector<std::string>& args);
	bool cmdWhoCheck(const std::vector<std::string>& args, int clientFd);
	void cmdWho(const std::vector<std::string>& args, int clientFd);

};

#endif

