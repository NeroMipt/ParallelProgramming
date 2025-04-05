cmake_minimum_required(VERSION 3.18)

#project(main)
set(TARGET_NAME "helloworld")

find_package(MPI REQUIRED)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS}")
add_executable(${TARGET_NAME} main.cpp)
target_link_libraries(cpp-rl-training PRIVATE  MPI::MPI_CXX  "${TORCH_LIBRARIES}" )
set_property(TARGET cpp-rl-training PROPERTY CXX_STANDARD 17)
target_link_libraries(${TARGET_NAME}
        PRIVATE
        MPI_C)