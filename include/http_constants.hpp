//
// Created by sergey on 21.09.19.
//

#ifndef STATIC_SERVER_HTTP_CONSTANTS_HPP
#define STATIC_SERVER_HTTP_CONSTANTS_HPP

#include <string>
#include <unordered_map>

namespace http {
// end_line character
	constexpr auto EOL = "\r\n";
// methods
	constexpr auto METHOD_GET = "GET";
	constexpr auto METHOD_HEAD = "HEAD";

// exception strings
	constexpr auto NOT_ALLOWED_STR = "invalid method";
	constexpr auto INVALID_REQ_LINE = "invalid request line";
	constexpr auto INVALID_URI = "invalid uri";
	constexpr auto NOT_FOUND_STR = "not found";

// status codes
	constexpr int OK = 200;
	constexpr int NOT_FOUND = 404;
	constexpr int FORBIDDEN = 403;
	constexpr int NOT_ALLOWED = 405;
	constexpr int INTERNAL_SERVER_ERROR = 500;

// status messages

	constexpr auto OK_MES = "OK";
	constexpr auto FORBIDDEN_MES = "Forbidden";
	constexpr auto NOT_FOUND_MES = "Not Found";
	constexpr auto NOT_ALLOWED_MES = "Method Not Allowed";
	constexpr auto INTERNAL_SERVER_ERROR_MES = "Internal Server Error";

// connection close
	constexpr auto CLOSE = "close";
	constexpr auto KEEP_ALIVE = "Keep-Alive";

// content types

	constexpr auto TEXT_PLAIN = "text/plain";
	constexpr auto TEXT_HTML = "text/html";
	constexpr auto TEXT_CSS = "text/css";

	constexpr auto IMAGE_JPG = "image/jpeg";
	constexpr auto IMAGE_PNG = "image/png";
	constexpr auto IMAGE_GIF = "image/gif";

	constexpr auto APPLICATION_JS = "application/javascript";
	constexpr auto APPLICATION_SWF = "application/x-shockwave-flash";

// file extensions

	constexpr auto HTML = ".html";
	constexpr auto CSS = ".css";
	constexpr auto TXT = ".txt";

	constexpr auto JPEG = ".jpeg";
	constexpr auto JPG = ".jpg";
	constexpr auto PNG = ".png";
	constexpr auto GIF = ".gif";

	constexpr auto JS = ".js";
	constexpr auto SWF = ".swf";


// version

	constexpr auto DEFAULT_VERSION = "HTTP/1.1";
// index

	constexpr auto INDEX = "index.html";
}


#endif //STATIC_SERVER_HTTP_CONSTANTS_HPP
