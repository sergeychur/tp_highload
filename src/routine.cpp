//
// Created by sergey on 15.09.19.
//

#include <iostream>
#include <sstream>
#include <thread>

#include "http_constants.hpp"
#include "routine.hpp"
#include "http_handler.hpp"

void on_alloc(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf) {
	buf->base = new char [suggested_size]();
	buf->len = suggested_size;
}

void on_write(uv_write_t *req, int status) {
	if(status) {
		std::cerr << uv_strerror(status) << std::endl;
	}
}

void on_read(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf) {
	if (nread < 0 && nread != UV_EOF) {
		uv_close((uv_handle_t*)client, nullptr);
	}
	if (nread > 0) {
		auto req = std::make_shared<uv_write_t>();
		auto routine = (Routine*)client->data;
		auto handler = routine->http_handler;
		std::string request_string(buf->base);
		Request request;
		request = handler->ParseRequest(request_string);
		std::string path_string = routine->document_root_ + request.uri;
		fs::path path = path_string;
		auto response = handler->FormResponse(request, path);
		std::ostringstream ss;
		ss << response;
		auto str = ss.str();
		uv_buf_t wrbuf = uv_buf_init(str.data(), str.length());
		uv_write(req.get(), client, &wrbuf, 1, on_write);
		uv_close((uv_handle_t*)client, nullptr);
	}

	delete buf->base;

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
