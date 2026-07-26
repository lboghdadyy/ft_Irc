#include "../../includes/server.hpp"

void UpperCaseCommand(std::string& s) {
    for (size_t i = 0; i < s.length(); i++)
        s[i] = std::toupper(s[i]);
}

void extractCommand(std::string& line, std::string& Command, std::string& rest) {
    size_t pos;
    if ((pos = line.find(' ')) != std::string::npos) {
        Command = line.substr(0, pos);
        rest = line.substr(pos, line.length() - 1);
    } else {
        Command = line;
        rest = "";
    }
}

void extractParams(std::string& rest, std::vector<std::string>& params) {
    while (!rest.empty()) {
        while (!rest.empty() && rest[0] == ' ')
            rest.erase(0, 1);
        if (rest.empty())
            break;
        if (rest[0] == ':') {
            rest.erase(0, 1);
            params.push_back(rest);
            rest.clear();
        } else {
            size_t pos = rest.find(' ');
            if (pos == std::string::npos) {
                params.push_back(rest);
                rest.clear();
            } else {
                params.push_back(rest.substr(0, pos));
                rest.erase(0, pos + 1);
            }
        }
    }
}

void Server::handleCommand(int fd, std::string line) {
    std::string Command;
    std::string rest;
    
    extractCommand(line, Command, rest);
    UpperCaseCommand(Command);
    std::vector<std::string> params;
    extractParams(rest, params);
    
    if (Command == "PASS")
        cmdPass(fd, params);
    else if(Command == "NICK")
        cmdNick(fd, params);
    else if(Command == "USER")
        cmdUser(fd, params);

    // else if(Command == "PRIVMSG")
        // cmdPrivmsg(fd, params);
    // else if(Command == "PING")
        // cmdPing(fd, params);
    // else if(Command == "QUIT")
        // cmdQuit(fd, params);
    if (Command == "JOIN")
        cmdJoin(fd, params);
    else if (Command == "INVITE")
        cmdInvite(fd, params);
    else if(Command == "KICK")
        cmdKick(fd, params);
    // else if(Command == "TOPIC") // ??
        // cmdTopic(fd, params);
    // else if(Command == "MODE")
        // cmdMode(fd, params);

    // ---- unknown ----
    // else
    //     // do nothing for now, or later: send 421 ERR_UNKNOWNCOMMAND
    //     do nothing
}