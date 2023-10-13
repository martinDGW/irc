/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Whois.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdegraeu <mdegraeu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/01 15:39:32 by mdegraeu          #+#    #+#             */
/*   Updated: 2023/06/06 14:48:19 by mdegraeu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../server/Server.hpp"

void	Server::whois(User &user, std::string &str)
{
	std::string t(str.substr(0, str.find(' ')));
	if(userExist(t))
	{
		User *target = this->getUser(t);
	
		this->send_msg(user.getSocket_fd(), RPL_WHOISREGNICK(user.getRealname(), target->getNickname()));
		this->send_msg(user.getSocket_fd(), RPL_WHOISUSER(user.getRealname(), user.getNickname(), target->getNickname(), target->getUserName(), target->getRealname()));
		this->send_msg(user.getSocket_fd(), RPL_WHOISACCOUNT(user.getRealname(), user.getNickname(), target->getNickname(), "NOSERVPRIV"));
		this->send_msg(user.getSocket_fd(), RPL_WHOISSERVER(user.getRealname(), user.getNickname(), target->getNickname(), this->getServerName()));
	}
	else
		send_msg(user.getSocket_fd(), ERR_NOSUCHNICK(user.getRealname(), str));
	send_msg(user.getSocket_fd(), RPL_ENDOFWHOIS(user.getRealname(), user.getNickname()));
}