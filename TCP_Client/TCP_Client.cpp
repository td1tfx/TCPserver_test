// TCP_Client.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "boost\asio.hpp"
#include <iostream>
#include "Message.h"
#include "Client.h"
#include <thread>

using boost::asio::ip::tcp;

enum {max_length = 1024};

int main(int argc, char* argv[])
{
	try {
		if (3 != 3) {
			return 1;
		}	
		boost::asio::io_service io_service;

		tcp::socket s(io_service);
		tcp::resolver resolver(io_service);
		//tcp::endpoint ep(boost::asio::ip::address::from_string("127.0.0.1"), 2001);
		//s.connect(ep);
		//boost::asio::connect(s, resolver.resolve({ argv[1],argv[2] }));
		auto endpoint_iterator = resolver.resolve({"127.0.0.1","2001"});

		Client c(io_service, endpoint_iterator);

		std::thread t([&io_service]() {io_service.run(); });

		std::cout << "Enter message:";
		char request[Message::max_body_length+1];
		while (std::cin.getline(request, Message::max_body_length+1)) {
			//boost::asio::write(s, boost::asio::buffer(msg.data(), msg.getLength()));
			c.write(request);
			//if (strncmp(request, "exit", std::strlen(request)) == 0) {
			//	break;
			//}
		}
		c.close();
		t.join();

		//char reply[max_length];
		//size_t reply_length = boost::asio::read(s, boost::asio::buffer(reply, request_length));
		//std::cout << "Reply is:";
		//std::cout.write(reply, reply_length);
		//std::cout << "\n";
	}
	catch (std::exception &e) {
		std::cerr << "Exception:" << e.what() << "\n";
	}



    return 0;
}

