#include "headers/Server.hpp"

int main(){
	Server s("8080", 0);
	int		ready_client;

	while (1){
		fd_set read;
		read = s.main;
		select(s.max_socket +1, &read, 0,0,0);
		ready_client = s.wait_clients(read);
		if (ready_client == s._socket)
			s.add_client(ready_client);
		else
			s.get_rqst(ready_client);
	}
	return 0;
}