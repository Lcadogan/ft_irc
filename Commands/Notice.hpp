#ifndef NOTICE_HPP
# define NOTICE_HPP

# include <iostream>
# include "Command.hpp"

class Notice : public Command
{
	private:

	public:
		Notice(Server& server,Client& client, std::string str);
        virtual void	                execute();
		virtual void					print_message(Client* other_client, Channel* channel);
		virtual                         ~Notice();
};

#endif 