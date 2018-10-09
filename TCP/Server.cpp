#include "stdafx.h"
#include "Server.h"



Server::~Server()
{
}


Server::Server(boost::asio::io_service& io_service, short port)
	: acceptor_(io_service, tcp::endpoint(tcp::v4(), port)),
	socket_(io_service)
{
	do_accept();
}

void Server::do_accept()
{
	acceptor_.async_accept(socket_,
		[this](boost::system::error_code ec)
	{
		if (!ec)
		{
			std::make_shared<session>(std::move(socket_))->start(argv0_);
		}

		do_accept();
	});
}

void Server::setArgv0(char* argv0) {
	argv0_ = new char[std::strlen(argv0)+1];
	//memcpy_s(argv0_, std::strlen(argv0) * sizeof(char), argv0, std::strlen(argv0) * sizeof(char));
	strcpy(argv0_, argv0);
	//printf_s(argv0_);
}

