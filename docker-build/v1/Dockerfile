FROM lvfeiyang/proxygen:v2

WORKDIR /home/lxj
COPY ./protobuf-3.14.0 /home/lxj/protobuf-3.14.0/
RUN apt-get install -yq autoconf automake curl && \
	cd protobuf-3.14.0 && ./configure && make && make install && ldconfig
