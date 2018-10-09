#pragma once
#include "boost/asio.hpp"
#include "session.h"



using boost::asio::ip::tcp;

class Server
{
public:
	~Server();

	void setArgv0(char* argv0);
	Server::Server(boost::asio::io_service& io_service, short port);

private:

	void do_accept();


	tcp::acceptor acceptor_;
	tcp::socket socket_;

	char* argv0_;
};

