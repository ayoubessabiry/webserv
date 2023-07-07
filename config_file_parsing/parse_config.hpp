/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_config.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smounir <smounir@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/12 14:28:49 by smounir           #+#    #+#             */
/*   Updated: 2023/06/12 14:28:49 by smounir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <stack>

enum parsing_state
{
	SERVER = 1,
	LOCATION
};

struct location_block
{
	std::string					prefix;
	std::string					root;
	std::string					upload;
	std::string					cgi_exec;
	std::string					cgi_path;
	std::string					client_max_body_size;
	std::vector<std::string>	redirect;
	std::vector<std::string>	indexes;
	std::vector<std::string>	methods;
};

struct server_block
{
	std::string					host;
	std::string					server_name;
	std::string					root;
	std::string					auto_index;
	std::string					access_log;
	std::string					ports;
	std::vector<std::string>	indexes;
	std::vector<std::string>	errors;
	std::vector<std::string>	methods;
	std::vector<location_block>	locations;
};

struct webserver
{
	std::vector<server_block>	server_blocks;
	std::vector<std::string>	config_tokens;
	void						parse_server_block(std::string config_file_data);
	void						print_config_file();	// For debugging
	bool						check_if_server_repeated();
};

