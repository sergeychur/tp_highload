//
// Created by sergey on 14.09.19.
//

#ifndef STATIC_SERVER_SERVER_HPP
#define STATIC_SERVER_SERVER_HPP

#include <vector>

#include "config.hpp"
#include "routine.hpp"

class Server {
	public:
		explicit Server(Config&& config);
		void Run();

	private:
		Config config_;
		std::vector<std::shared_ptr<Routine>> thread_pool_;
};
#endif //STATIC_SERVER_SERVER_HPP
