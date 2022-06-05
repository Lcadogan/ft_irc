#include "Invite.hpp"
#include "../Server.hpp"
#include "../Client.hpp"
#include "../Channel.hpp"

Invite::Invite(Server& server,Client& client, std::string str) : Command(server, client)
{
    setCommand(str);
}

void					Invite::print_message(Client* other_client, Channel* channel)
{
    other_client->socket.buf_write +=   ":" + client.getNick() +
                                        "!" + client.getUsername() +
                                        "@" + client.getHostname() +
                                        " " + command +
                                        " " + other_client->getNick() +
                                        " " + channel->getName() + "\r\n";
}

void	                Invite::execute()
{
     if (args.size() < 2)
        return (responce(ERR_NEEDMOREPARAMS, &client, nullptr));
    if (args.size() > 2)
        return ;
    
    Client * other_client = server.find_client(args[0]);
    Channel * channel = server.find_channel(args[1]);
    if (!channel)
        return (arg = args[1], responce(ERR_NOSUCHCHANNEL, &client, nullptr));
    if (!other_client)
        return (arg = args[0], responce(ERR_NOSUCHNICK, &client, nullptr));
    if (channel->this_channel(other_client))
        return (arg = args[0], responce(ERR_USERONCHANNEL, &client, channel));
    if (!channel->this_channel(&client))
        return (responce(ERR_NOTONCHANNEL, &client, channel));
    if (!channel->is_operator(&client))
        return (responce(ERR_CHANOPRIVSNEEDED, &client, channel));
    channel->addInvit(other_client);
    print_message(other_client, channel);
}

Invite::~Invite() {}