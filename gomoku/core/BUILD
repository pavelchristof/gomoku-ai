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

proto_library(
  name = "metrics_proto",
  srcs = ["metrics.proto"],
)

cc_proto_library(
    name = "metrics_proto_cc",
    deps = ["metrics_proto"],
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

proto_library(
  name = "recording_proto",
  srcs = ["recording.proto"],
)

cc_proto_library(
    name = "recording_proto_cc",
    deps = ["recording_proto"],
)
