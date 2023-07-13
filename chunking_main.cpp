
# include "headers/Init.hpp"


int	covert_hex_to_decimal(std::string hex_number)
{
	int					num;
	std::stringstream	ss;

	ss << std::hex << hex_number;
	ss >> num;

	return num;
}

bool test(std::string &req, Client client)
{
	int	body_chunk_size = req.size();
	std::cout << body_chunk_size << std::endl;

	client.rqst.is_reading_new_chunk_part = true;
	while(body_chunk_size)
	{
		// Search for first hexa
		if (!client.rqst.found_next_hexa)
		{
			if (!strstr(req.c_str(), "\r\n"))
				return false;
			std::size_t find = req.find("\r\n");
			client.rqst.next_hex_saver += req.substr(0, find);
			client.rqst.found_next_hexa = true;
			client.rqst.chunk_size = covert_hex_to_decimal(client.rqst.next_hex_saver);
			client.rqst.chunk_saver = req.substr(find + 2, req.size());
			body_chunk_size -= client.rqst.next_hex_saver.size();
			body_chunk_size -= 2;
		}
		// Proceed to find other hexas to convert
		if (client.rqst.found_next_hexa)
		{
			if (client.rqst.chunk_size == 0)
			{
				client.rqst.found_next_hexa = false;
				return true ;
			}
			if (client.rqst.is_reading_new_chunk_part)
			{
				client.rqst.chunk_saver += req;
				client.rqst.is_reading_new_chunk_part = false;
			}
			if (client.rqst.chunk_saver.size() > client.rqst.chunk_size)
			{
				std::string chunk = client.rqst.chunk_saver;
				client.rqst.next_hex_saver = chunk.substr(client.rqst.chunk_size, client.rqst.chunk_saver.size());
				client.rqst.chunk_saver = chunk.substr(0, client.rqst.chunk_size);
				body_chunk_size -= client.rqst.chunk_saver.size();
			}
			client.rqst.found_next_hexa = !(client.rqst.chunk_saver.size() >= client.rqst.chunk_size);

			std::fstream	body_file;
			body_file.open("test", 
							std::ios_base::binary|std::ios_base::out|
							std::ios_base::app);
			body_file << client.rqst.chunk_saver;
		}
		if (body_chunk_size <= 0)
			body_chunk_size = 0;
	}

	return false;
}

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

	// for(int i = 0; i < web_s.server_blocks.size(); ++i){
	// 	Server s(web_s.server_blocks[i]);//web_s.server_blocks[i].port.c_str(), web_s.server_blocks[i].host.c_str(), web_s.server_blocks[i].server_name);
	// 	websrv.add_server(s);
	// }
	// websrv.start_listening();

	Client client;

	
	client.rqst.next_hex_saver = "";

	std::string	chunked_body =  
	"C\r\n"
	"Some data..."
	"11\r\n"
	"Some more data..."
	"0\r\n";

	std::cout << test(chunked_body, client);
}



