
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

    if (!client.rqst.found_next_hexa)
        client.rqst.next_hex_saver = req;

	client.rqst.is_reading_new_chunk_part = true;
	while(body_chunk_size)
	{
		// Search for first hexa
		if (!client.rqst.found_next_hexa)
		{
            std::string chunk_part = client.rqst.next_hex_saver;
			if (!strstr(chunk_part.c_str(), "\r\n"))
				return false;
			std::size_t find = client.rqst.next_hex_saver.find("\r\n");
			client.rqst.next_hex_saver = client.rqst.next_hex_saver.substr(0, find);
            client.rqst.found_next_hexa = true;
			client.rqst.chunk_size = covert_hex_to_decimal(client.rqst.next_hex_saver);
			if (client.rqst.chunk_size == 0)
			{
				client.rqst.found_next_hexa = false;
				return true ;
			}
            client.rqst.chunk_saver = chunk_part.substr(find + 2, chunk_part.size());
            client.rqst.is_reading_new_chunk_part = false;
			body_chunk_size -= client.rqst.next_hex_saver.size();
			body_chunk_size -= 2;
		}
		// Proceed to find other hexas to convert
		if (client.rqst.found_next_hexa)
		{
			std::fstream	body_file;
			body_file.open("test", 
							std::ios_base::binary|std::ios_base::out|
							std::ios_base::app);

			if (client.rqst.is_reading_new_chunk_part)
			{
				client.rqst.chunk_saver += req;
                client.rqst.is_reading_new_chunk_part = false;
            }
			if (client.rqst.chunk_saver.size() >= client.rqst.chunk_size)
			{
				std::string chunk = client.rqst.chunk_saver;
				client.rqst.next_hex_saver = chunk.substr(client.rqst.chunk_size, client.rqst.chunk_saver.size());
                client.rqst.chunk_saver = chunk.substr(0, client.rqst.chunk_size);
                std::cout << client.rqst.chunk_saver << "\n\n";
                body_file << client.rqst.chunk_saver;
                body_chunk_size -= client.rqst.chunk_saver.size();
			}
            else
                body_chunk_size -= client.rqst.chunk_saver.size();
			
            client.rqst.found_next_hexa = !(client.rqst.chunk_saver.size() >= client.rqst.chunk_size);

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
	client.rqst.found_next_hexa = false;

	std::string	chunked_body =  
	"C\r\n"
	"Some data..."
	"11\r\n"
	"Sope more data..."
    "A\r\n"
    "ponponponp"
	"0\r\n";

    std::string o = chunked_body.substr(0, 16);
    test(chunked_body, client);
}
