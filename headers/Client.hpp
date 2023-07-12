#ifndef CLIENT_HPP_GARD
# define CLIENT_HPP_GARD
# define MAX_REQUEST_SIZE 2
# include "webserv.hpp"
# include "parse_request.hpp"
# include "parse_config.hpp"
# include "GetMethod.hpp"

class request
{
	public:
		std::string		method;
		std::string		uri;
		std::string		body;
		std::string		status;
		std::string 	file_name;
		std::map<std::string, std::string>		headers;

		bool			is_reading_chunked;

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

	socklen_t			addr_size;
	sockaddr_storage	client_add;
	std::string			request_collector;
	bool				is_reading_body;
	bool				body_file_opened;
	request				rqst;
	server_block		configuration;
	location_block		desired_location;
	ssize_t				recv_byte;
	ssize_t				header_byte;
	ssize_t				body_byte;
	char				buff[MAX_REQUEST_SIZE];
	int					socket;
	location_block		match_location();
};

bool	send_request(Client& client);

#endif
