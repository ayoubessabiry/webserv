/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smounir <smounir@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/11 08:39:25 by smounir           #+#    #+#             */
/*   Updated: 2023/07/11 08:39:25 by smounir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/Client.hpp"

location_block Client::match_location()
{
    int     location_index = 0;
    int     matching_size = 0, max_matching_size = -1;
    bool    match_none = false;

    if (rqst.uri == "/")
        return configuration.locations[location_index];
    for (int i = 0; i < configuration.locations.size(); i++)
    {
        if (configuration.locations[i].prefix == "/")
            i++;
        if (i >= configuration.locations.size())
            break ;
        for (int j = 1; j < configuration.locations[i].prefix.size()
            && rqst.uri[j] == configuration.locations[i].prefix[j]
            && rqst.uri >= configuration.locations[i].prefix ; j++)
        {
            if (configuration.locations[i].prefix[j] == '/')
                matching_size++;
        }
        if (max_matching_size < matching_size && matching_size > 0)
        {
            max_matching_size = matching_size;
            location_index = i;
        }
        matching_size = 0;
    }
    return configuration.locations[location_index];
}

