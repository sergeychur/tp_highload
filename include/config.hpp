//
// Created by sergey on 14.09.19.
//

#ifndef STATIC_SERVER_CONFIG_HPP
#define STATIC_SERVER_CONFIG_HPP

#include <string>

struct Config {
	public:
		Config() = default;
		explicit Config(const std::string& file_path);
		int cpu_limit;
		int thread_limit;
		std::string document_root;
		int port;

};
#endif //STATIC_SERVER_CONFIG_HPP
