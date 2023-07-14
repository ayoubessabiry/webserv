
# include "headers/Init.hpp"

int	main(int ac, char **av)
{
	std::ifstream 		file;
	std::stringstream	file_data;
	std::string			file_data_string;
	Init				websrv;
	webserver			web_s;

	file.open(av[1]);

	file_data << file.rdbuf();

	file_data_string = file_data.str();

	web_s.parse_server_block(file_data_string);
	// web_s.print_config_file();

	if (!web_s.parse_state)
		return 0;
	for(int i = 0; i < web_s.server_blocks.size(); ++i){
		Server s(web_s.server_blocks[i]);//web_s.server_blocks[i].port.c_str(), web_s.server_blocks[i].host.c_str(), web_s.server_blocks[i].server_name);
		websrv.add_server(s);
	}
	websrv.start_listening();

	// Client client;

	// std::string	chunked_body =  
	// "C\r\n"
	// "Some data..."
	// "11\r\n"
	// "Some more data..."
	// "0\r\n";

	// int tmp_start = 0;

	// for (int i = 0 ; ; i += MAX_REQUEST_SIZE)
	// {
	// 	if (i >= chunked_body.size())
	// 	{
	// 		i = chunked_body.size();
	// 	}
	// 	client.buff = (char*)chunked_body.substr(tmp_start, i).c_str();
	// 	send_request(client);
	// 	tmp_start = i;
	// }
}



