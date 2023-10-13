/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tde-vlee <tde-vlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/02 16:17:00 by mdegraeu          #+#    #+#             */
/*   Updated: 2023/06/06 17:06:40 by tde-vlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>

#include "../server/Server.hpp"

bool	isModeSetter(char c) {
	std::string valid("itbkol");
	if (valid.find(c) != valid.npos)
		return (true);
	return (false);
}

std::vector<std::string> getVecArgs(std::string &str) {
	std::vector<std::string> vec;
	size_t	x = 0;
	for (size_t i = 0; i < str.length(); i++) {
		if (str[i] == ' ') {
			vec.push_back(str.substr(x, i - x));
			x = i + 1;
		}
	}
	vec.push_back(str.substr(x));
	return (vec);
}

int	stoi(std::string &str) {
	for (size_t i = 0; i < str.length(); i++) {
		if (str[i] < '0' || str[i] > '9')
			return (-1);
	}
	std::istringstream conv(str);
	int	ret;
	conv >> ret;
	return (ret);
}

void	Server::mode(User &user, std::string &str) {
	if (this->chanExist(str)) {
		this->send_msg(user.getSocket_fd(), RPL_CHANNELMODEIS(user.getRealname(), user.getNickname(), str, this->_channel_map.find(str)->second->getMode()));
		this->send_msg(user.getSocket_fd(), RPL_CREATIONTIME(user.getRealname(), user.getNickname(), str, this->getChannel(str)->getCreationTime()));
		return ;
	}
	if (str[0] == '#') {
		std::string	channel = str.substr(0, str.find(' '));
		std::string modestr = str.substr(str.find(' ') + 1);
		if (!this->chanExist(channel)) {
			this->send_msg(user.getSocket_fd(), ERR_NOSUCHCHANNEL(user.getRealname(), user.getNickname(), channel));
			return ;
		}
		chmap::iterator it = this->_channel_map.find(channel);
		if (!it->second->isAdmin(user.getNickname())) {
			this->send_msg(user.getSocket_fd(), ERR_CHANOPRIVSNEEDED(user.getRealname(), user.getNickname(), channel));
			return ;
		}
		this->chanMode(user, *it->second, getVecArgs(modestr));
	}
}

void	Server::chanMode(User &user, Channel &channel, std::vector<std::string> vec) {
	char c = vec[0][0];
	if (c != '+' && c != '-')
		this->send_msg(user.getSocket_fd(), ERR_NEEDMOREPARAMS(user.getRealname(), user.getNickname(), "MODE"));

	if (vec.size() == 1) {
		for (size_t i = 1; i < vec[0].length(); i++) {
			if (isModeSetter(vec[0][i])) {
				channel.addRemoveMode(vec[0][i], c);
			}
			else
				this->send_msg(user.getSocket_fd(), ERR_UNKNOWNMODE(user.getRealname(), user.getNickname(), vec[0][i]));
		}
		this->send_msg(user.getSocket_fd(), RPL_CHANNELMODEIS(user.getRealname(), user.getNickname(), channel.getName(), channel.getMode()));
	}
	else {
		for (size_t i = 1; i < vec.size(); i++) {
			for (size_t j = 1; j < vec[0].length(); j++) {
				switch (vec[0][j]) {
					case 'i':
						if (c == '+')
						{
							std::string param = vec[i] + " " + channel.getName();
							this->invite(user, param);
						}
						else {
							if (channel.isInvited(vec[i]))
								this->send_msg(this->getTargetSock(vec[i]), RPL_UNINVITED(user.getRealname(), vec[i], channel.getName()));
							channel.removeInviteList(vec[i]);
						}
						break ;
					case 't':
							channel.addRemoveMode('t', c);
						break ;
					case 'b':
						if (c == '+')
						{
							if (this->isInChan(vec[i], channel.getName()))
								channel.ban(user, vec[i]);
							if (channel.getUserCount() == 0)
								this->removeChannel(channel.getName());
						}
						else if (c == '-')
						{
							if (channel.isBan(vec[i]))
								channel.unban(user, vec[i]);
						}
						break ;
					case 'k' :
						if (!channel.isMode('k')) {
							this->send_msg(user.getSocket_fd(), ERR_UNKNOWNMODE(user.getRealname(), user.getNickname(), vec[0][j]));			
							break ;
						}
						if (c == '+') {
							if (vec.size() > 2) {
								this->send_msg(user.getSocket_fd(), ERR_INVALIDKEY(user.getRealname(), user.getNickname(), channel.getName()));
								return ;
							}
							channel.setKey(user, vec[i]);
						}
						else if (c == '-')
							channel.removeKey(user);
						break ;
					case 'o' :
						if (!this->isInChan(vec[i], channel.getName()))
							this->send_msg(user.getSocket_fd(), ERR_USERNOTINCHANNEL(user.getRealname(), user.getNickname(), vec[i], channel.getName()));
						else {
							if (c == '+') {
								channel.addAdmin(*this->getUser(vec[i]));
								channel.send_msg(RPL_ADDOP(user.getRealname(), vec[i], channel.getName()));
							}
							else if (c == '-') {
								channel.removeAdmin(user, vec[i]);
								channel.send_msg(RPL_DELOP(user.getRealname(), vec[i], channel.getName()));
							}
						}
						break ;
					case 'l' :
						if (!channel.isMode('l')) {
							this->send_msg(user.getSocket_fd(), ERR_UNKNOWNMODE(user.getRealname(), user.getNickname(), vec[0][j]));			
							break ;
						}
						if (c == '+')
							channel.addRemoveMaxUsers(stoi(vec[i]));
						else if (c == '-')
							channel.addRemoveMaxUsers(-1);
						break ;
				}
			}
		}
	}
}
