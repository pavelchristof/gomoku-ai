load("@protobuf//:protobuf.bzl", "cc_proto_library")

package(default_visibility = ["//visibility:public"])

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
        "//gomoku/core:metrics_proto_cc",
        "//gomoku/core:recording_proto_cc",
    ],
    use_grpc_plugin = True,
    cc_libs = ["@protobuf//:protobuf"],
    protoc = "@protobuf//:protoc",
    default_runtime = "@protobuf//:protobuf",
)

cc_library(
    name = "ladder",
    hdrs = ["ladder.h"],
    srcs = ["ladder.cc"],
    deps = [
        ":ladder_spec_proto_cc",
        ":worker_proto_cc",
        "//tensorflow/core:lib",
    ]
)

cc_library(
    name = "supervisor",
    hdrs = ["supervisor.h"],
    srcs = ["supervisor.cc"],
    deps = [
        ":ladder",
        ":worker_proto_cc",
        "//tensorflow/core:lib",
    ],
)

cc_library(
    name = "worker_impl",
    hdrs = ["worker_impl.h"],
    srcs = ["worker_impl.cc"],
    deps = [
        ":worker_proto_cc",
        "//gomoku/actors:actor_registry",
        "//gomoku/core:metrics",
        "//tensorflow/core:lib",
    ],
)
