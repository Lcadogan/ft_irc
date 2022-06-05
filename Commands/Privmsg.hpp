#ifndef PRIVMSG_HPP
# define PRIVMSG_HPP

# include <iostream>
# include "Command.hpp"

class Privmsg : public Command
{
	private:

	public:
		Privmsg(Server& server,Client& client, std::string str);
        virtual void	                execute();
		virtual void					print_message(Client* other_client, Channel* channel);
		virtual                         ~Privmsg();
};

#endif 