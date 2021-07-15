cc_binary(
	name = "hello-world",
	srcs = ["main.cpp"],
	deps = ["//third_party/folly:folly"],
)

cc_binary(
	name = "grpc-cli",
	srcs = ["grpc-client.cpp"],
	deps = [
		'@com_github_grpc_grpc//:grpc++',
		"//pbdata:hello-grpc",
	],
)

cc_binary(
	name = "grpc-ser",
	srcs = ["grpc-server.cpp"],
	deps = [
		'@com_github_grpc_grpc//:grpc++',
		"@com_github_grpc_grpc//:grpc++_reflection",
		"//:hello-grpc",
	],
)
