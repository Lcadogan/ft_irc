#ifndef CLIENT_HPP
# define CLIENT_HPP
# include <netinet/in.h>
# include <string>
# include <sys/_types/_socklen_t.h>
# include <vector>
# include <unistd.h>

class Channel;

# include "Socket.hpp"
# include "Channel.hpp"

class Client
{
private:
	std::string 	nick;
	std::string 	username;
	std::string 	servername;
	std::string 	realname;
	std::string		hostname;
public:
	bool			pass_flag;
	bool			reg_flag_n;
	Socket			socket;
	Channel*		in_channel;

	void	setPassword(bool);
	void	setNick(std::string nick);
	void	addChannal(Channel *); 
	void	registration(std::vector<std::string>& args, std::string);
	bool	is_registred();


	bool				getPassword() const;
	const std::string&	getNick() const;
	const std::string&	getUsername() const;
	const std::string&	getHostname() const;
	const std::string&	getRealname() const;
	const std::string&	getServername() const;
	Client *getItem();
	
	Client(Socket::Type type, int socket_fd);
	Client(const Client& other);
	~Client();	
};


#endif
