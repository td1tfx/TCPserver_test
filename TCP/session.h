#pragma once

#include "boost/asio.hpp"


using boost::asio::ip::tcp;

class session: public std::enable_shared_from_this<session>
{
public:
	session(tcp::socket socket);

	~session();
	void start();

private:

	tcp::socket socket_;

	void do_read();
	void do_write(std::size_t length);
	void doScan();

	enum { max_length = 1024 };
	char data_[max_length];
};

