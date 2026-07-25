#include "../includes/channels.hpp"

Channel::Channel() : inviteOnly(false), topicRestricted(false), userLimit(0) {}

Channel::Channel(std::string n, int creatorFd) : name(n), inviteOnly(false), topicRestricted(false), userLimit(0) {
    members.insert(creatorFd);
    operators.insert(creatorFd);
}

void Channel::addMember(int fd) {
    members.insert(fd);
}

void Channel::removeMember(int fd) {
    members.erase(fd);
    operators.erase(fd);
    invited.erase(fd);
}

bool Channel::isMember(int fd) const {
    return members.find(fd) != members.end();
}

bool Channel::empty() const {
    return members.empty();
}

void Channel::addOperator(int fd) {
    operators.insert(fd);
}
bool Channel::isOperator(int fd) const {
    return operators.find(fd) != operators.end();
}

void Channel::invite(int fd) {
    invited.insert(fd);
}
bool Channel::isInvited(int fd) const {
    return invited.find(fd) != invited.end();
}
void Channel::removeInvite(int fd) {
    invited.erase(fd);
}

std::string Channel::getName() const {
    return name;
}
std::string Channel::getTopic() const {
    return topic;
}
void Channel::setTopic(std::string t) {
    topic = t;
}
std::string Channel::getKey() const {
    return key;
}
void Channel::setKey(std::string k) {
    key = k;
}
bool Channel::getInviteOnly() const {
    return inviteOnly;
}
void Channel::setInviteOnly(bool b) {
    inviteOnly = b;
}
size_t Channel::getUserLimit() const {
    return userLimit;
}
void Channel::setUserLimit(size_t n) {
    userLimit = n;
}
std::set<int>& Channel::getMembers() {
    return members;
}

Channel::~Channel() {}