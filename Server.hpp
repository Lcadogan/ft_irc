#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <sys/types.h>
# include <sys/socket.h>
# include <fcntl.h>
# include <unistd.h>
# include <arpa/inet.h>
# include <netinet/in.h>
# include <netdb.h>
# include <sys/_types/_size_t.h>
# include <sys/socket.h>
# include <sstream>
# include <vector>
# include <string>

# include "Client.hpp"

class Command;




# define Xv(err,res,str)	(x_void(err,res,str,__FILE__,__LINE__))
# define X(err,res,str)		(x_int(err,res,str,__FILE__,__LINE__))
# define BACKLOG 128

struct Network
{
    std::string			host;
	std::string			port;
	std::string 		password;
    Network (std::string, std::string, std::string);
};

class Server {
	private:
		const Network			network;
		std::vector<Client *>	clients;
		std::vector<Channel *>	channels;

		void 						response(void);
		void						initialization_fd();
		void 						doSelect();
		void 						check_socket();
		static fd_set   read_fd;
		static fd_set   write_fd;
		int				maxfd;
		Command* 					parse(Client& client);
		Command* 					newCommand(Client& client, std::string str);
	public:
		void						deleteClient(Client& client);
		void 						startServ();
		const Network&				getNetwork() const;
		const std::string&			getPassword();
		std::vector<Client*>&		getClients(void);
		std::vector<Channel*>&		getChannels(void);

		Channel*					find_channel(const std::string channal_name);
		Client*						find_client(const std::string client_name);

		
		Server(std::string host, std::string port, std::string password);
};



std::vector<std::string> split(const std::string& s, char delimiter);
int		x_int(int err, int res, const char *str, const char *file, int line);
void	*x_void(void *err, void *res, const char *str, const char *file, int line);

#endif
