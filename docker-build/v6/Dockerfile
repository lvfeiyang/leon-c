FROM lvfeiyang/leon-cpp-dev:v5

COPY ./cpprestsdk-2.10.18 /home/lxj/cpprestsdk-2.10.18/
RUN apt-get install -yq g++ git libboost-atomic-dev libboost-thread-dev libboost-system-dev libboost-date-time-dev libboost-regex-dev libboost-filesystem-dev libboost-random-dev libboost-chrono-dev libboost-serialization-dev libwebsocketpp-dev openssl libssl-dev ninja-build && \
	cd cpprestsdk-2.10.18 && mkdir build.debug && cd build.debug && cmake -G Ninja .. -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=0 && ninja && ninja install
