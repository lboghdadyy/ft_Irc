// Numeric reply codes defined in RFC 1459
// [OUSSAMA] use: RPL_NAMREPLY, RPL_ENDOFNAMES, RPL_TOPIC, RPL_INVITING
//            ERR_NOSUCHNICK, ERR_NOSUCHCHANNEL, ERR_NOTONCHANNEL,
//            ERR_CHANOPRIVSNEEDED, ERR_USERNOTINCHANNEL,
//            ERR_USERONCHANNEL, ERR_INVITEONLYCHAN,
//            ERR_BADCHANNELKEY, ERR_CHANNELISFULL, ERR_NEEDMOREPARAMS

#pragma once

// Success replies OUSSAMA use
#define RPL_INVITING      "341"   // INVITE success → sent to caller
#define RPL_TOPIC         "332"   // JOIN: channel has a topic
#define RPL_NOTOPIC       "331"   // JOIN: no topic set
#define RPL_NAMREPLY      "353"   // JOIN: list of names in channel
#define RPL_ENDOFNAMES    "366"   // JOIN: end of NAMREPLY list

// Error replies OUSSAMA use
#define ERR_NOSUCHNICK        "401"  // INVITE/KICK: target nick doesn't exist
#define ERR_NOSUCHCHANNEL     "403"  // JOIN/INVITE/KICK: channel doesn't exist
#define ERR_NEEDMOREPARAMS    "461"  // any command: missing arguments
#define ERR_NOTREGISTERED     "451"  // client hasn't done PASS/NICK/USER yet
#define ERR_NOTONCHANNEL      "442"  // KICK/INVITE: OUSSAMA are not in the channel
#define ERR_CHANOPRIVSNEEDED  "482"  // KICK/INVITE: OUSSAMA are not a channel operator
#define ERR_USERNOTINCHANNEL  "441"  // KICK: target is not in the channel
#define ERR_USERONCHANNEL     "443"  // INVITE: target is already in the channel
#define ERR_INVITEONLYCHAN    "473"  // JOIN: channel is +i and OUSSAMA're not invited
#define ERR_BADCHANNELKEY     "475"  // JOIN: wrong password for +k channel
#define ERR_CHANNELISFULL     "471"  // JOIN: channel hit +l user limit