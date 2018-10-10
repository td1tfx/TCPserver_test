#include "stdafx.h"
#include "session.h"


#include "nmap.h"

session::session(tcp::socket socket) : socket_(std::move(socket)) {

}


session::~session()
{
	if(argv0_ != NULL){
		delete argv0_;
	}
}

void session::start(char* argv0)
{
	setArgv0(argv0);
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
	char* commend[5];
	commend[0] = argv0_;
	//commend[0] = "C://Users//tfx//Downloads//scanProject//TCP//Release//TCP.exe";
	commend[1] = "172.16.1.77";
	commend[2] = "-p1-1000";
	commend[3] = "-oN";
	commend[4] = "output.txt";
	nmap_main(5, commend); 

	//get record

	std::ifstream in("output.txt");	
	std::string line; 	
	if (in) // �и��ļ�	
	{		while (getline (in, line))		
	{ 			
		write_(line.c_str());
	}	
	}else 
	{		
		write_("no scan result exist.");	
	}


}

void session::setArgv0(char* argv0) {
	argv0_ = new char[std::strlen(argv0) + 1];
	//memcpy_s(argv0_, std::strlen(argv0) * sizeof(char), argv0, std::strlen(argv0) * sizeof(char));
	strcpy(argv0_, argv0);
	//printf_s(argv0_);
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
				write_("Network Scan begin!");
				doScan();
			}
			//room_.deliver(read_msg_);
			readHeader();
		}
		else
		{
			//room_.leave(shared_from_this());
		}
	});

}

void session::write_() {
	auto self(shared_from_this());
	boost::asio::async_write(socket_,
		boost::asio::buffer(write_msgs_.front().data(),
			write_msgs_.front().getLength()),
		[this, self](boost::system::error_code ec, std::size_t /*length*/)
	{
		if (!ec)
		{
			write_msgs_.pop_front();
			if (!write_msgs_.empty())
			{
				write_();
			}
		}
		else
		{
			//socket_.close();
		}
	});
}

void session::write_(Message msg) {
		bool write_in_progress = !write_msgs_.empty();
		write_msgs_.push_back(msg);
		if (!write_in_progress) {
			write_();
		}
}

void session::write_(const char* msg) {
	Message msg_t;
	msg_t.setBodyLength(std::strlen(msg));
	std::memcpy(msg_t.body(), msg, msg_t.getBodyLength());
	msg_t.encodeHeader();
	write_(msg_t);
}



