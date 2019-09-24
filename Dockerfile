FROM ubuntu:18.04
USER root
RUN apt update -y
RUN apt install -y software-properties-common && add-apt-repository -y ppa:ubuntu-toolchain-r/test

RUN apt install -y cmake gcc-9 g++-9 libuv-dev make

WORKDIR /static_server
COPY . .
RUN cmake CMakeLists.txt && make
EXPOSE 80
CMD ["./static_server ./docker.cfg"]
