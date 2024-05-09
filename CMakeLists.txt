cmake_minimum_required(VERSION 3.28)
project(DiscordBot C)

include_directories(/usr/local/opt/curl/include/)

set(CMAKE_CXX_STANDARD 17)

set(CURL_LIBRARY "-lcurl -lm")

find_package(CURL REQUIRED)
find_package(Libwebsockets REQUIRED)

add_executable(DiscordBot main.c
        cJSON/cJSON.c
        cJSON/cJSON.h
)

include_directories(${CURL_INCLUDE_DIR})
include_directories(${LIBWEBSOCKETS_INCLUDE_DIRS})

target_link_libraries(DiscordBot ${CURL_LIBRARIES})
target_link_libraries(DiscordBot ${LIBWEBSOCKETS_LIBRARIES})