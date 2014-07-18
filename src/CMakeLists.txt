cmake_minimum_required(VERSION 2.8)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Ofast -fPIC")

aux_source_directory(./ SRC_LIST)
add_library(ee-fft STATIC ${SRC_LIST})
target_link_libraries(ee-fft m)

install(FILES ee-fft.h
        DESTINATION ${CMAKE_INSTALL_PREFIX}/include/)

install(TARGETS ee-fft ARCHIVE
        DESTINATION ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR})

aux_source_directory(./bench BENCH_SRC_LIST)
add_executable(bench-fft ${BENCH_SRC_LIST})
target_link_libraries(bench-fft ee-fft)
target_link_libraries(bench-fft m)

aux_source_directory(./gen GEN_SRC_LIST)
add_executable(genrev ${GEN_SRC_LIST})
target_link_libraries(genrev m)

