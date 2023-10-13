/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdegraeu <mdegraeu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/30 11:55:54 by mdegraeu          #+#    #+#             */
/*   Updated: 2023/05/31 14:27:15 by mdegraeu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../server/Server.hpp"

void	Server::part(User &user, std::string &str) {

	std::string	channel(str.substr(0, str.find(" :")));
	std::string	reason(str.substr(str.find(':') + 1));

	if (reason.empty()) {
		this->send_msg(user.getSocket_fd(), ERR_NEEDMOREPARAMS(user.getRealname(), user.getNickname(), "PART"));
		return ;
	}
	if (!this->chanExist(channel)) {
		this->send_msg(user.getSocket_fd(), ERR_NOSUCHCHANNEL(user.getRealname(), user.getNickname(), channel));
		return ;
	}
	chmap::iterator it = this->_channel_map.find(channel);
	if (!it->second->userExist(user.getNickname())) {
		this->send_msg(user.getSocket_fd(), ERR_NOTONCHANNEL(user.getRealname(), user.getNickname(), channel));
		return ;
	}
	it->second->send_msg(RPL_PART(user.getRealname(), channel, reason));

	it->second->removeUser(user.getNickname());
	if (it->second->getUserCount() == 0)
		this->removeChannel(channel);
	channel.erase();
	reason.erase();
}
