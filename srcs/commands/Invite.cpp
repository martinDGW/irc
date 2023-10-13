/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdegraeu <mdegraeu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 19:24:25 by tde-vlee          #+#    #+#             */
/*   Updated: 2023/05/31 14:26:38 by mdegraeu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../server/Server.hpp"

void	Server::invite(User &user, std::string &args)
{
	if (args.find(' ') == args.npos) {
		this->send_msg(user.getSocket_fd(), ERR_NEEDMOREPARAMS(user.getRealname(), user.getNickname(), "INVITE"));
	}
	else {
		std::string	target, channel;
		target = args.substr(0, args.find(" "));
		channel = args.substr(args.find(" ") + 1, args.npos);
		if (!this->isInChan(user.getNickname(), channel)) {
			this->send_msg(user.getSocket_fd(), ERR_NOTONCHANNEL(user.getRealname(), user.getNickname(), channel));
		}
		else {
			if (!this->userExist(target)) {
				this->send_msg(user.getSocket_fd(), ERR_NOSUCHNICK(user.getRealname(), target));
				return ;
			}
			if (!this->chanExist(channel)) {
				this->send_msg(user.getSocket_fd(), ERR_NOSUCHCHANNEL(user.getRealname(), user.getNickname(), channel));
				return ;
			}
			if (this->_channel_map.at(channel)->userExist(target)) {
				this->send_msg(user.getSocket_fd(), ERR_USERONCHANNEL(user.getRealname(), user.getNickname(), target, channel));
				return ;
			}
			this->_channel_map.at(channel)->addInviteList(target);
			this->send_msg(user.getSocket_fd(), RPL_INVITING(user.getRealname(), user.getNickname(), target, channel));
			this->send_msg(this->getTargetSock(target), RPL_INVITED(user.getRealname(), user.getNickname(), target, channel));
		}
	}
}
