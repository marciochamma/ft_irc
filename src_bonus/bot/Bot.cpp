/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fde-alen <fde-alen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 20:41:11 by fde-alen          #+#    #+#             */
/*   Updated: 2025/04/12 23:52:58 by fde-alen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"
#include "Server.hpp"
#include "Channel.hpp"
#include <sstream>
#include <algorithm>

Bot::Bot(const std::string& name) : Client(std::string("Bot") + name), _botName(std::string("Bot") + name)
{
    this->_welcomeMessage = "Hello! I'm " + _botName + ", your friendly IRC bot. Use !help for commands.";
    this->setStatus(AUTHENTICATED);

    const char* quotes[] = {
        "Space is big. You just won't believe how vastly, hugely, mind-bogglingly big it is. I mean, you may think it's a long way down the road to the chemist's, but that's just peanuts to space.",
        "The Guide says there is an art to flying*, or rather a knack. The knack lies in learning how to throw yourself at the ground and miss.",
        "In the beginning the Universe was created. This has made a lot of people very angry and has been widely regarded as a bad move.",
        "For a moment, nothing happened. Then, after a second or so, nothing continued to happen.",
        "Time is an illusion. Lunchtime doubly so.",
        "The ships hung in the sky in much the same way that bricks don't.",
        "It is a curious fact, and one widely acknowledged, that no matter what the circumstances, one can always find a public building which sells very bad tea.",
        "Don't Panic.",
        "The Hitchhiker's Guide to the Galaxy is a wholly remarkable book. Perhaps the most remarkable book ever to come out of the great publishing corporations of Ursa Minor.",
        "The chances of finding a smallish, off-blue, flower-like life form that's totally fed up with being trodden on were approximately what you might expect.",
        "Ford Prefect was a reporter for The Hitchhiker's Guide to the Galaxy.",
        "He was a lot like Douglas Adams himself, except six feet tall and more handsome.",
        "Arthur Dent was a rather worried man.",
        "His house had been knocked down that morning.",
        "This made him rather cross.",
        "The Earth had been demolished to make way for a hyperspace bypass.",
        "This also made him rather cross.",
        "Many worlds are born and die in the space of a moment.",
        "The Restaurant at the End of the Universe is one of the most extraordinary ventures in the entire history of gastronomy.",
        "It is a restaurant which exists precisely at the moment of the End of the Universe.",
        "The Total Perspective Vortex derives its power from the whole of infinity.",
        "It is a curious sensation, this feeling of being drunk on nothing but air.",
        "The best drink in existence is the Pan Galactic Gargle Blaster.",
        "Its effects are rather like having your brain smashed out by a slice of lemon wrapped round a large gold brick.",
        "And all of it perfectly legal.",
        "The Guide says that the history of every major Galactic Civilization tends to pass through three distinct and recognizable phases, those of Survival, Inquiry and Sophistication, otherwise ,known as the How, Why, and Where phases.",
        "For instance, the first phase is characterized by the question 'How can we eat?' the second by the question 'Why do we eat?' and the third by the question 'Where shall we have lunch?'",
        "The chances of finding out what's really going on are infinitesimally small.",
        "We are hurtling through space at a considerable number of miles per second.",
        "The chances of anything man wants to do or build succeeding are minute.",
        "The most improbable things constantly happen.",
        "The ships hung in the sky in much the same way that bricks don't.",
        "It seemed an unlikely coincidence that anything so mind-bogglingly useful could have evolved purely by chance.",
        "The Babel fish is small, yellow and leech-like, and probably the oddest thing in the Universe.",
        "It feeds on brainwave energy not from its carrier but from those around it.",
        "It absorbs all unconscious mental frequencies from this brainwave energy to nourish itself.",
        "It then excretes into the mind of its carrier a telepathic matrix of which are taken from the brainwave patterns it has absorbed.",
        "The practical upshot of all this is that if you stick a Babel fish in your ear you can instantly understand anything said to you in any form of language.",
        "The Hitchhiker's Guide to the Galaxy also mentions alcohol.",
        "It says that alcohol is a colourless volatile liquid.",
        "It also says that the best way to see a really good colour change is to drink a lot of it.",
        "A common mistake that people make when trying to design something completely foolproof is to underestimate the ingenuity of complete fools.",
        "The universe is a joke.",
        "Everything in it is a joke.",
        "Sometimes there just aren't enough appropriate swear words to properly express one's feelings.",
        "It is known that there are an infinite number of worlds, simply because there is an infinite amount of space for them to be in. However, not every one of them is inhabited. Therefore, there ,must be a finite number of inhabited worlds. Any finite number divided by infinity is as near to zero as makes no odds, so the average population of all the planets in the Universe can be said to be zero. From this it follows that the population of the whole Universe is also zero, and that any people you may meet from time to time are merely the products of a deranged imagination.",
        "The argument runs something like this: 'I refuse to prove that I exist,' says God, 'for proof denies faith, and without faith I am nothing.' 'But,' says Man, 'The Babel fish is a dead ,giveaway, isn't it? It could not have evolved by chance. It proves you exist, and so therefore, by your own arguments, you don't. Q.E.D.' 'Oh dear,' says God, 'I hadn't thought of that,' and promptly vanishes in a puff of logic.",
        "The Guide is definitive. Reality is frequently inaccurate.",
        "He hoped he hadn't given himself away. He hoped he hadn't said anything. He hoped he hadn't moved.",
        "So long, and thanks for all the fish."
    };
    const int numQuotes = sizeof(quotes)/sizeof(quotes[0]);
    for (int i = 0; i < numQuotes; ++i) {
        _quotes.push_back(quotes[i]);
    }

    std::srand(static_cast<unsigned int>(std::time(0)));
}

Bot::~Bot() { }

bool Bot::isCommandForMe(const std::string& message) const
{
    return message.size() > 0 && message[0] == '!';
}

void Bot::handleMessage(Server* server, const std::vector<std::string>& args, int clientFd, const std::string& channelName)
{
    if (args.empty())
        return;

    std::string command = args[0];
    std::string response;

    if (command == "!help")
    {
        response = "Available commands: !hello, !time, !quote, !roll [XdY], !help";
    }
    else if (command == "!hello" || command == "!hi")
    {
        Client* sender = server->getClientByFd(clientFd);
        if (sender)
            response = "Hello there, " + sender->getNick() + "! Hope you are having a nice day!";
        else
            response = "Hello there!";
    }
    else if (command == "!time")
    {
        response = "Current time is: " + server->getCurrentLocalTime() + ". It 's getting late!";
    }
    else if (command == "!quote")
    {
        int quoteIndex = std::rand() % _quotes.size();
        response = _quotes[quoteIndex];
    }
    else if (command == "!roll")
    {
        response = handleRollCommand(args);
    }
    else
    {
        return;
    }

    if (!channelName.empty())
        sendResponse(server, channelName, response);
    else
        sendResponse(server, clientFd, response);
}

std::string Bot::handleRollCommand(const std::vector<std::string>& args)
{
    if (args.size() == 1)
    {
        int roll = (std::rand() % 6) + 1;

        char buffer[50];
        std::sprintf(buffer, "You rolled: %d", roll);
        return std::string(buffer);
    }

    std::string arg = args[1];

    size_t d_pos = arg.find('d');
    if (d_pos != std::string::npos)
    {
        std::string num_dice_str = arg.substr(0, d_pos);
        std::string num_sides_str = arg.substr(d_pos + 1);

        int num_dice = atoi(num_dice_str.c_str());
        int num_sides = atoi(num_sides_str.c_str());

        if (num_dice < 1 || num_dice > 10) return "Number of dice must be between 1 and 10";
        if (num_sides < 2 || num_sides > 100) return "Number of sides must be between 2 and 100";

        std::string result = "You rolled: ";
        int total = 0;
        for (int i = 0; i < num_dice; ++i)
        {
            int roll = (std::rand() % num_sides) + 1;
            total += roll;

            char buffer[20];
            std::sprintf(buffer, "%d", roll);
            result += buffer;
            if (i < num_dice - 1) result += ", ";
        }

        if (num_dice > 1)
        {
            char total_buffer[50];
            std::sprintf(total_buffer, " (Total: %d)", total);
            result += total_buffer;
        }

        return result;
    }

    int num_sides = atoi(arg.c_str());
    if (num_sides < 2 || num_sides > 100)
        return "Number of sides must be between 2 and 100";

    int roll = (std::rand() % num_sides) + 1;

    char buffer[50];
    std::sprintf(buffer, "You rolled: %d", roll);
    return std::string(buffer);
}

void Bot::sendResponse(Server* server, const std::string& channelName, const std::string& message)
{
    std::vector<std::string> msgArgs;
    msgArgs.push_back(channelName);
    msgArgs.push_back(message);
    server->cmdMsg(msgArgs, this->getFd());
}

void Bot::sendResponse(Server* server, int clientFd, const std::string& message)
{
    Client* recipient = server->getClientByFd(clientFd);
    if (!recipient)
        return;

    std::vector<std::string> msgArgs;
    msgArgs.push_back(recipient->getNick());
    msgArgs.push_back(message);
    server->cmdMsg(msgArgs, this->getFd());
}

void Bot::joinChannel(Server* server, const std::string& channelName)
{
    std::vector<std::string> joinArgs;
    joinArgs.push_back(channelName);
    server->cmdJoin(joinArgs, this->getFd());

    sendResponse(server, channelName, this->_welcomeMessage);
}
