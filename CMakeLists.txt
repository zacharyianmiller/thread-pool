cmake_minimum_required(VERSION 3.10)

set(PROJECT_NAME thread-pool)
project(${PROJECT_NAME} LANGUAGES CXX)

add_executable(${PROJECT_NAME}
    main.cpp
    ThreadPool.cpp
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)
