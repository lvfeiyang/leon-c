FROM lvfeiyang/leon-cpp-dev:v2

COPY ./mysqlconn-8.0.22 /home/lxj/mysqlconn-8.0.22/
RUN apt-get install -yq librdkafka-dev && dpkg -i mysqlconn-8.0.22/*.deb
