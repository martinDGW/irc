/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cben-bar <cben-bar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 11:44:44 by mdegraeu          #+#    #+#             */
/*   Updated: 2023/05/31 18:36:21 by cben-bar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <utility>
#include <deque>
#include "../server/Server.hpp"

typedef std::deque< std::pair<std::string, std::string> > chanque;

static short	check_args(const std::string &args)
{
	if (args.empty())
		return (1);
	return (0);
}

static std::vector<std::string> split(std::string str, const std::string separator)
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

static chanque	parse_channel(const std::string &args)
{
	size_t						pass_idx;
	std::vector<std::string>	chan_vec;
	std::vector<std::string>	pass_vec;
	chanque						chan_queue;

	pass_idx = args.find_first_of(' ');
	if (pass_idx == args.npos)
		chan_vec = split(args, " ");
	else
	{
		chan_vec = split(args.substr(0, pass_idx), ",");
		pass_vec = split(args.substr(pass_idx + 1, args.size()), ",");
	}
	while (!chan_vec.empty())
	{
		if (pass_vec.empty())
			chan_queue.push_front(std::pair<std::string, std::string>(chan_vec.back(), ""));
		else
		{
			chan_queue.push_front(std::pair<std::string, std::string>(chan_vec.back(), pass_vec.back()));
			pass_vec.pop_back();
		}
		chan_vec.pop_back();
	}
	return (chan_queue);
}

static bool	checkChanMask(const std::string &name)
{
	static const std::string	special = "#&+!";

	if (name[0] != '#')
		return (1);
	for (size_t idx = 0; idx < name.size(); idx ++)
	{
		if ( !std::isalnum(name[idx]) && special.find(name[idx]) == name.npos)
			return (1);
	}
	return (0);
}

static void	joinProcess(User &user, Channel &channel, const std::string &servername) {
	channel.addUser(user);
	channel.send_msg(RPL_JOIN(user.getRealname(), channel.getName()));
	if (channel.getTopic().empty())
		channel.send_to_one(user.getSocket_fd(), RPL_NOTOPIC(servername, user.getNickname(), channel.getName()));
	else
		channel.send_to_one(user.getSocket_fd(), RPL_TOPIC(servername, user.getNickname(), channel.getName(), channel.getTopic()));
	channel.send_to_one(user.getSocket_fd(), RPL_NAMEREPLY(user.getRealname(), user.getNickname(), "=", channel.getName(), channel.getString_list()));
	channel.send_to_one(user.getSocket_fd(), RPL_ENDOFNAMES(user.getRealname(), user.getNickname(), channel.getName()));
}

/**
 * The JOIN command is used by client to start listening a specific
 * channel. Whether or not a client is allowed to join a channel is
 * checked only by the local server the client is connected to; all
 * other servers automatically add the user to the channel when the
 * command is received from other servers.
 *
 * Optionally, the user status (channel modes 'O', 'o', and 'v') on the
 * channel may be appended to the channel name using a control G (^G or
 * ASCII 7) as separator.  Such data MUST be ignored if the message
 * wasn't received from a server.  This format MUST NOT be sent to
 * clients, it can only be used between servers and SHOULD be avoided.
 *
 * The JOIN command MUST be broadcast to all servers so that each server
 * knows where to find the users who are on the channel.  This allows
 * optimal delivery of PRIVMSG and NOTICE messages to the channel.
 *
 * Numeric Replies:
 *
 * ERR_NEEDMOREPARAMS	v
 * ERR_BANNEDFROMCHAN	v
 * ERR_INVITEONLYCHAN	v
 * ERR_BADCHANNELKEY	v
 * ERR_CHANNELISFULL	v
 * ERR_BADCHANMASK		v
 * ERR_NOSUCHCHANNEL	v
 * ERR_TOOMANYCHANNELS	v
 * ERR_TOOMANYTARGETS	~
 * ERR_UNAVAILRESOURCE	~
 * RPL_TOPIC			v
 *
 * Examples:
 * :WiZ JOIN #Twilight_zone        ; JOIN message from WiZ
 */
void	Server::join(User &user, std::string &args)
{
	chanque	chan_queue;

	if (check_args(args))
	{
		send_msg(user.getSocket_fd(), ERR_NEEDMOREPARAMS(user.getUserName(), user.getNickname(), "JOIN"));
		return ;
	}
	chan_queue = parse_channel(args);
	while (!chan_queue.empty())
	{
		std::string name = strtolower(chan_queue.front().first);
		std::string pass = chan_queue.front().second;
		chan_queue.pop_front();
		if (checkChanMask(name))
		{
			this->send_msg(user.getSocket_fd(), ERR_BADCHANMASK(this->_servername, name));
			continue;
		}
		if (!this->chanExist(name))
		{
			if (user.chan_joined >= USERCHANNELSLIMIT)
			{
				this->send_msg(user.getSocket_fd(), ERR_TOOMANYCHANNELS(user.getRealname(), user.getNickname(), name));
				return ;
			}
			Channel	*newChan = this->addChannel(name);
			joinProcess(user, *newChan, this->getServerName());
			continue;
		}
		Channel &chan = *this->getChannel(name);
		if (chan.isMode('i'))
		{
			if (!chan.isInvited(user.getNickname()))
				this->send_msg(user.getSocket_fd(), ERR_INVITEONLYCHAN(user.getRealname(), user.getNickname(), chan.getName()));
			else {
				chan.removeInviteList(user.getNickname());
				joinProcess(user, chan, this->getServerName());
			}
			continue;
		}
		if (chan.isBan(user.getNickname()))
		{
			this->send_msg(user.getSocket_fd(), ERR_BANNEDFROMCHAN(user.getUserName(), user.getNickname(), chan.getName()));
			continue;
		}
		if (chan.isMode('k') && chan.getKey() != pass)
		{
			this->send_msg(user.getSocket_fd(), ERR_BADCHANNELKEY(user.getUserName(), user.getNickname(), chan.getName()));
			continue;
		}
		if (chan.getMaxUsers() > 0 && chan.getUserCount() == chan.getMaxUsers())
		{
			this->send_msg(user.getSocket_fd(), ERR_CHANNELISFULL(user.getUserName(), user.getNickname(), chan.getName()));
			continue;
		}
		if (user.chan_joined >= USERCHANNELSLIMIT)
		{
			this->send_msg(user.getSocket_fd(), ERR_TOOMANYCHANNELS(user.getRealname(), user.getNickname(), chan.getName()));
			return ;
		}
		joinProcess(user, chan, this->getServerName());
	}
	return ;
}
