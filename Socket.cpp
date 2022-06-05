#include "Socket.hpp"
# include <iostream>

fd_set  Socket::read_fd;
fd_set  Socket::write_fd;

Socket::Socket() 
{
    type = FD_FREE;
    socket_fd = -1;
}

Socket::Socket(Type type, int socket_fd)
{
    this->type = type;
    this->socket_fd = socket_fd;
}

int	Socket::read_sock()
{
    int 	nbytes;
	char	c_buf_read[BUF_SIZE + 1];

    memset(c_buf_read, 0, BUF_SIZE);
	nbytes = recv(socket_fd, c_buf_read, sizeof(c_buf_read), 0);
    buf_read += c_buf_read;
    return (nbytes);
}

void	Socket::write_sock()
{
    int nbytes;

	nbytes = send(socket_fd, buf_write.c_str(), strlen(buf_write.c_str()), 0);
	buf_write.clear();
}

Socket::Socket(const Socket& other)
{	
	type = other.type;
	socket_fd = other.socket_fd;
	buf_read = other.buf_read;
	buf_write = other.buf_write;
}

Socket::~Socket()
{
}
