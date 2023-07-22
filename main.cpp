# include "headers/Init.hpp"

int	main(int ac, char **av)
{
	std::ifstream 		file;
	std::stringstream	file_data;
	std::string			file_data_string;
	Init				websrv;
	webserver			web_s;

	if (ac != 2)
	{
		std::cout << "Wrong number " << std::endl;
		return 0;
	}
	file.open(av[1]);

	if (!file.is_open())
	{
		std::cout << "File not found " << std::endl;
		return 0;
	}

	file_data << file.rdbuf();

	if (file_data.eof())
	{
		std::cout << "File is empty " << std::endl;
		return 0;
	}
	file_data_string = file_data.str();

	web_s.parse_server_block(file_data_string);
	// web_s.print_config_file();

	if (!web_s.parse_state)
		return 0;
	for(size_t i = 0; i < web_s.server_blocks.size(); ++i){
		Server s(web_s.server_blocks[i]);
		websrv.add_server(s);
	}
	websrv.start_listening();
}
