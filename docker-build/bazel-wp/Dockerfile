FROM ubuntu:18.04

WORKDIR /home/lxj
VOLUME /root/.cache/bazel/

RUN apt update && apt install -yq curl gnupg && curl -fsSL https://bazel.build/bazel-release.pub.gpg | gpg --dearmor > bazel.gpg && \
	mv bazel.gpg /etc/apt/trusted.gpg.d/ && echo "deb [arch=amd64] https://storage.googleapis.com/bazel-apt stable jdk1.8" | tee /etc/apt/sources.list.d/bazel.list && \
	apt update && apt install -yq bazel
