//
// Created by sergey on 21.09.19.
//

#ifndef STATIC_SERVER_HTTP_HANDLER_HPP
#define STATIC_SERVER_HTTP_HANDLER_HPP


#include <string>
#include <vector>
#include <unordered_map>
#include <filesystem>
// TODO(Me): change for simply filesystem

#include "http_constants.hpp"

namespace fs = std::filesystem;

struct Request {
	std::string method;
	std::string uri;
	std::string version;
	std::string error;
};

struct Response {
	const std::string server = "tp_highload";
	const std::unordered_map<int, std::string> code_map = {
			{http::OK, http::OK_MES},
			{http::FORBIDDEN, http::FORBIDDEN_MES},
			{http::NOT_FOUND, http::NOT_FOUND_MES},
			{http::NOT_ALLOWED, http::NOT_ALLOWED_MES},
			{http::INTERNAL_SERVER_ERROR, http::INTERNAL_SERVER_ERROR_MES},
		};
	std::string version;
	int code;
	std::string date;
	std::string connection;
	int content_length;
	std::string content_type;
	friend std::ostream& operator<<(std::ostream& os, const Response& obj);
};

class HTTPHandler {
	public:
		HTTPHandler();
		Request ParseRequest(std::string& request);
		Response FormResponse(const Request& req, fs::path& path);

	private:
		bool validateUri(std::string& uri);
		std::string urlDecode(std::string& url);
		const std::unordered_map<std::string, int> error_map = {
				{http::INVALID_URI, http::FORBIDDEN},
				{http::INVALID_REQ_LINE, http::NOT_ALLOWED},
				{http::NOT_ALLOWED_STR, http::NOT_ALLOWED},
				{http::NOT_FOUND_STR, http::NOT_FOUND}
		};
		std::string formContentType(fs::path& path);
		const std::unordered_map<std::string, std::string> content_type_map = {
				{http::HTML, http::TEXT_HTML},
				{http::CSS, http::TEXT_CSS},
				{http::TXT, http::TEXT_PLAIN},

				{http::JS, http::APPLICATION_JS},
				{http::SWF, http::APPLICATION_SWF},

				{http::JPG, http::IMAGE_JPG},
				{http::JPEG, http::IMAGE_JPG},
				{http::PNG, http::IMAGE_PNG},
				{http::GIF, http::IMAGE_GIF},
		};
};



#endif //STATIC_SERVER_HTTP_HANDLER_HPP
