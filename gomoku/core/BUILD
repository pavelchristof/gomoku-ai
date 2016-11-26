load("@protobuf//:protobuf.bzl", "cc_proto_library")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "integral_types",
    hdrs = ["integral_types.h"],
)

cc_library(
    name = "board",
    hdrs = ["board.h"],
    srcs = ["board.cc"],
    deps = [
        ":integral_types",
        "//tensorflow/core:framework_headers_lib",
        "//third_party/eigen3",
    ],
)

cc_test(
    name = "board_test",
    srcs = ["board_test.cc"],
    deps = [
        ":board",
        "//tensorflow/core:test",
    ],
)

cc_library(
    name = "feature",
    hdrs = ["feature.h"],
    srcs = ["feature.cc"],
    deps = [
        ":board",
        "//third_party/eigen3",
    ],
)

cc_test(
    name = "feature_test",
    srcs = ["feature_test.cc"],
    deps = [
        ":feature",
        "//tensorflow/core:test",
    ],
)

cc_library(
    name = "monte_carlo",
    hdrs = ["monte_carlo.h"],
    srcs = ["monte_carlo.cc"],
    deps = [
        ":board",
        ":feature",
    ],
)

cc_test(
    name = "monte_carlo_test",
    srcs = ["monte_carlo_test.cc"],
    deps = [
        ":monte_carlo",
        "//tensorflow/core:test",
    ],
)

cc_proto_library(
    name = "metrics_proto_cc",
    srcs = ["metrics.proto"],
    cc_libs = ["@protobuf//:protobuf"],
    protoc = "@protobuf//:protoc",
    default_runtime = "@protobuf//:protobuf",
)

cc_library(
    name = "metrics",
    hdrs = ["metrics.h"],
    srcs = ["metrics.cc"],
    deps = [
        ":metrics_proto_cc",
        "//tensorflow/core:lib",
    ],
)

cc_library(
    name = "actor",
    hdrs = ["actor.h"],
    srcs = ["actor.cc"],
    deps = [
        ":board",
        ":feature",
        ":metrics",
    ],
)

cc_proto_library(
    name = "recording_proto_cc",
    srcs = ["recording.proto"],
    cc_libs = ["@protobuf//:protobuf"],
    protoc = "@protobuf//:protoc",
    default_runtime = "@protobuf//:protobuf",
)
