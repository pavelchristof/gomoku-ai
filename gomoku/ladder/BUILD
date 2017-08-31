package(default_visibility = ["//visibility:public"])

proto_library(
  name = "ladder_spec_proto",
  srcs = ["ladder_spec.proto"],
  deps = ["//gomoku/actors:actor_spec_proto_cc"],
)

cc_proto_library(
    name = "ladder_spec_proto_cc",
    deps = ["ladder_spec_proto"],
)

proto_library(
    name = "worker_proto",
    srcs = ["worker.proto"],
    deps = [
        "//gomoku/actors:actor_spec_proto_cc",
        "//gomoku/core:metrics_proto_cc",
        "//gomoku/core:recording_proto_cc",
    ],
    use_grpc_plugin = True,
)

cc_proto_library(
    name = "worker_proto_cc",
    srcs = ["worker_proto"],
    use_grpc_plugin = True,
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
        "//gomoku/core:metrics",
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
