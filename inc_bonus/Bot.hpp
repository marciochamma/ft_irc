/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fde-alen <fde-alen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 20:41:11 by fde-alen          #+#    #+#             */
/*   Updated: 2025/04/12 22:21:53 by fde-alen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOT_HPP
#define BOT_HPP

#include "Client.hpp"
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>

// Forward declaration
class Server;

class Bot : public Client
{
private:
    std::string _botName;
    std::string _welcomeMessage;
    std::vector<std::string> _quotes;

    std::string handleRollCommand(const std::vector<std::string>& args);

public:
    Bot(const std::string& name);
    ~Bot();

    void handleMessage(Server* server, const std::vector<std::string>& args, int clientFd, const std::string& channelName = "");
    void sendResponse(Server* server, const std::string& channelName, const std::string& message);
    void sendResponse(Server* server, int clientFd, const std::string& message);
    void joinChannel(Server* server, const std::string& channelName);
    bool isCommandForMe(const std::string& message) const;
};

#endif
