/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tde-vlee <tde-vlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 13:24:17 by mdegraeu          #+#    #+#             */
/*   Updated: 2023/05/04 00:54:31 by tde-vlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../server/Server.hpp"

/**
 * PASS
 *
 * Command: PASS
 * Parameters: <password>
 * 
 * The password supplied must match the one defined in the server configuration. It is possible to send multiple PASS commands before registering but only the last one sent is used for verification and it may not be changed once the client has been registered.
 *
 * If the password supplied does not match the password expected by the server, then the server SHOULD send ERR_PASSWDMISMATCH (464) and MAY then close the connection with ERROR. Servers MUST send at least one of these two messages.
 *
 * Servers may also consider requiring SASL authentication upon connection as an alternative to this, when more information or an alternate form of identity verification is desired.
 *
 * Numeric replies:
 *
 * ERR_NEEDMOREPARAMS (461)
 * ERR_ALREADYREGISTERED (462)
 * ERR_PASSWDMISMATCH (464)
 * 
 * Command Example:
 * PASS secretpasswordhere
 * 
 */
void	Server::pass(User &user, std::string &args)
{
	if (args.empty())
	{
		this->send_msg(user.getSocket_fd(), ERR_NEEDMOREPARAMS(this->_servername, user.getNickname(), "PASS"));
		return ;
	}
	if (this->_user_map.find(user.getSocket_fd()) != this->_user_map.end())
	{
		this->send_msg(user.getSocket_fd(), ERR_ALREADYREGISTERED(this->_servername, "PASS"));
		return ;
	}
	if (this->_serverpass.compare(args) != 0)
	{
		this->send_msg(user.getSocket_fd(), ERR_PASSWDMISMATCH(this->_servername));
		return ;
	}
	user.setPassStatus(1);
}
