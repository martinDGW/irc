/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Basics.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tde-vlee <tde-vlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/05 10:33:03 by mdegraeu          #+#    #+#             */
/*   Updated: 2023/05/30 18:16:19 by tde-vlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BASICS_H
# define BASICS_H

# include "./Replies.hpp"

# include <string>
# include <iostream>
# include <fstream>
# include <sstream>
# include <istream>
# include <ostream>
# include <streambuf>
# include <ctime>

# include <map>
# include <vector>

# include <unistd.h>
# include <string.h>
# include <sys/socket.h>
# include <sys/types.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <poll.h>

std::string	dateStr();

# define DEBUG(message) (std::cerr << __FILE__ << ":" << __LINE__ << " : " << __func__ << " : " << message << std::endl)

std::string	strtolower(std::string str);

#endif
