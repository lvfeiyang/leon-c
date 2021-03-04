cc_library(
	name = "fb",
	hdrs = glob(["include/**", "deps/include/**"]),
	srcs = [
		"lib/libproxygenhttpserver.a",
		"lib/libproxygen.a",
		"deps/lib/libfolly.a"
	],
	includes = [
		"include",
		"deps/include",
	],
	visibility = ["//visibility:public"],
)

cc_import(
	name = "httpserver",
	hdrs = glob(["include/proxygen/httpserver/**"]),
	static_library = "lib/libproxygenhttpserver.a",
	visibility = ["//visibility:public"],
)