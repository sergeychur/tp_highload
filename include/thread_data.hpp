//
// Created by sergey on 14.10.19.
//

#ifndef STATIC_SERVER_THREAD_DATA_HPP
#define STATIC_SERVER_THREAD_DATA_HPP

#include <netinet/in.h>
#include "http_handler.hpp"

struct ThreadData {
	ThreadData(std::string root, struct sockaddr_in addr_, int conn_count_): conn_count(conn_count_),
			document_root(root), addr(addr_), handler() {}
	int conn_count;
	std::string document_root;
	struct sockaddr_in addr;
	HTTPHandler handler;
};

#endif //STATIC_SERVER_THREAD_DATA_HPP
