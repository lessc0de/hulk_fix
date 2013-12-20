
add_executable( benchmark_decoder benchmark_decoder.cpp )
add_executable( benchmark_encoder benchmark_encoder.cpp )
add_executable( benchmark_tcp benchmark_tcp.cpp )

target_link_libraries( benchmark_decoder hulk_core hulk_fix pthread )
target_link_libraries( benchmark_encoder hulk_core hulk_fix pthread )
target_link_libraries( benchmark_tcp hulk_core hulk_fix pthread )
