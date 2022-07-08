# configure executables for network test with system sockets

add_executable(network_test_server server.cpp)
target_include_directories(network_test_server PUBLIC ${NEROSHOP_SOURCE_DIR})
target_link_libraries(network_test_server server)

add_executable(network_test_client client.cpp)
target_include_directories(network_test_client PUBLIC ${NEROSHOP_SOURCE_DIR})
target_link_libraries(network_test_client client)

# setup network test: start server, start client connect to server, kill server
# helpful: https://discourse.cmake.org/t/ctest-able-to-daemonize-fixtures/1012
add_test(NAME network_test_server COMMAND network_test_server)
add_test(NAME network_test_client COMMAND network_test_client)
add_test(NAME kill_network_test_server COMMAND network_test_server)

set_property(TEST network_test_server PROPERTY FIXTURES_SETUP sys_daemon)
set_property(TEST kill_network_test_server PROPERTY FIXTURES_CLEANUP sys_daemon)
set_tests_properties(network_test_client PROPERTIES FIXTURES_REQUIRED sys_daemon)
