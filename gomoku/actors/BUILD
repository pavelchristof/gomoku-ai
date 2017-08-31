package(default_visibility = ["//visibility:public"])

proto_library(
    name = "actor_spec_proto",
    srcs = ["actor_spec.proto"],
)

cc_proto_library(
    name = "actor_spec_proto_cc",
    deps = ["actor_spec_proto"],
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
    name = "mc_cell",
    hdrs = ["mc_cell.h"],
    srcs = ["mc_cell.cc"],
    deps = [
        "//gomoku/core:board",
        "//gomoku/core:feature",
    ],
)

cc_test(
    name = "mc_cell_test",
    srcs = ["mc_cell_test.cc"],
    deps = [
        ":mc_cell",
        "//tensorflow/core:test",
    ],
)

cc_library(
    name = "monte_carlo_actor",
    srcs = ["monte_carlo_actor.cc"],
    deps = [
        ":actor_config",
        ":actor_registry",
        ":mc_cell",
        "//gomoku/core:metrics",
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
