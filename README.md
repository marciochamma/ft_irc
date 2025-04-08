# Server-Client Application

This project is a server-client application written in C++. It sets up a server that listens for client connections, manages these connections, processes client input, and handles various commands from the clients.

## Features

- **Server Initialization**: Sets up a server socket to listen for incoming connections.
- **Client Connection**: Accepts new clients, sets them to non-blocking mode, and prompts for a password.
- **Command Handling**: Parses and executes commands from authenticated clients.
- **Channel Management**: Allows clients to join, leave, and interact within communication channels.
- **Signal Handling**: Gracefully shuts down the server upon receiving an interrupt signal.

## Classes

### Server

The `Server` class is responsible for initializing and running the server, handling client connections, and processing commands.

- **Initialization and Cleanup**: The constructor initializes the server, while the destructor ensures all file descriptors (FDs) are closed and resources are cleaned up.
- **Signal Handling**: The `signalHandler` method sets a signal flag when an interrupt signal is received.
- **Server Initialization**: The `initServer` method initializes the server with a specified port and password, sets up the server socket, and prepares for incoming connections.
- **Running the Server**: The `runServer` method starts the server loop, monitoring file descriptors for events and handling them accordingly.
- **Handling Poll Events**: The `monitorPollEvent` method processes events on different file descriptors, such as new client connections or input from connected clients.
- **Managing Clients**: Methods like `connectClient`, `handleClient`, `disconnectClient`, and `clearClientFromVector` manage client connections, data reception, and disconnections.
- **Command Execution**: The `execCmd` and `driveActions` methods parse and execute commands received from clients.
- **Authentication**: The `authenticate` method verifies client passwords before allowing further interaction.

### Client

The `Client` class represents a connected client, storing information such as file descriptor, IP address, username, nickname, current channel, authentication status, and buffer for incoming data.

- Provides methods to get and set these attributes.

### Channel

The `Channel` class represents a communication channel that clients can join.

- Stores information such as channel name, topic, password, client list, operator list, and modes (e.g., invite-only, topic protection).
- Provides methods to manage clients and operators within the channel.

## Setup

1. **Clone the Repository**:
   ```sh
   git clone <repository-url>
   ```

2. **Build the Project**:
   ```sh
   make r
   ```

3. **Run the Server**:
   ```sh
   ./server <port> <password>
   ```
4. **Run the Client**:
   ```sh
   nc 127.0.0.1 <port>
   ```

## Usage

- **Starting the Server**: Run the server executable with the desired port and password.
- **Connecting Clients**: Clients can connect to the server using the specified port. They will be prompted to enter the password.
- **Commands**: Authenticated clients can send various commands to interact with the server and other clients in channels.

