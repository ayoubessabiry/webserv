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
    bool    match = false;

    for (int i = 0; i < configuration.locations.size(); i++)
    {
        if (configuration.locations[i].prefix == "/")
            i++;
        for (int j = 0; i < configuration.locations.size() && j < configuration.locations[i].prefix.size() && j < rqst.uri.size(); j++)
        {
            if (configuration.locations[i].prefix[j] == '/' && 
                configuration.locations[i].prefix.size() <= rqst.uri.size())
            {
                match = true;
                matching_size++;
            }
            if (configuration.locations[i].prefix[j] != rqst.uri[j])
            {
                match = false;
                break ;
            }
        }
        if (max_matching_size < matching_size)
        {
            max_matching_size = matching_size;
            location_index = i;
        }
        matching_size = 0;
    }
    return configuration.locations[location_index];
}
