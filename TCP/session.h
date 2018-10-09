#pragma once

#include "boost/asio.hpp"
#include "Message.h"

using boost::asio::ip::tcp;

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

	enum { max_length = 1024 };
	char data_[max_length];
	Message msg_;

	char* argv0_;
};

