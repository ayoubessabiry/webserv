#ifndef CLIENT_HPP_GARD
# define CLIENT_HPP_GARD
# define MAX_REQUEST_SIZE 65535
# include "webserv.hpp"
# include "parse_request.hpp"
# include "parse_config.hpp"
# include "GetMethod.hpp"
# include "PostMethod.hpp"
# include "DeleteMethod.hpp"

class request
{
	public:
		std::string								method;
		std::string								uri;
		std::string								body;
		std::string								status;
		std::string 							file_name;
		std::string								chunk_part;
		std::string								chunk_saver;
		std::string								next_hex_saver;
		std::map<std::string, std::string>		headers;
		// std::fstream							body_file;
		bool									is_reading_chunked;

		int										chunk_size;

		bool			is_reading_new_chunk_part;
		bool			found_next_hexa;

		std::string	random_file_name_generate();
		bool	parse_request_data(std::string&, bool&);
		bool	body_chunked_encoding(std::string&);
		void	print_request();
};

bool	valid_method(std::string body);

class Client  
{
public :

	// Responses
	GetMethod			get;
	PostMethod			post;
	DeleteMethod		delete_method;

	socklen_t			addr_size;
	sockaddr_storage	client_add;
	std::string			request_collector;
	bool				is_reading_body;
	bool				body_file_opened;
	request				rqst;
	server_block		configuration;
	location_block		desired_location;
	size_t 				bytes_sent;
	size_t				fileSize;
	ssize_t				recv_byte;
	ssize_t				header_byte;
	ssize_t				body_byte;
	bool				header;
	char				buffer[MAX_REQUEST_SIZE];
	int					socket;
	location_block		match_location();
};

bool	send_request(Client& client, std::string& buff);

#endif
