FROM lvfeiyang/leon-cpp-dev:v1

WORKDIR /home/lxj
COPY ./hiredis-1.0.0 /home/lxj/hiredis-1.0.0/
RUN cd hiredis-1.0.0 && make && make install && ldconfig
