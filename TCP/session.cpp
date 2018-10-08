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
	readHeader();
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

void session::readHeader() {
	auto self(shared_from_this());
	boost::asio::async_read(socket_, boost::asio::buffer(msg_.data(), Message::header_length),
		[this, self](boost::system::error_code ec, size_t)
	{
		if (!ec && msg_.decodeHeader())
		{
			readBody();
		}
		else
		{
			//room_.leave(shared_from_this());
		}
	
	});
}

void session::readBody() {
	auto self(shared_from_this());
	boost::asio::async_read(socket_,
		boost::asio::buffer(msg_.body(), msg_.getBodyLength()),
		[this, self](boost::system::error_code ec, std::size_t /*length*/)
	{
		if (!ec)
		{
			//auto ifo = new char[msg_.getBodyLength()];
			//memcpy(ifo, msg_.body(), msg_.getBodyLength());
			if (strncmp(msg_.body(), "scan", msg_.getBodyLength()) == 0) {
				doScan();
			}
			//room_.deliver(read_msg_);
			//do_read_header();
		}
		else
		{
			//room_.leave(shared_from_this());
		}
	});

}