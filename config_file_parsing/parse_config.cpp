#include "../headers/parse_config.hpp"

bool	check_if_directive_valid(std::string directive)
{
	return  directive == "listen" ||
			directive == "host" ||
			directive == "server_name" ||
			directive == "index" || 
			directive == "auto_index" ||
			directive == "root" || 
			directive == "client_max_body_size" ||
			directive == "return" ||
			directive == "cgi_exec" ||
			directive == "cgi_path"; 
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

	if (count > 4)
	{
		return false;
	}

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
	return std::stoi(port);
}

void webserver::parse_server_block(std::string config_file_data)
{
	size_t	block_state = 0;
	size_t	bracket_state = 0;
	size_t	port_directive_numbers = 0;
	size_t	host_directive_numbers = 0;
	size_t	location_block_numbers = 0;
	bool	parse_state = true;

	server_block	server;
	location_block	location;

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
			server.root.clear();
			server.server_name.clear();
			server.auto_index.clear();
			server.ports.clear();
			server.methods.clear();
			server.errors.clear();
			server.indexes.clear();
			server.locations.clear();
			if (config_tokens[i] == "server" && config_tokens[i + 1] == "{")
			{
				block_state = SERVER;
				continue ;
			}
			else
			{
				std::cout << "ERROR\n";
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
					std::cout << "ERROR" << std::endl;
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
			if (config_tokens[i] == "listen")
			{
				i++;
				port_directive_numbers++;
				if (check_if_port_valid(config_tokens[i]))
					server.ports = (config_tokens[i++]);
				if (config_tokens[i] != ";")
				{
					std::cout << "ERROR\n";
					return ;
				}
				if (!check_if_port_valid(config_tokens[i]) && config_tokens[i] != ";")
				{
					std::cout << "ERROR\n";
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
					std::cout << "ERROR\n";
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
			if (config_tokens[i] == "method")
			{
				i++;
				while (config_tokens[i] != ";")
				{
					if (!check_if_method_valid(config_tokens[i]))
					{
						std::cout << "ERROR" << std::endl;
						parse_state = false;
						return ;
					}
					server.methods.push_back(config_tokens[i]);
					i++;
				}
				
			}
			if (config_tokens[i] == "error")
			{
				i++;
				while (config_tokens[i] != ";")
				{
					server.errors.push_back(config_tokens[i++]);
				}
			}
			if (config_tokens[i] == "root")
			{
				i++;
				std::string	root = "";
				root += config_tokens[i++];
				if (config_tokens[i] != ";")
				{
					std::cout << "ERROR" << std::endl;
					parse_state = false;
					return ;
				}
				server.root = root;
			}
			if (config_tokens[i] == "auto_index")
			{
				i++;
				std::string	auto_index = "";
				auto_index += config_tokens[i++];
				if (config_tokens[i] != ";")
				{
					std::cout << "ERROR" << std::endl;
					parse_state = false;
					return ;
				}
				server.auto_index = auto_index;
			}
			// Necessary block location
			if (config_tokens[i] == "location")
			{
				if (config_tokens[i + 2] == "{")
				{
					block_state = LOCATION;
					location_block_numbers++;
				}
				else
				{
					std::cout << "ERROR\n";
					parse_state = false;
					return ;
				}
				location.prefix = config_tokens[i + 1];
				i++;
			}
			if (config_tokens[i] == "index")
			{
				i++;
				while (config_tokens[i] != ";")
				{
					server.indexes.push_back(config_tokens[i]);
					i++;
				}	
			}
		}
		if (block_state == LOCATION)
		{
			if (config_tokens[i] == "index")
			{
				i++;
				while (config_tokens[i] != ";")
				{
					location.indexes.push_back(config_tokens[i]);
					i++;
				}	
			}
			if (config_tokens[i] == "client_max_body_size")
			{
				i++;
				location.client_max_body_size = config_tokens[i++];
				if (config_tokens[i] != ";")
				{
					std::cout << "ERROR" << std::endl;
					parse_state = false;
					return ;
				}
			}
			if (config_tokens[i] == "cgi_exec")
			{
				i++;
				location.cgi_exec = config_tokens[i++];
				if (config_tokens[i] != ";")
				{
					std::cout << "ERROR" << std::endl;
					parse_state = false;
					return ;
				}
			}
			if (config_tokens[i] == "cgi_path")
			{
				i++;
				location.cgi_path = config_tokens[i++];
				if (config_tokens[i] != ";")
				{
					std::cout << "ERROR" << std::endl;
					parse_state = false;
					return ;
				}
			}
			if (config_tokens[i] == "return")
			{
				i++;
				location.redirect.push_back(config_tokens[i++]);
				location.redirect.push_back(config_tokens[i++]);
				if (config_tokens[i] != ";")
				{
					std::cout << "ERROR" << std::endl;
					parse_state = false;
					return ;
				}
			}
			if (config_tokens[i] == "}")
			{
				block_state = SERVER;
				server.locations.push_back(location);
			}
		}
	}
}

void	webserver::print_config_file()
{
	for (size_t i = 0; i < server_blocks.size(); i++)
	{
		std::cout << "---------Server " << i << "---------\n";
		if (server_blocks[i].host != "")
			std::cout << "\t Host: " << server_blocks[i].host << "\n";
		if (!server_blocks[i].server_name.empty())
			std::cout << "\t Server name: " << server_blocks[i].server_name << "\n";
		if (!server_blocks[i].auto_index.empty())
			std::cout << "\t Auto-index: " << server_blocks[i].auto_index << "\n";
		if (!server_blocks[i].root.empty())
			std::cout << "\tRoot: " << server_blocks[i].root << "\n";
		if (!server_blocks[i].ports.empty())
		{
			std::cout << "\t----Ports-----\n"; 
			std::cout << "\t\t Port: " << " " << server_blocks[i].ports << "\n";
		}
		if (!server_blocks[i].methods.empty())
		{
			std::cout << "\t----Allowed Methods----\n";
			for (size_t j = 0; j < server_blocks[i].methods.size(); j++)
			{
				std::cout << "\t\t|" << server_blocks[i].methods[j] << "|\n";
			}
		}
		if (!server_blocks[i].errors.empty())
		{
			std::cout << "\t----Error----\n";
			for (size_t j = 0; j < server_blocks[i].errors.size(); j++)
			{
				std::cout << "\t\t|" << server_blocks[i].errors[j] << "|\n";
			}
		}
		if (!server_blocks[i].indexes.empty())
		{
			std::cout << "\t\t-----Indexes----	\n";
			for (size_t j = 0; j < server_blocks[i].indexes.size(); j++)
			{
				std::cout << "\t\t" << server_blocks[i].indexes[j] << std::endl;
			}
		}
		if (!server_blocks[i].locations.empty())
		{
			std::cout << "\t-----Locations------\n";
			for (size_t j = 0; j < server_blocks[i].locations.size(); j++)
			{
				if (!server_blocks[i].locations[j].prefix.empty())
					std::cout << "\tPrefix: " << server_blocks[i].locations[j].prefix << std::endl;
				if (!server_blocks[i].locations[j].cgi_exec.empty())
					std::cout << "\tCGI EXEC: " << server_blocks[i].locations[j].cgi_exec << std::endl;
				if (!server_blocks[i].locations[j].cgi_path.empty())
					std::cout << "\tCGI PATH: " << server_blocks[i].locations[j].cgi_path << std::endl;	
				if (!server_blocks[i].locations[j].indexes.empty())
				{
					std::cout << "\t\t-----Indexes----	\n";
					for (size_t k = 0; k < server_blocks[i].locations[j].indexes.size(); k++)
					{
						std::cout << "\t\t" << server_blocks[i].locations[j].indexes[k] << std::endl;
					}
				}
			}			
		}
		std::cout << "\n\n\n";
	}
}
