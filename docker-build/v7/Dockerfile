FROM leon-cpp-dev:v6

COPY ./cmake-3.20.3-linux-x86_64.tar.gz /home/lxj/
COPY ./c-ares-1.17.1.tar.gz /home/lxj/
RUN tar -xzf c-ares-1.17.1.tar.gz && mkdir -p c-ares-1.17.1/build && pushd c-ares-1.17.1/build && \
	cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr/local .. && make && make isntall && popd && \
	tar -xzf cmake-3.20.3-linux-x86_64.tar.gz --strip-components=1 -C /usr/ && \
	git clone --recurse-submodules -b v1.38.1 https://github.com/grpc/grpc && \
	cd grpc && mkdir -p cmake/build && pushd cmake/build && \
	cmake -DgRPC_INSTALL=ON -DgRPC_BUILD_TESTS=OFF -DCMAKE_INSTALL_PREFIX=/usr/local -DCMAKE_BUILD_TYPE=Release \
	# -DgRPC_ABSL_PROVIDER=package -DgRPC_CARES_PROVIDER=package -DgRPC_PROTOBUF_PROVIDER=package -DgRPC_RE2_PROVIDER=package -DgRPC_SSL_PROVIDER=package \
	-DgRPC_ZLIB_PROVIDER=package ../.. && make -j6 && make install && popd && \
	mkdir -p third_party/abseil-cpp/cmake/build && pushd third_party/abseil-cpp/cmake/build && \
	cmake -DCMAKE_INSTALL_PREFIX=/usr/local -DCMAKE_POSITION_INDEPENDENT_CODE=TRUE ../.. && \
	make -j && make install && popd
