cc_binary(
    name = "worker",
    srcs = ["worker.cc"],
    deps = [
        "//gomoku/actors:all_actors",
        "//gomoku/ladder:worker_impl",
        "//tensorflow/core:lib",
        "@gflags//:gflags",
        "@grpc//:grpc++",
    ],
)

cc_binary(
    name = "test_ladder",
    srcs = ["test_ladder.cc"],
    deps = [
        "//gomoku/actors:actor_config",
        "//gomoku/ladder:supervisor",
        "//gomoku/ladder:worker_proto_cc",
        "@gflags//:gflags",
        "@grpc//:grpc++",
    ],
)
