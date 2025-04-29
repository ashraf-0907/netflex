# cmake_minimum_required(VERSION 3.15)
# project(NetFlex)

# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

# # OpenSSL required
# find_package(OpenSSL REQUIRED)

# # === Source files ===
# file(GLOB_RECURSE NETFLEX_SRC
#     src/network/**/*.cpp
#     src/core/**/*.cpp
#     wrapper/*.cpp                         # ✅ Wrapper implementations
#     common/**/*.cpp
# )

# # === Include directories ===
# include_directories(
#     ${OPENSSL_INCLUDE_DIR}
#     ${CMAKE_SOURCE_DIR}/include
#     ${CMAKE_SOURCE_DIR}/wrapper
#     ${CMAKE_SOURCE_DIR}/src
#     ${CMAKE_SOURCE_DIR}/common
# )

# # === Library ===
# add_library(netflex_lib STATIC ${NETFLEX_SRC})
# target_link_libraries(netflex_lib PRIVATE ${OPENSSL_LIBRARIES})

# # === ✅ Separate executables for testing ===

# # TCP Server Test
# add_executable(tcp_server_test tests/tcp-tests/server.cpp)
# target_link_libraries(tcp_server_test PRIVATE netflex_lib)

# # TCP Client Test
# add_executable(tcp_client_test tests/tcp-tests/client.cpp)
# target_link_libraries(tcp_client_test PRIVATE netflex_lib)

# add_executable(udp_client_test tests/udp-tests/client.cpp)
# target_link_libraries(udp_client_test PRIVATE netflex_lib)

# add_executable(udp_server_test tests/udp-tests/server.cpp)
# target_link_libraries(udp_server_test PRIVATE netflex_lib)

# # SSL Server Test
# add_executable(ssl_server_test tests/ssl-tests/server.cpp)
# target_link_libraries(ssl_server_test PRIVATE netflex_lib)

# # SSL Client Test
# add_executable(ssl_client_test tests/ssl-tests/client.cpp)
# target_link_libraries(ssl_client_test PRIVATE netflex_lib)

# add_executable(threaded_tcp_test tests/threaded-tests/tcp-server.cpp)
# target_link_libraries(threaded_tcp_test PRIVATE netflex_lib)

# add_executable(threaded_ssl_test tests/threaded-tests/ssl-server.cpp)
# target_link_libraries(threaded_ssl_test PRIVATE netflex_lib)




# # === Info ===
# message(STATUS "OpenSSL Include Dir: ${OPENSSL_INCLUDE_DIR}")
# message(STATUS "OpenSSL Libraries: ${OPENSSL_LIBRARIES}")


cmake_minimum_required(VERSION 3.15)
project(NetFlex)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# === Dependencies ===
find_package(OpenSSL REQUIRED)

# === Include Directories ===
include_directories(
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/common
    ${CMAKE_SOURCE_DIR}/wrapper
    ${CMAKE_SOURCE_DIR}/include
    ${OPENSSL_INCLUDE_DIR}
)

# === Collect Source Files ===
file(GLOB_RECURSE NETFLEX_SRC
    src/*.cpp
    wrapper/*.cpp
    common/*.cpp
)

# === Create Static Library ===
add_library(netflex_lib STATIC ${NETFLEX_SRC})
target_link_libraries(netflex_lib PRIVATE ${OPENSSL_LIBRARIES})

# === Test Executables ===
add_executable(tcp_server_test tests/tcp-tests/server.cpp)
target_link_libraries(tcp_server_test PRIVATE netflex_lib)

add_executable(tcp_client_test tests/tcp-tests/client.cpp)
target_link_libraries(tcp_client_test PRIVATE netflex_lib)

add_executable(udp_client_test tests/udp-tests/client.cpp)
target_link_libraries(udp_client_test PRIVATE netflex_lib)

add_executable(udp_server_test tests/udp-tests/server.cpp)
target_link_libraries(udp_server_test PRIVATE netflex_lib)

add_executable(ssl_server_test tests/ssl-tests/server.cpp)
target_link_libraries(ssl_server_test PRIVATE netflex_lib)

add_executable(ssl_client_test tests/ssl-tests/client.cpp)
target_link_libraries(ssl_client_test PRIVATE netflex_lib)

add_executable(threaded_tcp_test tests/threaded-tests/tcp-server.cpp)
target_link_libraries(threaded_tcp_test PRIVATE netflex_lib)

add_executable(threaded_ssl_test tests/threaded-tests/ssl-server.cpp)
target_link_libraries(threaded_ssl_test PRIVATE netflex_lib)

# === Info ===
message(STATUS "OpenSSL Include Dir: ${OPENSSL_INCLUDE_DIR}")
message(STATUS "OpenSSL Libraries: ${OPENSSL_LIBRARIES}")
