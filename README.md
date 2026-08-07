*This project has been created as part of the 42 curriculum by sbaghdad, oufarah, sbouaa*

## Description

ft_irc is a fully custom IRC server written in C++98. It replicates the core behavior of real-world IRC servers by implementing low-level socket networking, IRC protocol parsing, channel management, and client communication without relying on external frameworks.

The server uses a non-blocking event-driven architecture with epoll to efficiently manage multiple simultaneous clients while maintaining full control over authentication, message routing, channel modes, operator privileges, and connection state through a custom command handling system.

## Architecture Overview

The server uses a custom Client and Channel management system to handle:

IRC command parsing
Client authentication and registration
Channel membership and operator states
Message broadcasting and routing
Outgoing message buffering and connection handling

Key design choices:

Non-blocking socket communication
Manual IRC protocol parsing (\r\n based)
Event-driven networking using epoll
Stateful client management for authentication and modes


## Features

-  Non-blocking I/O with epoll
-  Multiple simultaneous clients support
-  IRC authentication (PASS, NICK, USER)
-  Channel management (JOIN, PART, TOPIC)
-  Private and channel messaging (PRIVMSG)
-  Channel operator and mode handling (MODE)
-  Invite-only, password-protected, and limited channels
-  Custom command parsing and message dispatching
-  Robust connection handling and client disconnection management


## Instructions
    ### Requirements

    - Linux environment
    - C++98 compatible compiler (c++)
    - Make
    - IRC client (nc, KvIrc, irssi) for testing

### Compilation

make

### Run

./ircserv \[port\] \[password\]

### Example

./ircserv 8888 passwd

## Usage Examples
#### Connect:
    - nc -C localhost 8888

#### Authenticate:
    - PASS passwd
    - USER username 0 * :real_name
    - NICK nickname
#### Join a channel:
    - JOIN #general
#### Send a message::
    - PRIVMSG #general :hello everyone !
#### Change channel modes:
    - MODE #general +k key

## Resources

-   https://man7.org/linux/man-pages/man7/epoll.7.html
-   https://www.rfc-editor.org/rfc/rfc1459.html


## AI Usage

AI was used as a learning and debugging tool:
- Understanding epoll, TCP Protocol, Sockets, Accept...
- Debugging parsing and file descriptor issues
- Exploring design decisions

All code was reviewed, tested manually, and understood by the authors.


## Authors

-   sbaghdad
-   oufarah
-   sbouaa
