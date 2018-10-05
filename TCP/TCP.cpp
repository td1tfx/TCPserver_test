// TCP.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <utility>
#include "boost/asio.hpp"
#include "Server.h"

int main(int argc, char* argv[])
{
	try {
		if (2 != 2) {
			std::cerr << "Usage: ";
			return 1;
		}

		boost::asio::io_service io_service;

		Server s(io_service, 2001);
		io_service.run();
	}
	catch (std::exception& e) {
				
	
	}

    return 0;
}

