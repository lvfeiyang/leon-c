cc_import(
	name = "glog",
	hdrs = glob(["include/glog/**"]),
	# static_library = "lib/x86_64-linux-gnu/libglog.a",
	shared_library = "lib/x86_64-linux-gnu/libglog.so",
	visibility = ["//visibility:public"],
)
cc_import(
	name = "gflags",
	hdrs = glob(["include/gflags/**"]),
	# static_library = "lib/x86_64-linux-gnu/libgflags.a",
	shared_library = "lib/x86_64-linux-gnu/libgflags.so",
	visibility = ["//visibility:public"],
)

cc_import(
	name = "boost_context",
	shared_library = "lib/x86_64-linux-gnu/libboost_context.so",
	visibility = ["//visibility:public"],
)
cc_import(
	name = "dl",
	shared_library = "lib/x86_64-linux-gnu/libdl.so",
	visibility = ["//visibility:public"],
)
cc_import(
	name = "event",
	shared_library = "lib/x86_64-linux-gnu/libevent.so",
	visibility = ["//visibility:public"],
)
cc_import(
	name = "pthread",
	shared_library = "lib/x86_64-linux-gnu/libpthread.so",
	visibility = ["//visibility:public"],
)
cc_import(
	name = "event_pthreads",
	shared_library = "lib/x86_64-linux-gnu/libevent_pthreads.so",
	visibility = ["//visibility:public"],
)

cc_library(
	name = "usr-so",
	srcs = glob([
		"lib/x86_64-linux-gnu/*.so",
	]),
	visibility = ["//visibility:public"],
)

# cc_library(
# 	name = "usr-a",
# 	hdrs = glob(["include/**"]),
# 	srcs = [
# 		"lib/x86_64-linux-gnu/libglog.a",
# 		"lib/x86_64-linux-gnu/libpthread.a",
# 		"lib/x86_64-linux-gnu/libgflags.a",
# 	],
# 	includes = [
# 		"include/glog",
# 		"include",
# 		"include/gflags",
# 	],
# 	visibility = ["//visibility:public"],
# )
# cc_library(
# 	name = "unwind",
# 	hdrs = glob(["lib/gcc/x86_64-linux-gnu/7/include/**"]),
# 	srcs = [
# 		"lib/x86_64-linux-gnu/libunwind.so.8",
# 	],
# 	includes = ["include"],
# 	visibility = ["//visibility:public"],
# )
# cc_library(
# 	name = "usr-so",
# 	hdrs = glob(["include/**"]),
# 	srcs = [
# 		"lib/x86_64-linux-gnu/libglog.so",
# 		"lib/x86_64-linux-gnu/libgflags.so",
# 	],
# 	includes = [
# 		"include/glog",
# 		"include/gflags",
# 	],
# 	visibility = ["//visibility:public"],
# )
