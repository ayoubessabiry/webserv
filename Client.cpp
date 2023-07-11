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
    int     matching_size = 0, max_matching_size = 0;

    for (int i = 0; i < configuration.locations.size(); i++)
    {
        for (int j = 0; j < configuration.locations[i].prefix.size(); j++)
        {
            if (configuration.locations[i].prefix[j] != rqst.uri[j])
                return configuration.locations[0];
            if (configuration.locations[i].prefix[j] == '/')
                matching_size++;
        }
        if (max_matching_size > matching_size)
        {
            max_matching_size = matching_size;
            location_index = i;
        }
    }
    return configuration.locations[location_index];
}
