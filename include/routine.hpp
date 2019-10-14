//
// Created by sergey on 15.09.19.
//

#ifndef STATIC_SERVER_ROUTINE_HPP
#define STATIC_SERVER_ROUTINE_HPP

#include <memory>
#include <uv.h>

#include "http_handler.hpp"
#include "thread_data.hpp"


struct Routine {
	public:
		Routine() = default;
		Routine(const int id, struct sockaddr_in& addr, const std::string& root): id_(id),  data(root, addr,
				connections_count) {}
		void Run();
		void Stop();
		std::shared_ptr<uv_thread_t> thread_;
		int id_;
		ThreadData data;
		constexpr static int connections_count = 1024;
};

#endif //STATIC_SERVER_ROUTINE_HPP
