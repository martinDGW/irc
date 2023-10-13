/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strtolower.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tde-vlee <tde-vlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 21:07:19 by tde-vlee          #+#    #+#             */
/*   Updated: 2023/05/24 18:03:52 by tde-vlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>

std::string	strtolower(std::string str)
{
	size_t	idx;

	idx = 0;
	while (idx < str.size())
	{
		str[idx] = std::tolower(str[idx]);
		idx++;
	}
	return (str);
}
