//
// Created by sergey on 14.09.19.
//

#include <fstream>
#include <iostream>
#include <unordered_set>
#include <sstream>
#include <unordered_map>

#include "config.hpp"


Config::Config(const std::string& file_path) {
	std::ifstream file(file_path, std::ifstream::in);
	std::unordered_set<std::string> parsing_set = {
			"cpu_limit", "thread_limit", "document_root", "port", "host"
	};
	std::unordered_map<std::string, std::string> parsing_map;
	while (file.good()) {
		std::string line;
		std::getline(file, line);
		if (!line.empty()) {
			std::istringstream line_stream(line);
			std::string key;
			line_stream >> key;
			auto iter = parsing_set.find(key);
			if (iter == parsing_set.end()) {
				throw std::invalid_argument("Invalid config, unknown field");
			}
			std::string value;
			line_stream >> value;
			parsing_map[key] = value;
		}
	}

	auto iter = parsing_map.find("cpu_limit");
	if (iter == parsing_map.end()) {
		throw std::invalid_argument("Invalid config, no cpu_limit found");
	}
	std::istringstream ss(iter->second);
	ss >> cpu_limit;

	iter = parsing_map.find("thread_limit");
	if (iter == parsing_map.end()) {
		throw std::invalid_argument("Invalid config, no thread_limit found");
	}
	ss.clear();
	ss.str(iter->second);
	ss >> thread_limit;

	iter = parsing_map.find("host");
	if (iter == parsing_map.end()) {
		throw std::invalid_argument("Invalid config, no host found");
	}
	ss.clear();
	ss.str(iter->second);
	ss >> host;

	iter = parsing_map.find("port");
	if (iter == parsing_map.end()) {
		throw std::invalid_argument("Invalid config, no port found");
	}
	ss.clear();
	ss.str(iter->second);
	ss >> port;

	iter = parsing_map.find("document_root");
	if (iter == parsing_map.end()) {
		throw std::invalid_argument("Invalid config, no document_root found");
	}
	ss.clear();
	ss.str(iter->second);
	ss >> document_root;
}
