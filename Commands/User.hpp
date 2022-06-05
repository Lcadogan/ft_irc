#ifndef USER_HPP
# define USER_HPP

# include <iostream>
# include "Command.hpp"

class User : public Command
{
	private:

	public:
		User(Server& server,Client& client, std::string str);
        virtual void	                execute();
		virtual                         ~User();
};

class Quit : public Command
{
	private:

	public:
		Quit(Server& server,Client& client, std::string str);
        virtual void	                execute();
		virtual                         ~Quit();
};

class Part : public Command
{
	private:

	public:
		Part(Server& server,Client& client, std::string str);
        virtual void	                execute();
		virtual                         ~Part();
};


#endif 
