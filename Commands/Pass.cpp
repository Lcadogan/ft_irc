#include "Pass.hpp"

Pass::Pass(Server& server,Client& client, std::string str) : Command(server, client)
{
    setCommand(str);
}

void	                Pass::execute()
{
    if (args.size() != 1)
        return (responce(ERR_NEEDMOREPARAMS, &client, nullptr));
    if (client.getPassword())
        return (responce(ERR_ALREADYREGISTRED, &client, nullptr));
    if (args[0] == server.getPassword())
    {
        client.setPassword(true);
        client.socket.buf_write += "Please, enter \'nick <your nickname>\' command.\n";
        client.socket.buf_write += ">>";
    }
    else
        client.socket.buf_write = "Password is incorrect! Try again!\n";
}

Pass::~Pass() {}