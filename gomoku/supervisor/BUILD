load("@protobuf//:protobuf.bzl", "cc_proto_library")

package(default_visibility = ["//gomoku"])

cc_proto_library(
    name = "ladder_spec_proto_cc",
    srcs = ["ladder_spec.proto"],
    deps = ["//gomoku/actors:actor_spec_proto_cc"],
    cc_libs = ["@protobuf//:protobuf"],
    protoc = "@protobuf//:protoc",
    default_runtime = "@protobuf//:protobuf",
)

cc_proto_library(
    name = "worker_proto_cc",
    srcs = ["worker.proto"],
    deps = [
        "//gomoku/actors:actor_spec_proto_cc",
        "//gomoku/core:recording_proto_cc",
    ],
    use_grpc_plugin = True,
    cc_libs = ["@protobuf//:protobuf"],
    protoc = "@protobuf//:protoc",
    default_runtime = "@protobuf//:protobuf",
)
