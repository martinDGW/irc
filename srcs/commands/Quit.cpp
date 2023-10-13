/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tde-vlee <tde-vlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 19:23:06 by tde-vlee          #+#    #+#             */
/*   Updated: 2023/05/30 18:42:37 by tde-vlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../server/Server.hpp"

void	Server::quit(User &user, std::string &args)
{
	size_t idx;

	for (chmap::iterator chan_it = this->_channel_map.begin(); chan_it != this->_channel_map.end(); chan_it++)
	{
		if (chan_it->second->userExist(user.getNickname()))
			chan_it->second->send_msg(RPL_PART(user.getRealname(), chan_it->second->getName(), args));
	}
	idx = 1;
	while (idx < this->_all_pollfds.size() && user.getSocket_fd() != this->_all_pollfds.at(idx).fd)
		idx++ ;
	this->removeUser(user.getSocket_fd(), idx);
}
