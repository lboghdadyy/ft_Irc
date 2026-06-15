// commands/Join.cpp
// [YOU] — this is entirely yours

#include "../Server.hpp"
#include "../Client.hpp"
#include "../Channel.hpp"
#include "../replies.hpp"

// Called by Server::_dispatchCommand() when it sees "JOIN"
// params[0] = "#channelname"
// params[1] = "key" (optional, for +k mode)
void handleJoin(Server &server, Client &client, std::vector<std::string> &params)
{
    // [YOU] 1. Check client is registered
    // [YOU] 2. Check params not empty (ERR_NEEDMOREPARAMS)
    // [YOU] 3. Validate channel name starts with '#'
    // [YOU] 4. Find or create channel
    // [YOU] 5. Check +i mode → is client in inviteList?
    // [YOU] 6. Check +k mode → does key match?
    // [YOU] 7. Check +l mode → is channel full?
    // [YOU] 8. addMember(client), if first member: addOperator(client)
    // [YOU] 9. removeInvited(client.getNick()) if they were invited
    // [YOU] 10. Send JOIN confirmation to client
    // [YOU] 11. Send RPL_TOPIC or RPL_NOTOPIC
    // [YOU] 12. Send RPL_NAMREPLY (list of members) + RPL_ENDOFNAMES
    // [YOU] 13. Broadcast ":nick!user@host JOIN #channel" to all OTHER members
}