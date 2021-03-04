FROM lvfeiyang/leon-cpp-dev:v3

COPY ./yaml-cpp-yaml-cpp-0.6.3 /home/lxj/yaml-cpp-yaml-cpp-0.6.3/
RUN cd yaml-cpp-yaml-cpp-0.6.3 && mkdir build && cd build && cmake .. && make && make install && ldconfig
