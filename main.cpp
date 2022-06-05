#include "Server.hpp"

#define IP "127.0.0.1"

std::vector<std::string>	checkData(int argc, char *argv[])
{
	std::vector<std::string>	args;

	if (argc < 2 || argc > 3)
		return (args);
	if (argc == 2)
	{
		std::string tmp(argv[1]);
		std::istringstream str(tmp);
		std::string bufer;

		if (tmp.back() == ':')
			return args;
		while (std::getline(str, bufer, ':'))
			args.push_back(bufer);
		if (args.size() != 3)
			return (std::vector<std::string>());
	}
	else 
	{
		args.push_back(IP);
		args.push_back(argv[1]);
		args.push_back(argv[2]);
	}
	return (args);
}

int 	main(int argc, char *argv[]) 
{
	std::vector<std::string> args;
	if ((args = checkData(argc, argv)).empty())
		return (std::cerr << "./ircserv <port> <password>" << std::endl, 1);
	Server server(args[0], args[1], args[2]);
	server.startServ();
	return 0;
}
