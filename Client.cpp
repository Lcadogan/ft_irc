#include "Client.hpp"

void	Client::addChannal(Channel* channel)
{
    in_channel = channel;
}

void    Client::registration(std::vector<std::string>& args, std::string realname)
{
    username = args[0];
    hostname = args[1];
    servername = args[2];
    this->realname = realname;
    reg_flag_n = true;
}

bool	Client::is_registred()
{
    return (reg_flag_n);
}

bool Client::getPassword() const
{
    return (pass_flag);
}

const std::string&	Client::getRealname() const
{
    return (realname);
}

const std::string&	Client::getServername() const
{
    return (servername);
}


const std::string& Client::getNick() const
{
    return (nick);
}

const std::string&	Client::getUsername() const
{
    return (username);
}

const std::string&	Client::getHostname() const
{
    return (hostname);
}


void    Client::setPassword(bool pass)
{
    pass_flag = pass;
}

void	Client::setNick(std::string nick)
{
    this->nick = nick;
}

Client::Client(Socket::Type type, int socket_fd) : socket(Socket(type, socket_fd))
{
    reg_flag_n = false;
    pass_flag = false;
}

Client::Client(const Client& other)
{
    nick = other.getNick();
    username = other.getUsername();
    servername = other.getServername();
    realname = other.getRealname();
    hostname = other.getHostname();
    pass_flag = other.getPassword();
    reg_flag_n = other.reg_flag_n;
    socket = other.socket;
    in_channel = other.in_channel;
}

Client::~Client() 
{}

Client *Client::getItem() {
    return this;
}