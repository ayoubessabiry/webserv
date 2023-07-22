#include "../headers/parse_config.hpp"

bool	check_if_directive_valid_in_server(std::string directive)
{
	return  directive == "listen" ||
			directive == "host" ||
			directive == "server_name" ||
			directive == "error" ||
			directive == "location";
}

bool	check_if_directive_valid_in_location(std::string directive)
{
	return  directive == "index" || 
			directive == "auto_index" ||
			directive == "root" || 
			directive == "methods" || 
			directive == "upload" || 
			directive == "client_max_body_size" ||
			directive == "return" ||
			directive == "cgi_bin";
}

bool	check_if_port_valid(std::string port)
{
	for	(size_t i = 0 ; i < port.size(); i++)
	{
		if (!std::isdigit(port[i]))
			return false ;
	}
	return true;
}

bool	check_if_host_valid(std::string ip)
{
	std::stringstream	_ip(ip);
	std::string			ip_block;

	size_t	count = 0;

	while (std::getline(_ip, ip_block, '.'))
	{
		if (!check_if_port_valid(ip_block))
			return false;
		count++;
	}

	if (count > 4 || count < 4)
		return false;

	return true;
}

bool	check_if_method_valid(std::string method)
{
	return (method == "GET" || method == "POST" || method == "DELETE");
}

int	return_state(std::string token)
{
	return	((token == "server") * SERVER +
			 (token == "location") * LOCATION);
}

int	get_port(std::vector<std::string> config_tokens)
{
	std::string	port = "";
	for (size_t i = 0; config_tokens[i] != ";"; i++)
	{
		port += config_tokens[i];
	}
	return std::atoi(port.c_str());
}

