find_package(Protobuf CONFIG REQUIRED)
message(STATUS "Using protobuf ${Protobuf_VERSION}")
set(_PROTOBUF_PROTOC $<TARGET_FILE:protobuf::protoc>)

find_package(gRPC CONFIG REQUIRED)
message(STATUS "Using gRPC ${gRPC_VERSION}")
set(_GRPC_CPP_PLUGIN_EXECUTABLE $<TARGET_FILE:gRPC::grpc_cpp_plugin>)

add_custom_command(
	OUTPUT "${hw_proto_srcs}" "${hw_proto_hdrs}" "${hw_grpc_srcs}" "${hw_grpc_hdrs}"
	COMMAND ${_PROTOBUF_PROTOC}
	ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
		--cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
		-I "${hw_proto_path}"
		--plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
		"${hw_proto}"
	DEPENDS "${hw_proto}")
