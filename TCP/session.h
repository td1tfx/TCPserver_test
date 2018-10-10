#pragma once

#include "boost/asio.hpp"
#include "Message.h"
#include <fstream>

using boost::asio::ip::tcp;
typedef std::deque<Message> Message_queue;

class session: public std::enable_shared_from_this<session>
{
public:
	session(tcp::socket socket);

	~session();
	void start(char* argv0);
	void setArgv0(char* argv0);

private:

	tcp::socket socket_;

	void do_read();
	void do_write(std::size_t length);
	void doScan();
	void readHeader();
	void readBody();
	void write_();
	void write_(Message msg);
	void write_(const char* msg);


	enum { max_length = 1024 };
	char data_[max_length];
	Message msg_;
	Message_queue write_msgs_;

	char* argv0_;
};

