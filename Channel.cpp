# include "Channel.hpp"
# include "Client.hpp"
# include <set>
# include <string>

void	Channel::part_del_all_channels(Client *client)
{
	
	set<Client *>::iterator it = clients.find(client);
	clients.erase(it);
}

void	Channel::write_mess_all(string message)
{
	set<Client *>::iterator it = clients.begin();
	for (; it != clients.end(); it++)
	{
		(*it)->socket.buf_write = message;
	}
}

void	Channel::name_all_clients_channel(Client *client)
{
	string arr_name[clients.size()];
	int	i = 0;
	set<Client *>::iterator it = clients.begin();
	for (; it != clients.end(); it++)
	{
		arr_name[i] = (*it)->getNick();
		i++;
	}
	
	for (int j = 0; j < i; j++)
	{
		client->socket.buf_write += (" " + arr_name[j]);
	}
	client->socket.buf_write += "\n";

}

void			Channel::addInvit(Client *client)
{
	invited.insert(client);
}

void			Channel::addClient(Client *client)
{
    clients.insert(client);
	invited.erase(client);
}

bool				Channel::is_operator(Client* client)
{
    if (operators.find(client) == operators.end())
		return false;
	return true;
}

std::string         Channel::getClientNames(void) const
{
    std::string names;
    for (std::set<Client *>::const_iterator it = clients.begin(); it != clients.end(); ++it)
    {
		if (operators.find(*it) != operators.end())
			names += '@';
		names += (*it)->getNick() + ' ';
    }
    return (names);
}

bool 			Channel::is_Invit(Client *client)
{
	std::set<Client*>::iterator it = invited.find(client);
	if (it == invited.end())
		return (false);
	return (true);
}

bool				Channel::this_channel(Client* client)
{
    if (clients.find(client) == clients.end())
		return false;
	return true;
}

void			Channel::kick_client(Client *client)
{
	set<Client *>::iterator it = clients.find(client);
	set<Client *>::iterator it1 = operators.find(client);
	if (it1 != operators.end())
		operators.erase(it1);
	clients.erase(it);
}

void			Channel::set_Oper(Client *client)
{
	operators.insert(client);
}

void			Channel::del_Oper(Client *client)
{
	set<Client *>::iterator it1 = operators.find(client);
	if (it1 != operators.end())
		operators.erase(it1);
}

void				Channel::set_Topic(const std::string& new_topic)
{
    topic = new_topic;
}

void			Channel::set_Limit(size_t lim)
{
	limit = lim;
}

const std::string& Channel::getName() const
{
    return (name);
}

const std::string&	Channel::getTopic() const
{
    return (topic);
}

Channel::Channel()
{}

Channel::Channel(const Channel& other)
{
    name = other.getName();
    clients = other.clients;
    operators = other.operators;
    topic = other.getTopic();
}

Channel::~Channel()
{}

Channel::Channel(string name, Client *FirstChop, string password): name(name), password(password)
{
	limit = -1;
	flag_t = false;
	flag_o = false;
	flag_i = false;
	flag_n = false;
	flag_l = false;
	limit = INT_MAX;
	clients.insert(FirstChop);
	operators.insert(FirstChop);
}