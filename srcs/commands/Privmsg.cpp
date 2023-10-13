/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdegraeu <mdegraeu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 13:23:19 by mdegraeu          #+#    #+#             */
/*   Updated: 2023/06/06 16:31:52 by mdegraeu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../server/Server.hpp"

void	Server::privmsg(User &user, std::string &str)
{
	std::string target(str.substr(0, str.find(" :")));

	if (target.length() == 0)
	{
		this->send_msg(user.getSocket_fd(), ERR_NORECIPIENT(user.getRealname(), target));
		return ;
	}
	if (target == str)
	{
		this->send_msg(user.getSocket_fd(), ERR_NOTEXTTOSEND(user.getRealname(),user.getNickname()));
		return ;
	}
	str = str.substr(str.find(":"), str.npos);
	if (target[0] == '#')
	{
		if (!this->chanExist(target)) {
			this->send_msg(user.getSocket_fd(), ERR_NOSUCHCHANNEL(user.getRealname(), user.getNickname(), target));
		}
		else if (!this->isInChan(user.getNickname(), target)) {
			this->send_msg(user.getSocket_fd(), ERR_CANNOTSENDTOCHAN(user.getRealname(), user.getNickname(), target));
		}
		else {
			this->send_to_channel(user, *this->getChannel(target), RPL_PRIVMSG(user.getRealname(), target, str));
		}
	}
	else
	{
		if (!this->userExist(target))
			this->send_msg(user.getSocket_fd(), ERR_NOSUCHNICK(user.getRealname(), target));
		else
		{
			usrmap::iterator it = this->_user_map.find(this->getTargetSock(target));
			this->send_msg(it->first, RPL_PRIVMSG(user.getRealname(), target, str));
		}
	}
}
