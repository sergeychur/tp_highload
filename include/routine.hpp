//
// Created by sergey on 15.09.19.
//

#ifndef STATIC_SERVER_ROUTINE_HPP
#define STATIC_SERVER_ROUTINE_HPP

#include <memory>
#include <uv.h>

#include "http_handler.hpp"


struct Routine {
	public:
		Routine() = default;
		Routine(const int id, struct sockaddr_in& addr, const std::string& root): id_(id), addr_(addr),
				document_root_(root), http_handler(std::make_shared<HTTPHandler>()) {}
		void Run();
		void Stop();
		std::shared_ptr<HTTPHandler> http_handler;
		std::shared_ptr<uv_thread_t> thread_;
		int id_;
		struct sockaddr_in addr_;
		constexpr static int connections_count = 1024;
		std::string document_root_;
};

#endif //STATIC_SERVER_ROUTINE_HPP
