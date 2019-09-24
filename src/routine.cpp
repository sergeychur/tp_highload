//
// Created by sergey on 15.09.19.
//

#include <iostream>
#include <sstream>
#include <thread>
#include <uv.h>

#include <unistd.h>

#include <sys/sendfile.h>
#include <cstring>

#include "http_constants.hpp"
#include "routine.hpp"
#include "http_handler.hpp"


void on_alloc(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf) {
	buf->base = new char [suggested_size]();
	buf->len = suggested_size;
}

void send_file(int fd_to_write, const std::string& file_name) {
//	uv_fs_t open_req = {nullptr};
//	int fd_to_read = uv_fs_open(client->loop, &open_req, file_name.data(), O_RDONLY, 0, nullptr);
//
//	if (fd_to_read != -1) {
//		uv_fs_t stat_req = {nullptr};
//		uv_fs_fstat(client->loop, &stat_req, fd_to_read, nullptr);
//		auto size = stat_req.statbuf.st_size;
//		uv_fs_t send_req = {nullptr};
//		send_req.data = (void*)client;
//		int fd_to_write = client->io_watcher.fd;
//		auto remained = size;
//		while(remained > 0) {
//			remained -= sendfile(fd_to_read, fd_to_write, 0, 32);
//			std::cout << remained << std::endl;
//		}
//		uv_fs_req_cleanup(&stat_req);
//		std::cout << std::strerror(errno) << std::endl;
//	}
//	uv_fs_req_cleanup(&open_req);
//	int fd_to_read = open(file_name.data(), O_RDONLY);
//	if (fd_to_read != -1) {
//		struct stat buf{};
//		fstat(fd_to_read, &buf);
//		auto remained = buf.st_size;
//		struct stat sbuf{};
//		fstat(fd_to_write, &sbuf);
//		auto status = 0;
//		while(status != -1) {
//			auto sent = sendfile(fd_to_write, fd_to_read, 0, 1024);
//			remained -= sent;
//			status = sent;
//			std::cout << status << std::strerror(errno) << std::endl;
//		}
//		close(fd_to_read);
//	}
}

void on_send(uv_fs_t* req) {
//	auto kek = (Kek*)req->data;
//	uv_close((uv_handle_t*)kek->client, nullptr);
//	uv_fs_req_cleanup(req);
}

void on_write(uv_write_t *req, int status) {
	if(status) {
		std::cerr << uv_strerror(status) << std::endl;
		return;
	}

}

std::pair<std::string, bool> get_path_string(const std::string& root, const std::string& file_path) {
	if (*(file_path.end() - 1) == '/') {
		if (file_path.find('.') == file_path.npos) {
			return {root + file_path + http::INDEX, true};
		}
	}
	return {root + file_path, false};
}

void on_close(uv_handle_t* client) {

}

void on_read(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf) {
	if (nread < 0 && nread != UV_EOF) {
		uv_close((uv_handle_t *) client, nullptr);
		std::cerr << uv_strerror(nread) << std::endl;
		delete buf->base;
		return;
	}
	if (nread > 0) {
		auto req = std::make_shared<uv_write_t>();
		auto routine = (Routine *) client->data;
		auto handler = routine->http_handler;
		std::string request_string(buf->base);
		Request request;
		request = handler->ParseRequest(request_string);
		auto path_string_pair = get_path_string(routine->document_root_, request.uri);
		fs::path file_path(path_string_pair.first.data());
		if (!fs::exists(file_path) && request.error.empty()) {
			if (path_string_pair.second) {
				request.error = http::FORBIDDEN_STR;
			} else {
				request.error = http::NOT_FOUND_STR;
			}
		}
		auto response = handler->FormResponse(request, file_path);
		std::ostringstream ss;
		ss << response;
		auto str = ss.str();
		int status = 0;
		uv_buf_t wrbuf = uv_buf_init(str.data(), str.length());
		if (uv_try_write(client, &wrbuf, 1) < 0) {
//			std::cerr << "ehh " << std::strerror(errno) << std::endl;
		} else {
//			std::cout << status << std::strerror(errno) << std::endl;
		}

		if (request.method == http::METHOD_GET && response.code == http::OK) {
			int fd_to_read = open(path_string_pair.first.data(), O_RDONLY);
				if (fd_to_read >= 0) {
					struct stat buf{};
					fstat(fd_to_read, &buf);
					auto remained = buf.st_size;
					while(remained > 0) {
						status = sendfile(client->io_watcher.fd, fd_to_read, 0, 1024);
						if (status > 0) {
							remained -= status;
						}
//						std::cout << "sendfile " << status << std::strerror(errno) << std::endl;
					}
					close(fd_to_read);
				}
		}
		uv_close((uv_handle_t*)client, on_close);
	}
}

void on_new_connection(uv_stream_t *server, int status) {
	if (status < 0) {
		throw std::runtime_error(uv_strerror(status));
	}
	auto client = std::make_shared<uv_tcp_t>();
	client->data = server->data;
	uv_tcp_init(server->loop, client.get());
	auto stream_client = (uv_stream_t*)client.get();
	if (uv_accept(server, stream_client) == 0) {
		uv_read_start(stream_client, on_alloc, on_read);
	} else {
		uv_close((uv_handle_t*)(client).get(), nullptr);
	}
}

void run_(void* args){
	auto routine = static_cast<Routine*>(args);
	auto loop = std::make_shared<uv_loop_t>();
	int error = uv_loop_init(loop.get());
	if (error) {
		std::cerr << uv_strerror(error) << std::endl;
		return;
	}

	uv_tcp_t server = {};
	server.data = (void*)routine;
	int fd = 0;
	int option = 1;
	uv_tcp_init_ex(loop.get(), &server, routine->addr_.sin_family);
	uv_fileno((uv_handle_t*)&server, &fd);	// get the file descriptor g=for socket
	setsockopt(fd, SOL_SOCKET, SO_REUSEPORT, &option, sizeof(option)); // allow the port reuse
	uv_tcp_bind(&server, (struct sockaddr*)&routine->addr_, 0);
	error = uv_listen((uv_stream_t*) &server, routine->connections_count, on_new_connection);
	if (error) {
		std::cerr << uv_strerror(error) << std::endl;
		return;
	}
	uv_run(loop.get(), UV_RUN_DEFAULT);
}

void Routine::Run() {
	thread_ = std::make_shared<uv_thread_t>();
	std::cerr << "started " << id_ << std::endl;
	uv_thread_create(thread_.get(), run_, (void*)this);
}

void Routine::Stop() {
	uv_thread_join(thread_.get());
}
