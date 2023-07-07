# include "headers/parse_config.hpp"

int	main(int ac, char **av)
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
