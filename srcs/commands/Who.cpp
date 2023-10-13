/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Who.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tde-vlee <tde-vlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/02 14:39:23 by mdegraeu          #+#    #+#             */
/*   Updated: 2023/06/01 23:06:58 by tde-vlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../server/Server.hpp"

typedef void (*who_func)(User& user, std::string channel);

static std::vector<std::string>	split(std::string str, const std::string separator)
{
	std::vector<std::string>	str_vec;
	size_t						index;

	if (str.empty())
		return (str_vec);
	index = str.find_first_of(separator);
	while (index != str.npos)
	{
		str_vec.push_back(str.substr(0, index));
		str = str.substr(index + 1);
		index = str.find_first_of(separator);
	}
	str_vec.push_back(str);
	return (str_vec);
}

static void	who_all(Server& serv, User& user)
{
	for (chmap::const_iterator chan_it  = serv.getChannel_map().begin(); chan_it != serv.getChannel_map().end(); chan_it++)
	{
		Channel& chan = *chan_it->second;

		if (chan.userExist(user.getNickname()))
			continue;
		for (std::map<std::string, User *>::const_iterator user_it = chan.getUser_map().begin(); user_it != chan.getUser_map().end(); user_it++)
		{
			User& utarget = *user_it->second;

			if (utarget.getNickname() != user.getNickname())
			{
				serv.send_msg(user.getSocket_fd(), RPL_WHOREPLY(user.getRealname(), "*", user.getNickname(), utarget.getNickname(), utarget.getRealname(), "H"));
			}
		}
	}
	serv.send_msg(user.getSocket_fd(), RPL_ENDOFWHO(user.getRealname(), user.getNickname(), "*"));
}

static void	who_chan(Server& serv, User& user, std::string channel)
{
	Channel*	chan;
	std::string	message;

	if (channel == "0")
	{
		who_all(serv, user);
		return ;
	}
	chan = serv.getChannel(channel);
	for (std::map<std::string, User *>::const_iterator user_it = chan->getUser_map().begin(); user_it != chan->getUser_map().end(); user_it++)
	{
		User& utarget = *user_it->second;

		if (utarget.getNickname() != user.getNickname())
		{
			serv.send_msg(user.getSocket_fd(), RPL_WHOREPLY(user.getRealname(), channel, user.getNickname(), utarget.getNickname(), utarget.getRealname(), "H"));
		}
	}
	serv.send_msg(user.getSocket_fd(), RPL_ENDOFWHO(user.getRealname(), user.getNickname(), channel));
}

static void	who_chop(Server& serv, User& user, std::string channel)
{
	Channel*	chan;
	std::string	message;

	if (channel == "0")
	{
		who_all(serv, user);
		return ;
	}
	chan = serv.getChannel(channel);
	for (std::map<std::string, User *>::const_iterator admin_it = chan->getAdmin_map().begin(); admin_it != chan->getAdmin_map().end(); admin_it++)
	{
		User& utarget = *admin_it->second;

		if (utarget.getNickname() != user.getNickname())
		{
			serv.send_msg(user.getSocket_fd(), RPL_WHOREPLY(user.getRealname(), channel, user.getNickname(), utarget.getNickname(), utarget.getRealname(), "H"));
		}
	}
	serv.send_msg(user.getSocket_fd(), RPL_ENDOFWHO(user.getRealname(), user.getNickname(), channel));
}

/**
 *    Command: WHO
 * Parameters: [ <mask> [ "o" ] ]
 *
 * The WHO command is used by a client to generate a query which returns
 * a list of information which 'matches' the <mask> parameter given by
 * the client.  In the absence of the <mask> parameter, all visible
 * (users who aren't invisible (user mode +i) and who don't have a
 * common channel with the requesting client) are listed.  The same
 * result can be achieved by using a <mask> of "0" or any wildcard which
 * will end up matching every visible user.
 *
 * The <mask> passed to WHO is matched against users' host, server, real
 * name and nickname if the channel <mask> cannot be found.
 *
 * If the "o" parameter is passed only operators are returned according
 * to the <mask> supplied.
 * 
 * Numeric Replies:
 * 
 * ERR_NOSUCHSERVER
 * RPL_WHOREPLY
 * RPL_ENDOFWHO
 *
 * Examples:
 *
 * WHO *.fi                        ; Command to list all users who match
 *                                 against "*.fi".
 * 
 * WHO jto* o                      ; Command to list all users with a
 *                                 match against "jto*" if they are an
 *                                 operator.
 */
void	Server::who(User &user, std::string &str)
{
	std::vector<std::string>	args(split(str, " "));

	switch (args.size())
	{
		case 0 :
			who_all(*this, user);
			break;
		case 1 :
			who_chan(*this, user, args[0]);
			break;
		case 2 :
			if (args[1] != "o")
				break;
			who_chop(*this, user, args[0]);
			break;
	}
	return ;
}
