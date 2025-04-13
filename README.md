# IRC Server Project

## Table of Contents
1. [Introduction](#introduction)
2. [Features](#features)
3. [Installation](#installation)
4. [Usage](#usage)
5. [Commands](#commands)
6. [Technical Details](#technical-details)
7. [Project Structure](#project-structure)
8. [Contributing](#contributing)
9. [License](#license)
10. [System Architecture](#system-architecture)
11. [Core Components](#core-components)
12. [Command Protocol](#command-protocol)
13. [Channel Management](#channel-management)
14. [Client Management](#client-management)
15. [Error Handling](#error-handling)
16. [Performance Considerations](#performance-considerations)
17. [Limitations](#limitations)
18. [Possible Improvements](#future-improvements)
19. [Appendix: Class Diagrams](#appendix-class-diagrams)

---

## Introduction
This project is a custom IRC (Internet Relay Chat) server implementation written in C++. It allows multiple clients to connect, join channels, send messages, and interact with each other in real-time. The server supports standard IRC commands and includes additional features for channel management and user communication.

---

## Features
- **Multi-client support**: Handle multiple simultaneous connections using `poll()`
- **Channel management**: Create, join, and manage channels with various modes
- **User authentication**: Password protection for server access
- **Command processing**: Support for standard IRC commands
- **Channel modes**: Invite-only, password-protected, operator privileges
- **Real-time messaging**: Public and private messaging capabilities
- **User management**: Nickname and username handling
- **Error handling**: Comprehensive error checking and user feedback
- **(Bonus) Bot integration**: Interactive bot with special commands (`!help`, `!time`, etc.)

---

## Installation
### Prerequisites
- C++ compiler (supporting C++98 or later)
- Makefile
- Linux/Unix environment (for socket programming)

### Steps
1. Clone the repository:
   ```bash
   git clone <repository-url>
   cd ft_irc
   ```
2. Compile the project:
   ```bash
   make
   ```
3. Run the server:
   ```bash
   ./ircserv <port> <password>
   ```
   - `<port>`: Port number (1024-65535)
   - `<password>`: Server password for client authentication

---

## Usage
### Starting the Server
```bash
./ircserv 6667 mypassword
```

### Connecting as a Client
Use an IRC client (e.g., `irssi`, `nc`) to connect:
```bash
nc localhost 6667
```
After connecting, authenticate with the server password:
```
PASS mypassword
```

### (Bonus) Adding the Bot
Admins can add the bot to any channel using:
```
/addbot <channel>
```
The bot will join the channel and respond to commands prefixed with `!`

---

## Commands
### General Commands
| Command       | Description                                  | Example                     |
|---------------|----------------------------------------------|-----------------------------|
| `/help`       | Show help for commands                       | `/help join`                |
| `/nick`       | Change your nickname                         | `/nick alice`               |
| `/user`       | Set your username                            | `/user alice`               |
| `/quit`       | Disconnect from the server                   | `/quit Goodbye!`            |
| `/who`        | List users in the current channel            | `/who`                      |
| `/addbot` (bonus) | Add bot to channel                     | `/addbot #general`          |

### Channel Commands
| Command       | Description                                  | Example                     |
|---------------|----------------------------------------------|-----------------------------|
| `/join`       | Join a channel                               | `/join #general`            |
| `/part`       | Leave a channel                              | `/part #general`            |
| `/list`       | List all channels                            | `/list`                     |
| `/invite`     | Invite a user to a channel                   | `/invite bob #general`      |
| `/kick`       | Kick a user from a channel                   | `/kick bob #general`        |
| `/topic`      | Set or view the channel topic                | `/topic #general Hello!`    |
| `/mode`       | Set channel modes                            | `/mode #general +i`         |

### Messaging Commands
| Command       | Description                                  | Example                     |
|---------------|----------------------------------------------|-----------------------------|
| `/msg`        | Send a message to a user or channel          | `/msg #general Hello!`      |
| `/privmsg`    | Alias for `/msg`                             | `/privmsg bob Hi!`          |

### (Bonus) Bot Commands
| Command       | Description                                  | Example                     |
|---------------|----------------------------------------------|-----------------------------|
| `!help`       | Show bot commands                            | `!help`                     |
| `!hello`      | Greet the bot                                | `!hello`                    |
| `!time`       | Get current time                             | `!time`                     |
| `!quote`      | Get random quote                             | `!quote`                    |
| `!roll`       | Roll dice (XdY format)                       | `!roll 2d6`                 |

---

## Technical Details
### Key Components
1. **Server Core**:
   - Uses `poll()` for multiplexing I/O
   - Handles client connections and disconnections
   - Manages channels and user interactions

2. **Client Management**:
   - Tracks client state (authenticated/unauthenticated)
   - Manages nicknames, usernames, and passwords

3. **Channel Management**:
   - Supports modes like invite-only (`+i`), password-protected (`+k`), etc.
   - Tracks operators and regular users

4. **Command Processing**:
   - Parses and executes commands
   - Validates arguments and permissions

5. **(Bonus) Bot System**:
   - Special client with automated responses
   - Processes commands prefixed with `!`
   - Maintains separate command handlers

### Error Handling
- Invalid commands or arguments trigger user-friendly error messages
- Server logs events and errors for debugging

---

## System Architecture

### Design Philosophy
The server follows a monolithic architecture with:
- Single-threaded event loop using `poll()`
- Object-oriented design with clear separation of concerns
- Non-blocking I/O operations

### Key Modules
1. **Server Core** - Manages connections and event loop
2. **Client Handler** - Processes client requests and maintains state
3. **Channel Manager** - Handles channel operations and modes
4. **Command Processor** - Parses and executes IRC commands
5. **(Bonus) Bot Module** - Handles automated interactions

### Data Flow
```
Client Connection → Server Core → Command Parser → Command Executor → Response Generator → Client
                      ↑
                  (Bonus) Bot Integration
```

---

## Core Components

### Server Class (`Server.hpp/cpp`)
- Manages the main event loop
- Handles client connections/disconnections
- Maintains lists of clients and channels
- Implements signal handling for graceful shutdown
- **(Bonus)** Manages bot instance and integration

Key Methods:
- `initServer()` - Initializes server socket
- `runServer()` - Main event loop
- `monitorPollEvent()` - Handles I/O events
- `execCmd()` - Command dispatcher
- `initBot()` - (Bonus) Initializes bot instance

### Client Class (`Client.hpp/cpp`)
Represents a connected user with:
- Socket file descriptor
- Authentication state
- Nickname/username
- Current channel
- Message buffer

### Channel Class (`Channel.hpp/cpp`)
Manages channel state including:
- Name and topic
- Client lists (regular and operators)
- Invite lists
- Mode flags (i, t, k, o, l)

### (Bonus) Bot Class (`Bot.hpp/cpp`)
Extends Client with:
- Automated command processing
- Response generation
- Special command handlers
- Quote database

---

## Command Protocol

### Command Structure
All commands follow the format:
```
/command [arguments]
```
(Bonus) Bot commands use:
```
!command [arguments]
```

### Command Processing Pipeline
1. Input received via `poll()`
2. Parsed into tokens
3. Validated (arguments, permissions)
4. Executed with appropriate state changes
5. Responses sent to relevant clients

### (Bonus) Bot Command Handling
1. Messages starting with `!` are routed to bot
2. Bot processes command and generates response
3. Response sent to channel or user

---

## Channel Management

### Channel Modes

| Mode | Description | Example |
|------|-------------|---------|
| i | Invite-only | `/MODE #general +i` |
| t | Topic restricted to ops | `/MODE #general +t` |
| k | Password protected | `/MODE #general +k secret` |
| o | Operator privileges | `/MODE #general +o alice` |
| l | User limit | `/MODE #general +l 10` |

### (Bonus) Bot Channel Presence
- Added via `/addbot` command
- Appears as special client in channel
- Responds to `!` commands in channel

---

## Performance Considerations

### Scalability
- Current implementation handles ~100 concurrent clients
- Bottlenecks:
  - Linear searches in client/channel lists
  - Single-threaded design

### Optimization Opportunities
1. Use hash maps for client/channel lookup
2. Implement connection pooling
3. Add threading for I/O operations

---

## Limitations

1. **Single Server**: No server-to-server protocol
2. **Persistence**: No channel/user persistence between restarts
3. **Security**: Basic password authentication only
4. **Scalability**: Performance degrades with >100 clients

---

## Possible Improvements

1. **IRCv3 Support**:
   - SASL authentication
   - Message tags
   - Capability negotiation

2. **Enhanced Features**:
   - Channel history
   - File transfers
   - Server clusters

3. **Performance**:
   - Epoll/kqueue support
   - Multithreading
   - Connection pooling

4. **(Bonus) Bot Enhancements**:
   - More interactive features
   - Plugin system
   - Persistent memory

---

## Appendix: Class Diagrams

### Server Class
```
+---------------------+
|       Server        |
+---------------------+
| - port: int         |
| - pwd: string       |
| - servFd: int       |
| - clients: vector   |
| - channels: vector  |
| - fds: pollfd[]     |
| - bot: Bot* (bonus) |
+---------------------+
| + initServer()      |
| + runServer()       |
| + execCmd()         |
| + initBot() (bonus) |
+---------------------+
```

### (Bonus) Bot Class
```
+---------------------+
|        Bot          |
+---------------------+
| - botName: string   |
| - welcomeMessage    |
| - quotes: vector    |
+---------------------+
| + handleMessage()   |
| + sendResponse()    |
| + joinChannel()     |
| + isCommandForMe()  |
+---------------------+
```

This combined documentation provides a complete reference for both standard features and the (bonus) bot functionality, with clear demarcation of bonus content.
