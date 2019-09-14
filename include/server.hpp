//
// Created by sergey on 14.09.19.
//

#ifndef STATIC_SERVER_SERVER_HPP
#define STATIC_SERVER_SERVER_HPP

#include "config.hpp"

class Server {
	public:
		explicit Server(Config&& config): config_(config) {}
		void Run();

	private:
		Config config_;
};
#endif //STATIC_SERVER_SERVER_HPP
