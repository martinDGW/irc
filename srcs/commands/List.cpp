/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   List.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdegraeu <mdegraeu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/01 15:35:37 by mdegraeu          #+#    #+#             */
/*   Updated: 2023/06/01 15:57:36 by mdegraeu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../server/Server.hpp"

void	Server::list(User &user, std::string &str) {
	if (str.empty()) {
		for (chmap::iterator it = this->_channel_map.begin(); it != this->_channel_map.end(); it++) {
			this->send_msg(user.getSocket_fd(), RPL_LISTSTART(user.getRealname(), user.getNickname(), it->first));
			std::ostringstream conv;
			conv << it->second->getUserCount();
			this->send_msg(user.getSocket_fd(), RPL_LIST(user.getRealname(), user.getNickname(), it->first, conv.str(), it->second->getTopic()));
			this->send_msg(user.getSocket_fd(), RPL_LISTEND(user.getRealname(), user.getNickname()));
		}
	}
	else if (this->chanExist(str)) {
		chmap::iterator it = this->_channel_map.find(str);
		this->send_msg(user.getSocket_fd(), RPL_LISTSTART(user.getRealname(), user.getNickname(), it->first));
		std::ostringstream conv;
		conv << it->second->getUserCount();
		this->send_msg(user.getSocket_fd(), RPL_LIST(user.getRealname(), user.getNickname(), it->first, conv.str(), it->second->getTopic()));
		this->send_msg(user.getSocket_fd(), RPL_LISTEND(user.getRealname(), user.getNickname()));
	}
	else
		this->send_msg(user.getSocket_fd(), RPL_LISTEND(user.getRealname(), user.getNickname()));
}
