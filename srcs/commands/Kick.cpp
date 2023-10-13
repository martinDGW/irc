/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tde-vlee <tde-vlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 17:44:27 by mdegraeu          #+#    #+#             */
/*   Updated: 2023/06/06 16:57:45 by tde-vlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../server/Server.hpp"

void	Server::kick(User &user, std::string &str)
{
	std::string channel = str.substr(0, str.find(' '));
	std::string	target = str.substr(str.find(' ') + 1, str.npos);

	if (!this->chanExist(channel))
	{
		this->send_msg(user.getSocket_fd(), ERR_NOSUCHCHANNEL(user.getRealname(), user.getNickname(), channel));
		return ;
	}
	if (!this->isInChan(user.getNickname(), channel))
	{
		this->send_msg(user.getSocket_fd(), ERR_NOTONCHANNEL(user.getRealname(), user.getNickname(), channel));
		return ;
	}
	chmap::iterator it = this->_channel_map.find(channel);
	if (!it->second->isAdmin(user.getNickname())) {
		this->send_msg(user.getSocket_fd(), ERR_CHANOPRIVSNEEDED(user.getRealname(), user.getNickname(), channel));
		return ;
	}
	if (!this->isInChan(target, channel))
	{
		this->send_msg(user.getSocket_fd(), ERR_USERNOTINCHANNEL(user.getRealname(), user.getNickname(), target, channel));
		return ;
	}
	if (it->second->isAdmin(target))
	{
		this->send_msg(user.getSocket_fd(), ERR_CHANOPRIVSNEEDED(user.getRealname(), user.getNickname(), channel));
		return ;
	}
	it->second->send_msg(RPL_KICK(user.getRealname(), target, channel));
	it->second->removeUser(target);
	if (it->second->getUserCount() == 0)
		this->removeChannel(channel);
	channel.erase();
	target.erase();
}