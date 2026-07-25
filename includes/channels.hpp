#ifndef CHANNELS_HPP
#define CHANNELS_HPP


#include "server.hpp"
#include <set>

class Channel {
    private:
        std::string name;          // "#general" — identifies the channel
        std::string topic;         // set by TOPIC command, shown to joiners
        std::string key;           // password; empty string = no password
        std::set<int> members;     // fds of everyone currently in the channel
        std::set<int> operators;   // fds of members with op privileges (subset of members)
        std::set<int> invited;     // fds invited (relevant only when inviteOnly is true)
        bool inviteOnly;           // MODE +i
        bool topicRestricted;      // MODE +t — only ops can change topic
        size_t userLimit;          // MODE +l ; 0 = unlimited
    public:
        Channel();
        Channel(std::string name, int creatorFd);

        void addMember(int fd);
        void removeMember(int fd);
        bool isMember(int fd) const;
        bool empty() const;

        void addOperator(int fd);
        bool isOperator(int fd) const;

        void invite(int fd);
        bool isInvited(int fd) const;
        void removeInvite(int fd);   // clear invite once they've joined

        std::string getName() const;
        std::string getTopic() const;
        void setTopic(std::string t);
        std::string getKey() const;
        void setKey(std::string k);
        bool getInviteOnly() const;
        void setInviteOnly(bool b);
        size_t getUserLimit() const;
        void setUserLimit(size_t n);
        std::set<int>& getMembers();

        ~Channel();
};

#endif