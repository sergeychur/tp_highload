//
// Created by sergey on 14.09.19.
//

#include <iostream>

#include "config.hpp"
#include "server.hpp"

int main(int argc, char** argv) {
	if (argc != 2) {
		throw std::invalid_argument("No path to config file specified");
	}
	const std::string file_path(argv[1]);
	Config config(file_path);
	Server server(std::move(config));
	try {
		server.Run();
	}
	catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
	return 0;
}
