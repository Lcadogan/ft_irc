# include "Server.hpp"
# include "Commands/Join.hpp"
# include "Commands/Mode.hpp"
# include "Commands/Nick.hpp"
# include "Commands/Pass.hpp"
# include "Commands/Topic.hpp"
# include "Commands/Invite.hpp"
# include "Commands/Kick.hpp"
# include "Commands/Names.hpp"
# include "Commands/Privmsg.hpp"
# include "Commands/Notice.hpp"
# include "Commands/User.hpp"

fd_set  Server::read_fd;
fd_set  Server::write_fd;
bool	work = true;

std::vector<std::string> split(const std::string& s, char delimiter){
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        if (token.compare("") && token.compare(" ")) {
            tokens.push_back(token);
        }
    }
    return tokens;
}

Server::Server(std::string host, std::string port, std::string password) : 
		network(host, port, password)
{
	struct addrinfo	hints;
	struct addrinfo	*result;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

	int ret = getaddrinfo(network.host.c_str(), network.port.c_str(), &hints, &result);
	if (ret != 0)
	{
		std::cerr << "Server: getaddrinfo error" << std::endl;
		exit(EXIT_FAILURE);
	}
	clients.clear();
	Client * serv = new Client(Socket::FD_SERVER, 
	X(-1, socket(result -> ai_family, result -> ai_socktype, result -> ai_protocol), "socket"));
	int opt = 1;
	maxfd = serv->socket.socket_fd;
	setsockopt(serv->socket.socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	X(-1, bind(serv->socket.socket_fd, result->ai_addr, result->ai_addrlen), "bind");
	X(-1, listen(serv->socket.socket_fd, BACKLOG), "listen");
	clients.push_back(serv);
	freeaddrinfo(result);
}

Channel*			Server::find_channel(const std::string channal_name)
{
	for(std::vector<Channel *>::iterator chit = channels.begin(); chit != channels.end(); ++chit)
        if (channal_name == (*chit)->getName())
			return (*chit);
	return (nullptr);
}

Client*				Server::find_client(const std::string client_name)
{
	for(std::vector<Client *>::iterator cit = clients.begin(); cit != clients.end(); ++cit)
        if (client_name == (*cit)->getNick())
			return (*cit);
	return (nullptr);
}

void	Server::initialization_fd()
{
  	FD_ZERO(&read_fd);
  	FD_ZERO(&write_fd);

	for (std::vector<Client *>::const_iterator cit = clients.cbegin(); cit != clients.cend(); ++cit) 
		if ((*cit)->socket.type != Socket::FD_FREE)
		{		
	      	FD_SET((*cit)->socket.socket_fd, &read_fd);
	      	if (!(*cit)->socket.buf_write.empty())
	        	FD_SET((*cit)->socket.socket_fd, &write_fd);
		}
}

void Server::doSelect()
{
	X(-1, select(maxfd + 1, &read_fd, &write_fd, NULL, NULL), "select");
}

void	writeMessage(Client *client)
{
	client->socket.buf_write = "\n";
	client->socket.buf_write = "Welcome to IRC server.\n";
	client->socket.buf_write += "Please, enter \'pass <passoword of server>\' command.\n";
	client->socket.buf_write += "Or you can write \'quit\' command to close your session.\n";
	client->socket.buf_write += ">> ";

}

Network::Network(std::string host, std::string port, std::string password) : host(host), port(port), password(password) 
{}

void Server::check_socket()
{
    if (FD_ISSET(clients[0]->socket.socket_fd, &read_fd))
    {
        struct sockaddr_in 	addr;
	    socklen_t			addr_size;

	    X(-1, fcntl(clients[0]->socket.socket_fd, O_NONBLOCK), "fcntl");
	    addr_size = sizeof(addr);
		Client * client = new Client(Socket::FD_CLIENT, 
	    X(-1, accept(clients[0]->socket.socket_fd , reinterpret_cast<struct sockaddr*>(&addr), &addr_size), "accept"));
		clients.push_back(client);
		writeMessage(client);
		maxfd = maxfd > client->socket.socket_fd ? maxfd : client->socket.socket_fd;
    }
	for (std::vector<Client*>::iterator it = clients.begin(); it != clients.end(); ++it) 
	{
		if (FD_ISSET((*it)->socket.socket_fd, &read_fd))
		{
            int ret = (*it)->socket.read_sock();
			std::cout << "Client " << (*it)->socket.socket_fd << " : ";
			if (ret == 0)
			{
				close((*it)->socket.socket_fd);
				std::cout << "DISCONNECT " << std::endl;
				(*it)->socket.buf_read = "QUIT";
				Command * cmd = newCommand(**it, "QUIT");
				cmd->Execute_prep();
				if (it == clients.end())
					break ;
			}
			else if (ret == -1 )
				std::cout << "CONNECT" << std::endl;
		}
		if (FD_ISSET((*it)->socket.socket_fd, &write_fd))
			(*it)->socket.write_sock();
	}
}

void Server::response(void)
{
	for (std::vector<Client *>::iterator it = clients.begin(); it != clients.end(); ++it)
	{
		if ((*it)->socket.buf_read.back() != '\n')
			continue ;
		std::istringstream ss((*it)->socket.buf_read);
		while (std::getline(ss, (*it)->socket.buf_read))
		{
			std::cout << (*it)->socket.buf_read << std::endl;
			if ( (*it)->socket.buf_read.back() == '\r')
				(*it)->socket.buf_read.pop_back();
			Command		*cmd = parse(**it);
			(*it)->socket.buf_read.clear();
			if (!cmd)
				continue ;
			std::cout << *cmd << std::endl;
			cmd->Execute_prep();
			delete cmd;
			if (it == clients.end())
				return ;
		}
	
	}
}

void Server::startServ() 
{
	while (work)
	{
		initialization_fd();
      	doSelect();
      	check_socket();
		response();
	}
}

const Network&				Server::getNetwork() const
{
	return (network);
}

const std::string&	Server::getPassword()
{
	return (network.password);
}

std::vector<Client *>& Server::getClients(void)
{
	return (clients);
}

std::vector<Channel *>&	Server::getChannels(void)
{
	return (channels);
}

std::string toUpper(std::string token) {
	std::string upperString;

	for (std::string::size_type i = 0; i < token.length(); ++i)
    		upperString.push_back(std::toupper(token[i]));
	return upperString;
}

std::string checkPrefix(std::string message) {
	int		    pos1 = 0;
	int		    pos2 = 0;
	std::string prefix;

	if (message.compare(0, 1, ":") == 0) {
        pos1 = 1;
        pos2 = message.find(" ");
        if (pos2 != static_cast<int>(std::string::npos)) {
            return message.substr(pos1, pos2 - 1);
        }
	}
    return "";
}

Command* Server::newCommand(Client& client, std::string str)
{
	if (str == "QUIT")
		return (new Quit(*this, client, str));
	else if (str == "PASS")
		return (new Pass(*this, client, str));
	else if (str == "NICK")
		return (new Nick(*this, client, str));
	else if (str == "USER")
		return (new User(*this, client, str));
	else if (str == "JOIN")
		return (new Join(*this, client, str));
	else if (str == "PART")
		return (new Part(*this, client, str));
	else if (str == "INVITE")
		return (new Invite(*this, client, str));
	else if (str == "TOPIC")
		return (new Topic(*this, client, str));
	else if (str == "NAMES")
		return (new Names(*this, client, str));
	else if (str == "KICK")
		return (new Kick(*this, client, str));
	else if (str == "PRIVMSG")
		return (new Privmsg(*this, client, str));
	else if (str == "NOTICE")
		return (new Notice(*this, client, str));
	else if (str == "MODE")
		return (new Mode(*this, client, str));
	return (nullptr);


}

Command* Server::parse(Client& client)
{
	std::string prefix;
    std::string token;
    std::string buf(client.socket.buf_read);
    Command *cmd = nullptr;
    prefix = (checkPrefix(client.socket.buf_read));
    if (!prefix.empty())
        buf = buf.erase(0, prefix.length() + 1);
	std::istringstream tokenStream(buf);
	while (std::getline(tokenStream, token, ' ')) {
        if (!cmd) {
			cmd = newCommand(client, toUpper(token));
            if (!cmd)
				return (nullptr);
        } else  if (token[0] != ':'){
            cmd->setArgs(token);
        }
        else
            break ;
    }
    size_t pos = buf.find(':');
    if (pos != std::string::npos)
        cmd->setText(buf.substr(pos));
	return (cmd);
}
