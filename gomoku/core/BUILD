load("//tensorflow/core:platform/default/build_config.bzl", "tf_proto_library")

package(default_visibility = ["//gomoku"])

cc_library(
    name = "board",
    hdrs = ["board.h"],
    srcs = ["board.cc"],
    deps = [
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
    name = "features",
    hdrs = ["features.h"],
    srcs = ["features.cc"],
    deps = [
        ":board",
        "//tensorflow/core:framework_headers_lib",
        "//third_party/eigen3",
    ],
)

cc_test(
    name = "features_test",
    srcs = ["features_test.cc"],
    deps = [
        ":features",
        "//tensorflow/core:test",
    ],
)

cc_library(
    name = "actor",
    hdrs = ["actor.h"],
    srcs = ["actor.cc"],
    deps = [
        ":board",
        ":features",
    ],
)

tf_proto_library(
    name = "recording_proto",
    srcs = ["recording.proto"],
    cc_api_version = 2,
    visibility = ["//gomoku"],
)
