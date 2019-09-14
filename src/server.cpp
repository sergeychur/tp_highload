//
// Created by sergey on 14.09.19.
//

#include <iostream>

#include "server.hpp"

void Server::Run() {
	std::cout << config_.document_root << " " << config_.cpu_limit << " " << config_.port << " " << config_.thread_limit << std::endl;
	return;
}

