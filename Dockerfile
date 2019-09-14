FROM alpine
USER root
RUN apt update -y
RUN apt install -y cmake g++ libuv-dev make

WORKDIR /static_server
COPY . .
RUN cmake --build CMakeLists.txt
EXPOSE 80
CMD ["./static_server ./http.cfg"]
