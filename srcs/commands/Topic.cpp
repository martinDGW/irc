/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cben-bar <cben-bar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/10 14:44:16 by cben-bar          #+#    #+#             */
/*   Updated: 2023/05/31 18:40:42 by cben-bar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../server/Server.hpp"
#include <unistd.h>

void	topicProcess(User &user, Channel &channel) {
	if (channel.isTopic()) {
		channel.send_msg(RPL_TOPIC(user.getRealname(), user.getNickname(), channel.getName(), channel.getTopic()));
		channel.send_msg(RPL_TOPICWHOTIME(user.getRealname(), channel.getName(), user.getNickname(), channel.getTopicDate()));
	}
	else
		channel.send_msg(RPL_NOTOPIC(user.getRealname(), user.getNickname(), channel.getName()));
}

//====================================================//

void	Server::topic(User &user, std::string &str)
{
	std::size_t pos1 = str.find(' ');

	std::string	channel(str.substr(0, pos1));

	if (!this->chanExist(channel))
	{
		this->send_msg(user.getSocket_fd(), ERR_NOSUCHCHANNEL(user.getRealname(), user.getNickname(), channel));
		return ;
	}
	if (!isInChan(user.getNickname(), channel))
    {
	    this->send_msg(user.getSocket_fd(), ERR_NOTONCHANNEL(user.getRealname(), user.getNickname(), channel));
		return ;
	}
	if (channel != str) {
		if (this->_channel_map.at(channel)->isMode('t') && !this->_channel_map.at(channel)->isAdmin(user.getNickname()))
		{
			this->send_msg(user.getSocket_fd(), ERR_CHANOPRIVSNEEDED(user.getRealname(), user.getNickname(), channel));
			return ;
		}    

		std::size_t pos2 = str.find(':');
		if (pos2 == str.npos) {
			this->send_msg(user.getSocket_fd(), ERR_NEEDMOREPARAMS(user.getRealname(), user.getNickname(), "TOPIC"));
			return ;
		}
		if (str[pos1 + 1] != str[pos2]) {
			this->send_msg(user.getSocket_fd(), ERR_UNKNOWNERROR(user.getRealname(), user.getNickname(), "TOPIC"));
			return ;
		}

		std::string	topic;
		topic = str.substr(pos2 + 1);
		chmap::iterator it = this->_channel_map.find(channel);
		if (str.empty() || topic == "cleartopic")
			it->second->clearTopic();
		else {
			it->second->setTopic(topic);
			it->second->setTopicDate(dateStr());
		}
	}
	topicProcess(user, *this->getChannel(channel));
}