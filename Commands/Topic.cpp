#include "Topic.hpp"
#include "../Server.hpp"
#include "../Client.hpp"
#include "../Channel.hpp"

Topic::Topic(Server& server,Client& client, std::string str) : Command(server, client)
{
    setCommand(str);
}

void	                Topic::execute()
{
   if (args.empty() || args.size() > 1)
        return ;
    Channel *channel = server.find_channel(args[0]);
    if (!channel)
        return ;
    if (!channel->this_channel(&client))
        return (responce(ERR_NOTONCHANNEL, &client, channel));
    if (text.empty())
    {
        if (channel->getTopic().empty())
            return (responce(RPL_NOTOPIC, &client, channel));
        return (responce(RPL_TOPIC, &client, channel));
    }
    if (!channel->is_operator(&client) && channel->flag_t)
        return (responce(ERR_CHANOPRIVSNEEDED, &client, channel));
    channel->set_Topic(text);
    for (std::set<Client *>::iterator it = channel->clients.begin(); it != channel->clients.end(); ++it)
        responce(RPL_TOPIC,*it, channel);
}

Topic::~Topic() {}