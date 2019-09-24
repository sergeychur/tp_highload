FROM ubuntu:18.04
USER root
RUN apt update -y
RUN apt install -y software-properties-common && add-apt-repository -y ppa:ubuntu-toolchain-r/test
RUN apt install -y cmake gcc-9 g++-9 libuv1.dev make

RUN update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-9 1

WORKDIR /static
COPY . .
RUN cmake CMakeLists.txt && make
EXPOSE 80
CMD /static/static_server /static/docker.cfg
