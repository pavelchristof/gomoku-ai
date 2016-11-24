cc_binary(
    name = "worker",
    srcs = ["worker.cc"],
    deps = [
        "//gomoku/actors:all_actors",
        "//gomoku/supervisor:worker_impl",
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
        "//gomoku/supervisor:supervisor",
        "//gomoku/supervisor:worker_proto_cc",
        "@gflags//:gflags",
        "@grpc//:grpc++",
    ],
)
