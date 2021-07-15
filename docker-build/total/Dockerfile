FROM ubuntu:18.04

WORKDIR /home/lxj

COPY ./proxygen-2021.06.14.00.tar.gz \
	proxygen-config.cmake.in build.sh \
	protobuf-cpp-3.14.0.tar.gz \
	hiredis-1.0.0.tar.gz \
	yaml-cpp-yaml-cpp-0.6.3.tar.gz \
	./hadoop-3.2.2.tar.gz ./jre-8u291-linux-x64.tar.gz \
	cpprestsdk-2.10.18.tar.gz \
	./cmake-3.20.3-linux-x86_64.tar.gz \
	/home/lxj/install/
COPY ./mysqlconn-8.0.22 /home/lxj/install/mysqlconn-8.0.22/
COPY ./hdfs-cli.sh /etc/profile.d/

# install proxygen
RUN apt-get update && apt-get install -yq sudo git cmake && \
	tar -xzf install/proxygen-2021.06.14.00.tar.gz && mv proxygen-2021.06.14.00 proxygen && \
	cp -f install/proxygen-config.cmake.in proxygen/cmake/ && cp -f install/build.sh proxygen/proxygen/ && \
	git config --global http.proxy 'socks5://127.0.0.1:10808' && git config --global https.proxy 'socks5://127.0.0.1:10808' && \
	tar -xzf install/cmake-3.20.3-linux-x86_64.tar.gz --strip-components=1 -C /usr/ && \
	cd proxygen/proxygen && ./build.sh --no-fetch-dependencies && ./install.sh && cd /home/lxj

# install protobuf
RUN apt-get install -yq autoconf automake curl && \
	tar -xzf install/protobuf-cpp-3.14.0.tar.gz -C install && \
	cd install/protobuf-3.14.0 && ./configure && make && make install && ldconfig && cd /home/lxj && \
	# install redis
	tar -xzf install/hiredis-1.0.0.tar.gz -C install && \
	cd install/hiredis-1.0.0 && make && make install && ldconfig && cd /home/lxj && \
	# install kafka mysql
	apt-get install -yq librdkafka-dev && dpkg -i install/mysqlconn-8.0.22/*.deb && \
	# install yaml
	tar -xzf install/yaml-cpp-yaml-cpp-0.6.3.tar.gz -C install && \
	mkdir -p install/yaml-cpp-yaml-cpp-0.6.3/build && cd install/yaml-cpp-yaml-cpp-0.6.3/build && \
	cmake .. && make && make install && ldconfig && cd /home/lxj && \
	# install cpprest
	apt-get install -yq g++ git libboost-atomic-dev libboost-thread-dev libboost-system-dev libboost-date-time-dev libboost-regex-dev \
	libboost-filesystem-dev libboost-random-dev libboost-chrono-dev libboost-serialization-dev libwebsocketpp-dev openssl libssl-dev ninja-build && \
	tar -xzf install/cpprestsdk-2.10.18.tar.gz -C install && mkdir -p install/cpprestsdk-2.10.18/build.debug && cd install/cpprestsdk-2.10.18/build.debug && \
	cmake -G Ninja .. -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=0 && ninja && ninja install && cd /home/lxj

# install hdfs
RUN tar -zxf install/hadoop-3.2.2.tar.gz -C /opt && tar -zxf install/jre-8u291-linux-x64.tar.gz -C /opt && \
	apt update && apt install -y --no-upgrade ca-certificates lsb-release wget && \
	wget https://apache.jfrog.io/artifactory/arrow/$(lsb_release --id --short | tr 'A-Z' 'a-z')/apache-arrow-apt-source-latest-$(lsb_release --codename --short).deb && \
	apt install -y ./apache-arrow-apt-source-latest-$(lsb_release --codename --short).deb && apt update && \
	apt install -y libarrow-dev libparquet-dev && echo "source /etc/profile" >> /root/.bashrc

# install grpc
RUN	git clone --recurse-submodules -b v1.38.1 https://github.com/grpc/grpc && \
	mkdir -p grpc/cmake/build && cd grpc/cmake/build && \
	cmake -DgRPC_INSTALL=ON -DgRPC_BUILD_TESTS=OFF -DCMAKE_INSTALL_PREFIX=/usr/local -DCMAKE_BUILD_TYPE=Release \
	# -DgRPC_ABSL_PROVIDER=package -DgRPC_CARES_PROVIDER=package -DgRPC_PROTOBUF_PROVIDER=package -DgRPC_RE2_PROVIDER=package -DgRPC_SSL_PROVIDER=package -DgRPC_ZLIB_PROVIDER=package \
	../.. && make -j6 && make install && cd /home/lxj && \
	mkdir -p grpc/third_party/abseil-cpp/cmake/build && cd grpc/third_party/abseil-cpp/cmake/build && \
	cmake -DCMAKE_INSTALL_PREFIX=/usr/local -DCMAKE_POSITION_INDEPENDENT_CODE=TRUE ../.. && \
	make -j && make install && cd /home/lxj

# rm install
RUN git config --global --unset http.proxy && git config --global --unset https.proxy && \
	rm -rf install
