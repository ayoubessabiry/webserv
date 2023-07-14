
# include "headers/Init.hpp"

int	covert_hex_to_decimal(std::string hex_number)
{
	int					num;
	std::stringstream	ss;

	ss << std::hex << hex_number;
	ss >> num;

	return num;
}

bool test(std::string &req, Client &client)
{
	int	body_chunk_size = req.size();

	client.rqst.is_reading_new_chunk_part = true;
	while(body_chunk_size)
	{
		if (!client.rqst.found_next_hexa)
		{
			if (client.rqst.is_reading_new_chunk_part)
				client.rqst.chunk_part += req;
			if (!strstr(client.rqst.chunk_part.c_str(), "\r\n"))
				return false;
			std::size_t find = client.rqst.chunk_part.find("\r\n");
			client.rqst.next_hex_saver = client.rqst.chunk_part.substr(0, find);

            client.rqst.found_next_hexa = true;
			client.rqst.chunk_size = covert_hex_to_decimal(client.rqst.chunk_part);
			if (client.rqst.chunk_size == 0)
			{
				client.rqst.found_next_hexa = false;
				return true ;
			}
            client.rqst.chunk_saver = client.rqst.chunk_part.substr(find + 2, client.rqst.chunk_part.size());
            client.rqst.is_reading_new_chunk_part = false;
			body_chunk_size -= client.rqst.next_hex_saver.size();
			body_chunk_size -= 2;
		}
		if (client.rqst.found_next_hexa)
		{
			std::fstream	body_file;
			body_file.open("test", 
							std::ios_base::binary|std::ios_base::out|
							std::ios_base::app);

			if (client.rqst.is_reading_new_chunk_part)
				client.rqst.chunk_saver += req;
			if (client.rqst.chunk_saver.size() >= client.rqst.chunk_size)
			{
				std::string chunk = client.rqst.chunk_saver;
				client.rqst.chunk_part = chunk.substr(client.rqst.chunk_size, client.rqst.chunk_saver.size());
                client.rqst.chunk_saver = chunk.substr(0, client.rqst.chunk_size);
                body_file << client.rqst.chunk_saver;
                body_chunk_size -= client.rqst.chunk_saver.size();
				if (client.rqst.is_reading_new_chunk_part)
					body_chunk_size += req.size();
            	client.rqst.found_next_hexa = false;
			}
            else
                body_chunk_size -= client.rqst.chunk_saver.size();

			client.rqst.is_reading_new_chunk_part = false;
		
            if (body_chunk_size <= 0)
                body_chunk_size = 0;
		}
	}

	return false;
}

int	main(int ac, char **av)
{
	Client client;

	client.rqst.next_hex_saver = "";
	client.rqst.chunk_saver = "";
	client.rqst.found_next_hexa = false;

	std::string	chunked_body =  
	"C\r\n"
	"Some data..."
	"C\r\n"
	"Some data..."
	"C\r\n"
	"Some data..."
	"C\r\n"
	"Some data..."
	"C\r\n"
	"Some data..."
	"C\r\n"
	"Some data..."
	"C\r\n"
	"Some data..."
	"C\r\n"
	"Some data..."
	"C\r\n"
	"Some data..."
	"C\r\n"
	"Some data..."
	"C\r\n"
	"Some data..."
	"C\r\n"
	"Some data..."
	"C\r\n"
	"Some data..."
	"C\r\n"
	"Some data..."
	"C\r\n"
	"Some data..."
	"0\r\n";

    std::string o = "C\r\n"
	"So";
    std::cout << test(chunked_body, client) << std::endl;
}
