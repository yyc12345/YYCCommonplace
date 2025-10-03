# Create executable benchmark
add_executable(YYCCBenchmark "")
# Setup test sources
target_sources(YYCCBenchmark
PRIVATE
    main.cpp

    yycc/string/op.cpp

    yycc/carton/fft.cpp
)
# target_sources(YYCCBenchmark
# PRIVATE
# FILE_SET HEADERS
# FILES
#     shared/literals.hpp
# )
# Setup headers
target_include_directories(YYCCBenchmark
PUBLIC
    "${CMAKE_CURRENT_LIST_DIR}"
)
# Setup libraries
target_link_libraries(YYCCBenchmark
PRIVATE
    YYCCommonplace
    benchmark::benchmark
)
