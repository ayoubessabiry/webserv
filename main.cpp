# include "headers/parse_config.hpp"
# include "headers/Init.hpp"

int	main(int ac, char **av)
{
	std::ifstream 		file;
	std::stringstream	file_data;
	std::string			file_data_string;
	webserver 			web_s;
	Init				websrv;

	file.open(av[1]);

	file_data << file.rdbuf();

	file_data_string = file_data.str();

	web_s.parse_server_block(file_data_string);
	// web_s.print_config_file();
	Client client;

	client.configuration = web_s.server_blocks[0];
	client.rqst.uri = "/path/ind";

	std::cout << client.match_location().auto_index << std::endl;
	// std::cout << client.match_location().methods[0] << std::endl;
	// std::cout << client.match_location().root << std::endl;

	// for(int i = 0; i < web_s.server_blocks.size(); ++i){
	// 	Server s(web_s.server_blocks[i].port.c_str(), web_s.server_blocks[i].host.c_str(), web_s.server_blocks[i].server_name);
	// 	websrv.add_server(s);
	// }
	// websrv.start_listening();
	
}
