#include "parse_config.hpp"

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
			if (config_tokens[i] == "server" && config_tokens[i + 1] == "{")
			{
				block_state = SERVER;
				continue ;
			}
			else
			{
				std::cout << "ERROR\n";
				return ;
			}
		}
		if (block_state == SERVER)
		{
			if (config_tokens[i] == "listen")
			{
				i++;
				while (config_tokens[i] != ";")
				{
					server.ports.push_back(config_tokens[i]);
					i++;
				}
			}
			if (config_tokens[i] == "host")
			{
				i++;
				std::string	host = "";
				while (config_tokens[i] != ";")
					host += config_tokens[i++];
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
					std::string	method = "";
					for (size_t j = i; config_tokens[j] != "," &&
									   config_tokens[j] != ";" ; j++)
					{
						method += config_tokens[j];
						i = j;
					}
					if (method != "")
						server.methods.push_back(method);
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
				while (config_tokens[i] != ";")
				{
					root += config_tokens[i];
					i++;
				}
				server.root = root;
			}
			if (config_tokens[i] == "location")
			{
				block_state = LOCATION;
				i++;
				while (config_tokens[i] != "location")
					i++;
				server_blocks.push_back(server);
			}
			if (block_state == LOCATION)
			{
				while (config_tokens[i] != "location")
				{
					
					i++;
				}
			}
		}
	}
}

void	webserver::print_config_file()
{
	for (size_t i = 0; i < server_blocks.size(); i++)
	{
		std::cout << "---------Server " << i << "---------\n";
		if (!server_blocks[i].ports.empty())
		{
			std::cout << "\t----Ports-----\n"; 
			for	(size_t j = 0; j < server_blocks[i].ports.size(); j++)
			{
				std::cout << "\t\t Port: " << i << " " << server_blocks[i].ports[j] << "\n";
			}
		}
		if (server_blocks[i].host != "")
			std::cout << "\t Host: " << server_blocks[i].host << "\n";
		if (!server_blocks[i].server_name.empty())
			std::cout << "\t Server name: " << server_blocks[i].server_name << "\n";
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
		if (!server_blocks[i].root.empty())
		{
			std::cout << "\tRoot: " << server_blocks[i].root << "\n";
		}
		if (!server_blocks[i].locations.empty())
		{
			std::cout << "\t-----Locations------\n";
			for (size_t j = 0; j < server_blocks[i].locations.size(); j++)
			{
				if (!server_blocks[i].locations[j].indexes.empty())
				{
					for (size_t k = 0; k < server_blocks[i].locations[j].indexes.size(); k++)
					{
						std::cout << "\t--------Indexes-------\n";
						std::cout << "\t\t" << server_blocks[i].locations[j].indexes[k] << "\n";
					}
				}
			}			
		}
	}
	
}

int main(int ac, char **av)
{
	std::ifstream 		file;
	std::stringstream	file_data;
	std::string			file_data_string;

	webserver 	web_s;

	file.open(av[1]);

	file_data << file.rdbuf();

	file_data_string = file_data.str();

	web_s.parse_server_block(file_data_string);
	web_s.print_config_file();
}
