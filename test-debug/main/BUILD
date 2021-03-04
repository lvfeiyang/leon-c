cc_binary(
	name = "leon-debug",
	# includes = ["/var/lxj/wp/cpp/facebook/proxygen/proxygen/_build/include/proxygen/httpserver"],
	srcs = ["main.cpp"],
	# includes = [
	# 	"/usr/include/glog",
	# ],
	copts = ["-std=c++17"],
	linkopts = ["-lpthread", "-lgflags", "-lglog"],
	# copts = ["-std=c++17","-lpthread","-lgflags","-lglog","-lfolly","-lproxygenhttpserver"],
	# deps = [
	# 	"@usr//:glog", "@usr//:gflags",
	# 	"@usr//:boost_context", "@usr//:dl", "@usr//:event", "@usr//:pthread",
	# 	# "@usr//:usr-so",
	# 	"@fb//:fb",
	# ],
	# linkstatic = 0,
)