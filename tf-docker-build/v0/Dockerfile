FROM tensorflow/tensorflow:devel-gpu

RUN cd /tensorflow_src && ./configure && \
	bazel build --config=opt --config=cuda --config=noaws --config=mkl //tensorflow/tools/pip_package:build_pip_package
