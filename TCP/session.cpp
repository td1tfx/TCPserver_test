#include "stdafx.h"
#include "session.h"
#include "nmap.h"

session::session(tcp::socket socket) : socket_(std::move(socket)) {

}


session::~session()
{
}

void session::start()
{
	do_read();
}

void session::do_read()
{
	auto self(shared_from_this());
	socket_.async_read_some(boost::asio::buffer(data_, max_length),
		[this, self](boost::system::error_code ec, std::size_t length)
	{
		if (!ec)
		{
			if (data_ == "scan"){
				doScan();
			}
			do_write(length);
		}
	});
}

void session::do_write(std::size_t length)
{
	auto self(shared_from_this());
	boost::asio::async_write(socket_, boost::asio::buffer(data_, length),
		[this, self](boost::system::error_code ec, std::size_t /*length*/)
	{
		if (!ec)
		{
			do_read();
		}
	});
}

void session::doScan(){
	char* commend = " 172.16.1.1/24 -p1-1000";
	nmap_main(2, &commend);

}