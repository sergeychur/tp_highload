//
// Created by sergey on 14.09.19.
//

#include <iostream>

#include "server.hpp"

Server::Server(Config&& config) : config_(config) {
	struct sockaddr_in addr = {0};
	if (uv_ip4_addr(config_.host.c_str(), config_.port, &addr) != 0) {
		throw std::runtime_error("Cannot get sockaddr_in struct");
	}
	for (auto i = 0; i < config_.thread_limit; ++i) {
		thread_pool_.emplace_back(std::make_shared<Routine>(i, addr, config_.document_root));
	}
}

void Server::Run() {
	for (auto& routine : thread_pool_) {
		try {
			routine->Run();
		} catch (std::exception& e) {
			std::cerr << e.what() << std::endl;
		}
	}
	for (auto& routine : thread_pool_) {
		routine->Stop();
	}
}