void webserver::parse_server_block(std::string config_file_data)
{
	size_t	block_state = 0;
	size_t	bracket_state = 0;
	size_t	port_directive_numbers = 0;
	size_t	host_directive_numbers = 0;
	size_t	location_block_numbers = 0;
	size_t	root_directive_numbers = 0;

	server_block	server;
	location_block	location;

	parse_state = true;

	for (size_t i = 0 ; i < config_file_data.size() ; i++)
	{
		std::string	token = "";
		for (size_t j = i ; config_file_data[j] != '\n' &&
							!std::isspace(config_file_data[j]) &&
							config_file_data[j] != '{' &&
							config_file_data[j] != '}' &&
							config_file_data[j] != ';' &&
							config_file_data[j] != ',' ; j++)
		{
			token += config_file_data[j];
			i = j;
		}
		if (config_file_data[i] == ';' || config_file_data[i] == '}'
		|| config_file_data[i] == '{' || config_file_data[i] == ',')
		{
			token = "";
			token += config_file_data[i];
			config_tokens.push_back(token);
			continue;
		}
		if (token == "")
			continue;
		config_tokens.push_back(token);
	}
	for (size_t i = 0; i < config_tokens.size(); i++)
	{
		if (block_state == 0)
		{
			server.host.clear();
			server.server_name.clear();
			server.port.clear();
			server.error_path.clear();
			server.locations.clear();
			if (config_tokens[i] == "server" && config_tokens[i + 1] == "{")
			{
				block_state = SERVER;
				continue ;
			}
			else
			{
				std::cout << "ERReOR\n";
				parse_state = false;
				return ;
			}
		}
		if (block_state == SERVER)
		{
			if (config_tokens[i] == "}")
			{
				block_state = 0;
				if (port_directive_numbers == 0 || host_directive_numbers == 0 || location_block_numbers == 0)
				{
					if (port_directive_numbers == 0)
						std::cout << "Error: Server should have a port !!" << std::endl;
					if (host_directive_numbers == 0)
						std::cout << "Error: Server should have a host !!" << std::endl;
					if (location_block_numbers == 0)
						std::cout << "Error: Server should have a location block !!" << std::endl;
					parse_state = false;
					server_blocks.clear();
					return ;
				}
				port_directive_numbers = 0;
				host_directive_numbers = 0;
				location_block_numbers = 0;
				server_blocks.push_back(server);
			}
			// Necessary directives listen and host
			if (config_tokens[i] != "{" && config_tokens[i] != "}"
				 && !check_if_directive_valid_in_server(config_tokens[i]))
			{
				std::cout << "Error: "<< config_tokens[i]
									<<" Invalid Directive in server block !!"
				 << std::endl;
				parse_state = false;
				return ;
			}
			if (config_tokens[i] == "listen")
			{
				i++;
				port_directive_numbers++;
				if (config_tokens[i].size() > 5 || 
					(config_tokens[i].size() == 5 && 
					config_tokens[i][0] == '6' &&
					config_tokens[i][1] == '5' &&
					config_tokens[i][2] == '5' &&
					config_tokens[i][3] == '3' &&
					config_tokens[i][4] > '5'
					))
				{
					std::cout << "Error: Port is invalid !!" << std::endl;
					parse_state = false;
					return ;
				}
				if (check_if_port_valid(config_tokens[i]))
					server.port = (config_tokens[i++]);
				else
				{
					std::cout << "Error: Port is invalid !!" << std::endl;
					parse_state = false;
					return ;
				}
				if (config_tokens[i] != ";")
				{
					std::cout << "ERRORtt\n";
					return ;
				}
				if (port_directive_numbers > 1)
				{
					std::cout << "Error: Only one port per server !!" << std::endl;
					parse_state = false;
					return ;
				}
				if (!check_if_port_valid(config_tokens[i]) && config_tokens[i] != ";")
				{
					std::cout << "ERRORe\n";
					parse_state = false;
					return ;
				}	
			}
			if (config_tokens[i] == "host")
			{
				i++;
				host_directive_numbers++;
				std::string	host = "";
				host += config_tokens[i++];
				if (config_tokens[i] != ";" || 
					!check_if_host_valid(host))
				{
					std::cout << "Error: Host is invalid" << std::endl;
					parse_state = false;
					return ;
				}				
				if (host_directive_numbers > 1)
				{
					std::cout << "Error: Only one host per server !!" << std::endl;
					parse_state = false;
					return ;
				}
				server.host = host;
			}
			if (config_tokens[i] == "server_name")
			{
				i++;
				std::string	server_name = "";
				while (config_tokens[i] != ";")
					server_name += config_tokens[i++];
				
				server.server_name = server_name;
			}
			if (config_tokens[i] == "error")
			{
				i++;
				if (!check_if_port_valid(config_tokens[i]))
				{
					std::cout << "Error" << std::endl;
					parse_state = false;
					return;
				}
				int	error_num = std::stoi(config_tokens[i++]);
				std::string error_path = config_tokens[i++];
				server.error_path.insert(std::make_pair<int, std::string>(error_num, error_path));
				if (config_tokens[i] != ";")
				{
					std::cout << "Error" << std::endl;
					parse_state = false;
					return;
				}
			}
			// Necessary block location
			if (config_tokens[i] == "location")
			{
				location.prefix = config_tokens[i + 1];
				if (config_tokens[i + 2] == "{")
				{
					block_state = LOCATION;
					location_block_numbers++;
					i++;
				}
				else
				{
					std::cout << "EeRROR\n";
					parse_state = false;
					return ;
				}
				i++;
			}
		}
		// LOCATION BLOCK
		if (block_state == LOCATION)
		{
			if (config_tokens[i] != "{" && config_tokens[i] != "}"
				 && !check_if_directive_valid_in_location(config_tokens[i]))
			{
				std::cout << "Error: "<< config_tokens[i]
									<<" Invalid Directive in location block !!"
				 << std::endl;
				parse_state = false;
				return ;
			}
			if (config_tokens[i] == "index")
			{
				i++;
				while (config_tokens[i] != ";")
				{
					location.indexes.push_back(config_tokens[i]);
					i++;
				}	
			}
			if (config_tokens[i] == "auto_index")
			{
				i++;
				location.auto_index = config_tokens[i++];
				if (location.auto_index != "off" && location.auto_index != "on")
				{
					std::cout << "Error: Auto index value should be on/off !!" << std::endl;
					parse_state = false;
					return ;
				}
				if (config_tokens[i] != ";")
				{
					std::cout << "EeRROR" << std::endl;
					parse_state = false;
					return ;
				}
			}
			if (config_tokens[i] == "client_max_body_size")
			{
				i++;
				location.client_max_body_size = config_tokens[i++];
				if (config_tokens[i] != ";")
				{
					std::cout << "ERROR tt" << std::endl;
					parse_state = false;
					return ;
				}
			}
			if (config_tokens[i] == "upload")
			{
				i++;
				location.upload = config_tokens[i++];
				if (config_tokens[i] != ";")
				{
					std::cout << "ERROR tt" << std::endl;
					parse_state = false;
					return ;
				}
			}
			if (config_tokens[i] == "cgi_bin")
			{
				i++;
				std::string	cgi_extension = config_tokens[i++];
				std::string cgi_path = config_tokens[i++];
				location.cgi_bin.insert(std::make_pair<std::string, std::string>(cgi_extension,
																				  cgi_path));
				if (config_tokens[i] != ";")
				{
					std::cout << "Error" << std::endl;
					parse_state = false;
					return;
				}
			}
			if (config_tokens[i] == "return")
			{
				i++;
				location.redirect.push_back(config_tokens[i++]);
				location.redirect.push_back(config_tokens[i++]);
				if (config_tokens[i] != ";")
				{
					std::cout << "EeRROR" << std::endl;
					parse_state = false;
					return ;
				}
			}
			if (config_tokens[i] == "methods")
			{
				i++;
				while (config_tokens[i] != ";")
				{
					if (!check_if_method_valid(config_tokens[i]))
					{
						std::cout << "Error: Valid methods are GET, POST, DELETE" << std::endl;
						parse_state = false;
						return ;
					}
					location.methods.push_back(config_tokens[i]);
					i++;
				}				
			}
			if (config_tokens[i] == "root")
			{
				i++;
				root_directive_numbers++;
				location.root = config_tokens[i++];
				if (config_tokens[i] != ";")
				{
					std::cout << "ERROR" << std::endl;
					parse_state = false;
					return ;
				}
			}
			if (config_tokens[i] == "}")
			{
				if (root_directive_numbers <= 0 || root_directive_numbers > 1)
				{
					std::cout << "Error: Should have only one root !!" << std::endl;
					parse_state = false;
					return ;
				}
				block_state = SERVER;
				server.locations.push_back(location);
				location.auto_index.clear();
				location.client_max_body_size.clear();
				location.indexes.clear();
				location.methods.clear();
				location.root.clear();
				location.redirect.clear();
				root_directive_numbers = 0;
			}
		}
	}
	for (int i = 0 ; i < server_blocks.size(); i++)
	{
		for (int j = i + 1 ; j < server_blocks.size(); j++)
		{
			if (server_blocks[i].port == server_blocks[j].port)
			{
				std::cout << "Error: port aleady setuped " << std::endl;
				parse_state = false;
				return ;
			}
		}
	}
}

void	webserver::print_config_file()
{
	for (size_t i = 0; i < server_blocks.size(); i++)
	{

	}
}
