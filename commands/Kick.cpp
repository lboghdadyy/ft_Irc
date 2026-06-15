// commands/Kick.cpp
// [YOU] — this is entirely yours

#include "../Server.hpp"
#include "../Client.hpp"
#include "../Channel.hpp"
#include "../replies.hpp"

void handleKick(Server &server, Client &client, std::vector<std::string> &params)
{
    // [YOU] 1. Check registered
    // [YOU] 2. Check params has at least 2: #channel + nick (ERR_NEEDMOREPARAMS)
    // [YOU] 3. Find channel → ERR_NOSUCHCHANNEL
    // [YOU] 4. Check caller is IN the channel (ERR_NOTONCHANNEL)
    // [YOU] 5. Check caller is OPERATOR (ERR_CHANOPRIVSNEEDED)
    // [YOU] 6. Find target client by nick → ERR_NOSUCHNICK
    // [YOU] 7. Check target IS in the channel (ERR_USERNOTINCHANNEL)
    // [YOU] 8. Build reason string (params[2] or default "Kicked")
    // [YOU] 9. Broadcast ":caller!user@host KICK #channel target :reason"
    //          to ALL members INCLUDING the target (so they see it before leaving)
    // [YOU] 10. removeMember(target), removeOperator(target) if they were op
    // [YOU] 11. If channel now empty → decide (delete it or keep it — ask M1)
}