# load("@rules_proto//proto:defs.bzl", "proto_library")
# proto_library(
# 	name = "hello-proto",
# 	srcs = ["pbdata/hello.proto"],
# )
# cc_proto_library(
# 	name = "hello",
# 	deps = [":hello-proto"],
# )
load("@com_github_grpc_grpc//bazel:cc_grpc_library.bzl", "cc_grpc_library")
cc_grpc_library(
	name = "hello-grpc",
	srcs = ["hello.proto"],
	deps = [],
	# grpc_only = True,
	visibility = ["//visibility:public"],
)
