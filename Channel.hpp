#ifndef CHANNEL_HPP
# define CHANNEL_HPP

class Client;
# include <set>
# include <string>
# include <iostream>
using namespace std;

class Channel 
{
private:
	string 			topic;
public:
	size_t			limit;
	bool			flag_t;
	bool			flag_o;
	bool			flag_i;
	bool			flag_n;
	bool			flag_l;
	string 			name;
	string		password;
	set<Client *>	clients;
	set<Client *>	operators;
	set<Client *>	invited;

	void			addInvit(Client *client);
	void			addClient(Client *client);
	bool 			is_Invit(Client *client);
	bool			is_operator(Client *client);
	bool			this_channel(Client *client);
	void			kick_client(Client *client);
	void			set_Topic(const string& topic);
	void			set_Oper(Client *client);
	void			del_Oper(Client *client);
	void			set_Limit(size_t lim);
	const string&	getName() const;
	const string&	getTopic() const;
	string         	getClientNames(void) const;
	Channel();
	Channel(string);
	Channel(const Channel& other);
	Channel(string name, Client *client, string password);
	~Channel();

	void	part_del_all_channels(Client *client);
	void		write_mess_all(string message);
	void	name_all_clients_channel(Client *client);
};



#endif
