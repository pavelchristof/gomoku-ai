package(default_visibility = ["//visibility:private"])

cc_library(
    name = "supervisor",
    hdrs = ["supervisor.h"],
    srcs = ["supervisor.cc"],
    deps = [
        "//gomoku/actors:actor_registry",
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
        "//gomoku/actors:actor_spec_proto_cc",
        "//gomoku/actors:all_actors",
        "//gomoku/core:actor",
    ],
    linkstatic = True,
)
