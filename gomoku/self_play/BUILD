package(default_visibility = ["//visibility:private"])

cc_library(
    name = "supervisor",
    hdrs = ["supervisor.h"],
    srcs = ["supervisor.cc"],
    deps = [
        "//gomoku/core:actor",
        "//gomoku/core:recording_proto_cc",
        "//tensorflow/core:lib",
        "@gflags//:gflags",
    ],
)

cc_binary(
    name = "self_play",
    srcs = ["self_play.cc"],
    deps = [
        ":supervisor",
        "//gomoku/core:actor",
    ]
)
