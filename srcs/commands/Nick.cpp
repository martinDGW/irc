/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cben-bar <cben-bar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 13:25:23 by mdegraeu          #+#    #+#             */
/*   Updated: 2023/05/30 13:42:52 by cben-bar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../server/Server.hpp"
#include "../includes/Replies.hpp"

static short checkNickname(const std::string &nickname)
{
	static const std::string	reserved_nick[] = { "NickServ", "ChanServ", "OperServ", "MemoServ", "BotServ", "HostServ", "HelpServ", "Global", "Services", "Oper", "User", "Server", "Root", "Admin", "Sysop", "Owner", "Founder", "Coordinator", "Op", "Voice" };
	static const std::string	special = " -[]`^{}";

	for (size_t idx = 0; idx < nickname.size(); idx ++)
	{
		if ( !std::isalnum(nickname[idx]) && special.find(nickname[idx]) == nickname.npos)
			return (1);
	}
	for (size_t idx = 0; idx < 20; idx++)
	{
		if (nickname == reserved_nick[idx])
			return (1);
	}
	return (0);
}

/**
 * NICK
 * 
 * Command: NICK
 * Parameters: <nickname>
 * 
 * The NICK command is used to give the client a nickname or change the previous one.
 * 
 * If the server receives a NICK command from a client where the desired nickname is already in use on the network, it should issue an ERR_NICKNAMEINUSE numeric and ignore the NICK command.
 * 
 * If the server does not accept the new nickname supplied by the client as valid (for instance, due to containing invalid characters), it should issue an ERR_ERRONEUSNICKNAME numeric and ignore the NICK command.
 * 
 * If the server does not receive the <nickname> parameter with the NICK command, it should issue an ERR_NONICKNAMEGIVEN numeric and ignore the NICK command.
 * 
 * The NICK message may be sent from the server to clients to acknowledge their NICK command was successful, and to inform other clients about the change of nickname. In these cases, the <source> of the message will be the old nickname [ [ "!" user ] "@" host ] of the user who is changing their nickname.
 * 
 * Numeric Replies:
 * 
 * ERR_NONICKNAMEGIVEN (431)
 * ERR_ERRONEUSNICKNAME (432)
 * ERR_NICKNAMEINUSE (433)
 * ERR_NICKCOLLISION (436)
 * 
 * @param user 
 * @param nickname 
 */
void	Server::nick(User &user, std::string &nickname)
{
	if (nickname.empty())
	{
		send_msg(user.getSocket_fd(), ERR_NONICKNAMEGIVEN(this->_servername));
		return ;
	}
	if (checkNickname(nickname))
	{
		send_msg(user.getSocket_fd(), ERR_ERRONEUSNICKNAME(this->_servername, nickname));
		return ;
	}
	if (user.getNickname() == nickname)
	{
		send_msg(user.getSocket_fd(), ERR_NICKCOLLISION(this->_servername, user.getNickname()));
		return ;
	}
	if (this->userExist(nickname))
	{
		this->send_msg(user.getSocket_fd(), ERR_NICKNAMEINUSE(this->_servername, user.getNickname()));
		return ;
	}
	for (chmap::iterator it = this->_channel_map.begin(); it != this->_channel_map.end(); it++)
	{
		if (it->second->userExist(user.getNickname()))
			it->second->rename(user.getNickname(), nickname);
	}
	if (!user.getRealname().empty())
		this->send_msg(user.getSocket_fd(), RPL_NICK(user.getRealname(), nickname));
	for (chmap::iterator chanit = this->_channel_map.begin(); chanit != this->_channel_map.end(); chanit++)
	{
		chanit->second->rename(user.getNickname(), nickname);
	}
	user.setNickname(nickname);
}
