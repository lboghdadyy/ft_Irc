// commands/Invite.cpp
// [YOU] — this is entirely yours


#include "../Server.hpp"
#include "../Client.hpp"
#include "../Channel.hpp"
#include "../replies.hpp"

void handleInvite(Server &server, Client &client, std::vector<std::string> &params)
{
    // [YOU] 1. Check registered
    // [YOU] 2. Check params has 2 elements: nick + #channel (ERR_NEEDMOREPARAMS)
    // [YOU] 3. Find channel → ERR_NOSUCHCHANNEL if missing
    // [YOU] 4. Check caller is IN the channel (ERR_NOTONCHANNEL)
    // [YOU] 5. Check caller is an OPERATOR (ERR_CHANOPRIVSNEEDED)
    // [YOU] 6. Find target client by nick → ERR_NOSUCHNICK if missing
    // [YOU] 7. Check target is NOT already in channel (ERR_USERONCHANNEL)
    // [YOU] 8. addInvited(target.getNick())
    // [YOU] 9. Send RPL_INVITING to caller
    // [YOU] 10. Send ":caller!user@host INVITE target #channel" to target's fd
}