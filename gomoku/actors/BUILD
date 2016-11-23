load("@protobuf//:protobuf.bzl", "cc_proto_library")

package(default_visibility = ["//visibility:public"])

cc_proto_library(
    name = "actor_spec_proto_cc",
    srcs = ["actor_spec.proto"],
    cc_libs = ["@protobuf//:protobuf"],
    protoc = "@protobuf//:protoc",
    default_runtime = "@protobuf//:protobuf",
)

cc_library(
    name = "actor_config",
    hdrs = ["actor_config.h"],
    srcs = ["actor_config.cc"],
    deps = [":actor_spec_proto_cc"],
)

cc_library(
    name = "actor_registry",
    hdrs = ["actor_registry.h"],
    srcs = ["actor_registry.cc"],
    deps = [
        ":actor_spec_proto_cc",
        "//gomoku/core:actor",
        "//tensorflow/core:lib",
    ],
)

cc_library(
    name = "random_actor",
    srcs = ["random_actor.cc"],
    deps = [":actor_registry"],
    alwayslink = 1,
)

cc_library(
    name = "monte_carlo_actor",
    srcs = ["monte_carlo_actor.cc"],
    deps = [
        ":actor_config",
        ":actor_registry",
        "//gomoku/core:monte_carlo",
    ],
    alwayslink = 1,
)

cc_library(
    name = "all_actors",
    deps = [
        ":random_actor",
        ":monte_carlo_actor",
    ],
)
