# configure executables for network test

add_executable(network_test_server server.cpp)
target_include_directories(network_test_server PUBLIC ${NEROSHOP_SOURCE_DIR})
target_link_libraries(network_test_server server)

add_executable(network_test_client client.cpp)
target_include_directories(network_test_client PUBLIC ${NEROSHOP_SOURCE_DIR})
target_link_libraries(network_test_client client)

# setup network test
# helpful: https://discourse.cmake.org/t/ctest-able-to-daemonize-fixtures/1012
add_test(NAME network_test_server COMMAND network_test_server)
add_test(NAME network_test_client COMMAND network_test_client)
add_test(NAME clean_network_test_server COMMAND network_test_server)

set_property(TEST network_test_server PROPERTY FIXTURES_SETUP daemon)
set_property(TEST clean_network_test_server PROPERTY FIXTURES_CLEANUP daemon)
set_tests_properties(network_test_client PROPERTIES FIXTURES_REQUIRED daemon)
