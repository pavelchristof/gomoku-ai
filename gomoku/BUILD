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
