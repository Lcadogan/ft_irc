#include "User.hpp"
#include "../Server.hpp"
#include "../Client.hpp"
#include "../Channel.hpp"

User::User(Server& server,Client& client, std::string str) : Command(server, client)
{
    setCommand(str);
}

void	                User::execute()
{
    if (args.size() < 3 || text.empty())
        return (responce(ERR_NEEDMOREPARAMS, &client, nullptr));
    if (args.size() > 3)
        return (responce(RPL_NO, &client, nullptr));
    else if (client.is_registred())
        return (responce(ERR_ALREADYREGISTRED, &client, nullptr));
    client.registration(args, text);
    return (responce(RPL_WELCOME, &client, nullptr));
}

User::~User() {}

Quit::Quit(Server& server,Client& client, std::string str) : Command(server, client)
{
    setCommand(str);
}

void	                Quit::execute()
{
    Part part(server, client, "PART");
    for (std::vector<Channel *>::iterator chit = server.getChannels().begin(); chit != server.getChannels().end(); ++chit)
        part.getArgs().push_back((*chit)->getName());
    part.execute();
    close(client.socket.socket_fd);
	vector<Client *>::iterator it1 = find(server.getClients().begin(), server.getClients().end(), &client);
    delete &client;
    server.getClients().erase(it1);
}

Quit::~Quit() {}

Part::Part(Server& server,Client& client, std::string str) : Command(server, client)
{
    setCommand(str);
}

void	                Part::execute()
{
    if (args.empty())
        return (responce(ERR_NEEDMOREPARAMS, &client, nullptr));
    std::vector<std::string> args1 = split(args[0], ',');
    for(std::vector<std::string>::iterator it = args1.begin(); it != args1.end(); ++it)
    {
        Channel *channel = server.find_channel(*it);
        if (!channel)
            (arg = *it, responce(ERR_NOSUCHCHANNEL, &client, nullptr));
        else if (!channel->this_channel(&client))
            responce(ERR_NOTONCHANNEL, &client, channel);
        else
        {
            for (std::set<Client *>::iterator it = channel->clients.begin(); it != channel->clients.end(); ++it)
                print_message(*it, channel);
            channel->kick_client(&client);
        }
    }
}

Part::~Part() {}