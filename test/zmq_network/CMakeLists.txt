# configure executables for network tests with zmq

add_test(NAME daemon_arg_help COMMAND ${DAEMON_EXECUTABLE} --help)
set_tests_properties(daemon_arg_help PROPERTIES
                     PASS_REGULAR_EXPRESSION "Show help message")

add_test(NAME daemon_arg_version COMMAND ${DAEMON_EXECUTABLE} --version)
set_tests_properties(daemon_arg_version PROPERTIES
                     PASS_REGULAR_EXPRESSION "${DAEMON_EXECUTABLE} v")

add_test(NAME daemon_arg_license COMMAND ${DAEMON_EXECUTABLE} --license)
set_tests_properties(daemon_arg_license PROPERTIES
                     PASS_REGULAR_EXPRESSION "Copyright.*Neroshop")

# start daemon in background, followed by a kill
add_executable(kill_daemon kill_daemon.cpp)
add_test(NAME daemon_detach COMMAND ${DAEMON_EXECUTABLE} --detach)
set_tests_properties(daemon_detach PROPERTIES
                     PASS_REGULAR_EXPRESSION "Forked PID")
add_test(NAME kill_daemon_detach COMMAND kill_daemon)
set_tests_properties(kill_daemon_detach PROPERTIES
                     PASS_REGULAR_EXPRESSION "Killing PID")
set_property(TEST daemon_detach PROPERTY FIXTURES_SETUP daemon_detach)
set_property(TEST kill_daemon_detach PROPERTY FIXTURES_CLEANUP daemon_detach)

# start daemon in background, connect from cli, kill daemon
add_test(NAME daemon_connect COMMAND ${DAEMON_EXECUTABLE} --detach --port 55091)
set_tests_properties(daemon_connect PROPERTIES
                     PASS_REGULAR_EXPRESSION "Forked PID")
file(TO_NATIVE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/connect.55091" _in)
add_test(NAME daemon_connect_cli
         COMMAND sh -c "$<TARGET_FILE:${CLI_EXECUTABLE}> < ${_in}")
set_tests_properties(daemon_connect_cli PROPERTIES
                     PASS_REGULAR_EXPRESSION "Connected")
add_test(NAME kill_daemon_connect COMMAND kill_daemon)
set_tests_properties(kill_daemon_connect PROPERTIES
                     PASS_REGULAR_EXPRESSION "Killing PID")
set_property(TEST daemon_connect PROPERTY FIXTURES_SETUP daemon_connect)
set_property(TEST kill_daemon_connect PROPERTY FIXTURES_CLEANUP daemon_connect)
set_tests_properties(daemon_connect_cli PROPERTIES FIXTURES_REQUIRED daemon_connect)
