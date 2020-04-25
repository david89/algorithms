cc_library(
    name = "kmp",
    srcs = ["kmp.cpp"],
    hdrs = ["kmp.hpp"],
)

cc_test(
    name = "kmp_test",
    srcs = ["kmp_test.cpp"],
    deps = [
        ":kmp",
        "@gtest//:gtest_main",
    ],
)
