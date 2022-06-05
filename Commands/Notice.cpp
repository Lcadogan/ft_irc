#include "Notice.hpp"
#include "../Server.hpp"
#include "../Client.hpp"
#include "../Channel.hpp"

Notice::Notice(Server& server,Client& client, std::string str) : Command(server, client)
{
    setCommand(str);
}
void					Notice::print_message(Client* other_client, Channel* channel)
{

    other_client->socket.buf_write +=   ":" + client.getNick() +
                                        "!" + client.getUsername() +
                                        "@" + client.getHostname() +
                                        " " + command +
                                        " " + (!channel ? other_client->getNick() : channel->getName()) +
                                        " " + text + "\r\n";
}

void	                Notice::execute()
{
    if (text.empty() && text.empty())
            return (responce(ERR_NOTEXTTOSEND, &client, nullptr));
    if (args.size() < 1)
        return (responce(ERR_NORECIPIENT, &client, nullptr));
    std::vector<std::string> args1 = split(args[0], ',');
    for(std::vector<std::string>::const_iterator cit = args1.begin(); cit != args1.end(); ++cit)
    {
        Channel * channel = server.find_channel(*cit);
        Client * other_client = server.find_client(*cit);
        if (other_client != nullptr)
            print_message(other_client, nullptr);
        else if (channel != nullptr)
        {
            if (channel->flag_n && !channel->this_channel(&client))
                continue ;
            for (std::set<Client *>::iterator it = channel->clients.begin(); it != channel->clients.end(); ++it)
                if (client.getNick().compare((*it)->getNick()))
                    print_message(*it, channel);
        }
        else
        {
            arg = *cit;
            responce(ERR_NOSUCHNICK, &client, nullptr);
        }    
    }
}

Notice::~Notice() {}