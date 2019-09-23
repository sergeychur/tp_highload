//
// Created by sergey on 21.09.19.
//

#include <chrono>
#include <thread>
#include <sstream>
#include <iostream>
#include <iomanip>

#include "http_handler.hpp"
#include "http_constants.hpp"


Request HTTPHandler::ParseRequest(std::string &request) {
	std::istringstream ss(request);
	std::string method;
	std::string uri;
	std::string version;
	ss >> method >> uri >> version;
	auto req = Request();
	if (method.empty() || uri.empty() || uri.empty()) {
		req.error = http::INVALID_REQ_LINE;
		req.version = http::DEFAULT_VERSION;
		return req;
	}
	req.uri = urlDecode(req.uri);
	req.uri = req.uri.substr(0, req.uri.find("?"));
	req.method = method;
	req.version = version;
	if (method != http::METHOD_GET && method != http::METHOD_HEAD) {
		req.error = http::NOT_ALLOWED_STR;
		return req;
	}
	req.uri = uri;
	bool valid = validateUri(req.uri);
	if (!valid) {
		req.error = http::INVALID_URI;
	}
	return req;
}

bool HTTPHandler::validateUri(std::string& uri) {
	std::string forbidden_combo = "/../";
	auto pos = uri.find(forbidden_combo);
	return (pos == uri.npos);
}

static char from_hex(char ch) {
	return isdigit(ch) ? ch - '0' : tolower(ch) - 'a' + 10;
}

std::string HTTPHandler::urlDecode(std::string &url) {
	char h;
	std::ostringstream escaped;
	escaped.fill('0');

	for (auto i = url.begin(), n = url.end(); i != n; ++i) {
		std::string::value_type c = (*i);

		if (c == '%') {
			if (i[1] && i[2]) {
				h = from_hex(i[1]) << 4 | from_hex(i[2]);
				escaped << h;
				i += 2;
			}
		} else if (c == '+') {
			escaped << ' ';
		} else {
			escaped << c;
		}
	}

	return escaped.str();
}

static std::string get_rfc_date() {
	std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
	std::time_t now_time = std::chrono::system_clock::to_time_t(now);
	std::ostringstream date;
	date << std::put_time(std::gmtime(&now_time), "%a, %d %b %Y %H:%M:%S GMT");
	return date.str();
}

Response HTTPHandler::FormResponse(const Request& req, fs::path& path) {
	Response response;
	if (!req.error.empty()) {
		auto iter = error_map.find(req.error);
		if (iter != error_map.end()) {
			response.code = iter->second;
		}
	} else {
		bool exception_caught = false;
		try {
			response.content_length = req.method == http::METHOD_GET ?fs::file_size(path) : 0;
		} catch (fs::filesystem_error& e) {
			response.code = http::INTERNAL_SERVER_ERROR;
			exception_caught = true;
		}
		if (!exception_caught) {
			response.content_type = formContentType(path);
			response.code = http::OK;
		}
	}
	response.version = req.version;
	response.date = get_rfc_date();
	response.connection = http::CLOSE;
	return response;
}

std::string HTTPHandler::formContentType(fs::path& path) {
	std::string extension = path.extension().generic_string();
	auto iter = content_type_map.find(extension);
	if (iter != content_type_map.end()) {
		return iter->second;
	}
	return "";
}

std::ostream& operator<<(std::ostream& os, const Response& obj) {
	auto iter = obj.code_map.find(obj.code);
	if (iter != obj.code_map.end()) {
		os << obj.version << " " << obj.code << " "<< iter->second << http::EOL;
		os << "Date: " << obj.date << http::EOL;
		os << "Server: " << obj.server << http::EOL;
//		os << "Connection: " << obj.connection << http::EOL;
//		os << "Keep-Alive: 300" << http::EOL;
		if (obj.code == http::OK) {
			os << "Content-Length: " << obj.content_length << http::EOL;
			if (!obj.content_type.empty()) {
				os << "Content-Type: " << obj.content_type << http::EOL;
			}
		}
		os << http::EOL;
		return os;
	}
	throw std::runtime_error("invalid response code");
}