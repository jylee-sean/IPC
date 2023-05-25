cmake_minimum_required (VERSION 3.6.3)

project(boost_asio_practice)

set(CMAKE_CXX_STANDARD 17)

find_package(Boost)

if(Boost_FOUND)
	include_directories(${Boost_INCLUDE_DIRS})
endif()

add_executable(client2.out wall_client.cpp)
#add_executable(server2.out wall_server.cpp)
add_executable(server2.out asio_server.cpp)

# add_executable(client.out boost_example_client.cpp)
# add_executable(server.out boost_example_server.cpp)
