FROM lvfeiyang/leon-cpp-dev:v4

COPY ./hadoop-3.2.2.tar.gz ./jre-8u291-linux-x64.tar.gz /home/lxj/
COPY ./hdfs-cli.sh /etc/profile.d/
RUN tar -zxf hadoop-3.2.2.tar.gz -C /opt && tar -zxf jre-8u291-linux-x64.tar.gz -C /opt && \
	apt update && apt install -y --no-upgrade ca-certificates lsb-release wget && \
	wget https://apache.jfrog.io/artifactory/arrow/$(lsb_release --id --short | tr 'A-Z' 'a-z')/apache-arrow-apt-source-latest-$(lsb_release --codename --short).deb && \
	apt install -y ./apache-arrow-apt-source-latest-$(lsb_release --codename --short).deb && apt update && \
	apt install -y libarrow-dev libparquet-dev && echo "source /etc/profile" >> /root/.bashrc
