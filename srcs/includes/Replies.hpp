/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Replies.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdegraeu <mdegraeu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 13:18:54 by mdegraeu          #+#    #+#             */
/*   Updated: 2023/06/06 16:21:15 by mdegraeu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REPLIES_HPP
# define REPLIES_HPP

#define LOCAL_HOST_IP "127.0.0.1"
#define SERVINFO std::string("IRC server")

# define RPL_WELCOME(repName, nickName) ":" + repName + " 001 " + nickName + " :Welcome to the Server!"
# define RPL_NICK(repName, nickName) ":" + repName + " NICK " + nickName
# define RPL_JOIN(repName, channel) ":" + repName + " JOIN " + channel
# define RPL_PRIVMSG(repName, target, str) ":" + repName + " PRIVMSG " + target + " " + str
# define RPL_NOTICE(repName, target, str) ":" + repName + " NOTICE " + target + " " + str
# define RPL_KICK(repName, target, channel) ":" + repName + " KICK " + channel + " " + target
# define RPL_INVITED(repName, invitor, invited, channel) ":" + repName + " INVITE " + invited + " " + channel
# define RPL_UNINVITED(repName, invited, channel) ":" + repName + " MODE " + channel + " -i " + invited
# define RPL_BAN(repName, target, channel) ":" + repName + " MODE " + channel + " +b " + target
# define RPL_UNBANTOCHAN(repName, target, channel) ":" + repName + " MODE " + channel + " -b " + target
# define RPL_ADDOP(repName, nickname, channel) ":" + repName + " MODE " + channel + " +o " + nickname
# define RPL_DELOP(repName, nickname, channel) ":" + repName + " MODE " + channel + " -o " + nickname
# define RPL_PART(repName, channel, reason) ":" + repName + " PART " + channel + " :" + reason


# define RPL_WHOISREGNICK(repname, nickname) ":" + repname + " 307 " + nickname + " has identified for this nick"
# define RPL_WHOISUSER(repName, nickname, targetnick, targetusername, targetrealname) ":" + repName + " 311 " + nickname + " " + targetnick + " " + targetusername + " " + LOCAL_HOST_IP + " * :" + targetrealname
# define RPL_WHOISSERVER(repName, nickname, targetnick, servername) ":" + repName + " 312 " + nickname + " " + targetnick + " " + servername + " " + SERVINFO
# define RPL_ENDOFWHO(repName, nickname, channel) ":" + repName + " 315 " + nickname + " "  + channel + " :End of WHO list"
# define RPL_ENDOFWHOIS(repName, nickName) ":" + repName + " 318 " + nickName + ": End of /WHOIS list"


# define RPL_LISTSTART(repName, nickname, channel) ":" + repName + " 321 " + nickname + " " + channel + " :"
# define RPL_LIST(repName, nickname, channel, user_count, topic) ":" + repName + " 322 " + nickname + " " + channel + " " + user_count + " :" + topic
# define RPL_LISTEND(repName, nickname) ":" + repName + " 323 " + nickname + " :End of /LIST"

# define RPL_CHANNELMODEIS(repName, nickname, channel, modestr) ":" + repName + " 324 " + nickname + " " + channel + " " + modestr
# define RPL_CREATIONTIME(repName, nickname, channel, creationTime) ":" + repName + " 329 " + nickname + " " + channel + " " + creationTime
# define RPL_WHOISACCOUNT(repName, nickname, targetnick, account) ":" + repName + " 330 " + nickname + " " + targetnick + " " + account
# define RPL_NOTOPIC(repName, nickname, channel) ":" + repName + " 331 " + nickname + " " + channel + " :No topic is set"
# define RPL_TOPIC(repName, nickname, channel, topic) ":" + repName + " 332 " + nickname + " " + channel + " :" + topic
# define RPL_TOPICWHOTIME(repName, channel, nickname, date) ":" + repName  + " 333 " + repName + " " + channel + " " + nickname + " " + date

# define RPL_INVITING(repName, invitor, invited, channel) ":" + repName + " 341 " + invitor + " " + invited + " " + channel
# define RPL_WHOREPLY(repName, channel, nickname, targetname, targetrealname, flags) ":" + repName + " 352 " + channel + " " + nickname + " " + targetname + " " + flags + " :1 " + targetrealname
# define RPL_NAMEREPLY(repName, nickname, symbol, channel, list) ":" + repName + " 353 " + nickname + " " + symbol + " " + channel + " :" + list
# define RPL_ENDOFNAMES(repName, nickname, channel) ":" + repName + " 366 " + nickname + " " + channel + " :End of /NAMES list"

# define ERR_UNKNOWNERROR(repName, nickname, command) ":" + repName + " 400 " + nickname + " " + command + " : Unkownerror, certainly due to an invalid argument."
# define ERR_NOSUCHNICK(repName, target) ":" + repName + " 401 " + target + " :No such target '" + target + "'"
# define ERR_NOSUCHCHANNEL(repName, nickname, channel) ":" + repName + " 403 " + nickname + " " + channel + ":No such channel"

# define ERR_CANNOTSENDTOCHAN(repName, nickname, channel) ":" + repName + " 404 " + nickname + " " + channel + " :Cannot send to channel"
# define ERR_TOOMANYCHANNELS(repName, nickname, channel) ":" + repName + " 405 " + nickname + " " + channel + " :You've joined too many channels"
# define ERR_NORECIPIENT(repName, target) ":" + repName + " 411 " + target + " :No recipient specified"
# define ERR_NOTEXTTOSEND(repName, nickname) ":" + repName + " 412 " + nickname + " :No text to send"

# define ERR_UNKNOWNCOMMAND(repName, nickname, command) ":" + repName + " 421 " + nickname + " " + command + " :Unknown command"

# define ERR_NONICKNAMEGIVEN(servername) ":" + servername + " 431 :No nickname given"
# define ERR_ERRONEUSNICKNAME(servername, nickname) ":" + servername + " 432 " + nickname + " :Erroneus nickname"
# define ERR_NICKNAMEINUSE(servername, nickname) ":" + servername + " 433 " + nickname + " :Nickname is already in use"
# define ERR_NICKCOLLISION(servername, nickname) ":" + servername + " 436 " + nickname + " :Nickname collision KILL"

# define ERR_USERNOTINCHANNEL(repName, nickname, target, channel) ":" + repName + " 441 " + nickname + " " + target + " " + channel + " :Not on that channel"
# define ERR_NOTONCHANNEL(repName, nickname, channel) ":" + repName + " 442 " + nickname + " " + channel + " :You're not on that channel"

# define ERR_NOTREGISTERED(servername, command) ":" + servername + " 451 " + command + " :You have not registered"

# define ERR_USERONCHANNEL(repName, nickname, target, channel) ":" + repName + " 443 " + nickname + " " + target + " " + channel + " :is already on channel"
# define ERR_NEEDMOREPARAMS(repName, nickname, command) ":" + repName + " 461 " + nickname + " " + command + " :Not enough parameters"
# define ERR_ALREADYREGISTERED(servername, command) ":" + servername + "462 " + command + " :You may not reregister"
# define ERR_PASSWDMISMATCH(servername) ":" + servername + " 464 :Password incorrect"

# define ERR_CHANNELISFULL(repName, nickname, channel) ":" + repName + " 471 " + nickname + " " + channel + " :Cannot join channel (+l)"
# define ERR_UNKNOWNMODE(repName, nickname, modeChar) ":" + repName + " 472 " + nickname + " " + modeChar + " :isunknown mode char to me"
# define ERR_INVITEONLYCHAN(repName, nickname, channel) ":" + repName + " 473 " + nickname + " " + channel + " :Cannot join channel (+i)"
# define ERR_BANNEDFROMCHAN(repName, nickname, channel) ":" + repName + " 474 " + nickname + " " + channel + " :Cannot join channel (+b)"
# define ERR_BADCHANNELKEY(repName, nickname, channel) ":" + repName + " 475 " + nickname + " " + channel + " :Cannot join channel (+k)"
# define ERR_BADCHANMASK(servername, channel)  ":" + servername + " 476 " + channel + " :Bad Channel Mask"

# define ERR_NOPRIVILEGES(repName, nickname) ":" + repName + " 481 " + nickname + " :Permission Denied- You're not an IRC operator"
# define ERR_CHANOPRIVSNEEDED(repName, nickname, channel) ":" + repName + " 482 " + nickname + " " + channel + " :You're not channel operator"

# define ERR_INVALIDKEY(repName, nickname, channel) ":" + repName + " 525 " + nickname + " " + channel + " :Key is not well-formed"

#endif
