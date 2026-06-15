#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include <string>
#include <set>

class Client;

class Channel
{
private:
    std::string           name;
    std::string           topic;
    std::set<Client*>     members;
    std::set<Client*>     operators;
    std::set<std::string> inviteList;

    // Modes
    bool        inviteOnly;
    std::string key;        // empty = no password
    int         userLimit;  // 0 = no limit

public:
    Channel(const std::string &name, Client *firstOperator);
    ~Channel();

    // ----------------------------------------------------------------
    // Getters — YOU use all of these
    // ----------------------------------------------------------------
    std::string getName()  const;   // "#general"
    std::string getTopic() const;   // bghdadi ay7taj tahowa hadi 

    // ----------------------------------------------------------------
    // Member management — YOU use all of these
    // ----------------------------------------------------------------
    // [YOU] Add a client to the channel (called at the end of JOIN)
    void addMember(Client *client);
    // [YOU] Remove a client from the channel (called in KICK)
    void removeMember(Client *client);
    // [YOU] Check if a client is in the channel (used in INVITE and KICK)
    bool hasMember(Client *client) const;
    // [YOU] Get all members — needed to send NAMREPLY (353) in JOIN
    const std::set<Client*> &getMembers() const;
    // ----------------------------------------------------------------
    // Operator management — YOU use all of these
    // ----------------------------------------------------------------
    // [YOU] Make a client a channel operator (first JOIN makes them op)
    void addOperator(Client *client);
    // [YOU] Remove operator status (when KICKed operator leaves)
    void removeOperator(Client *client);
    // [YOU] Check if client is an operator (required for KICK and INVITE)
    bool isOperator(Client *client) const;
    // ----------------------------------------------------------------
    // Invite list — YOU use all of these
    // ----------------------------------------------------------------
    // [YOU] Add a nickname to the invite list (called in INVITE)
    void addInvited(const std::string &nick);
    // [YOU] Check if a nick was invited (checked in JOIN when +i mode is set)
    bool isInvited(const std::string &nick) const;
    // [YOU] Remove from invite list after they successfully JOIN
    void removeInvited(const std::string &nick);

    // --------------------------------------------
    bool        isInviteOnly() const;    // baghdadi sweb sets +i, oussama ay7taj checks in JOIN
    std::string getKey()       const;    // baghdadi sweb sets +k, oussama ay7taj checks in JOIN
    int         getUserLimit() const;    // baghdadi sweb sets +l, oussama ay7taj checks in JOIN

    /*
    bghdadi parts
    void setInviteOnly(bool val);
    void setKey(const std::string &key);
    void setUserLimit(int limit);
    void setTopic(const std::string &topic);    
    */    
};

#endif