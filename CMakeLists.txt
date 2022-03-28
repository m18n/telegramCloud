cmake_minimum_required(VERSION 3.0.2)
project(TelegramCloud)
file(GLOB CPPS "*.cpp")

# set(CMAKE_CXX_STANDARD 11)
add_subdirectory(td)
add_executable(${PROJECT_NAME} ${CPPS})
target_link_libraries(${PROJECT_NAME} PRIVATE Td::TdStatic)

