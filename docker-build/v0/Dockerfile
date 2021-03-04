FROM ubuntu:18.04

WORKDIR /home/lxj
COPY ./proxygen /home/lxj/proxygen/
RUN apt-get update && apt-get install -yq sudo && \
	cd proxygen/proxygen && ./build.sh && ./install.sh
