get_filename_component(_TEST_NAME ${CMAKE_CURRENT_LIST_DIR} NAME)
add_executable(${_TEST_NAME}_tests ${_TEST_NAME}_tests_main.cpp)
target_link_libraries(${_TEST_NAME}_tests test_common libslic3r ${Boost_LIBRARIES} ${TBB_LIBRARIES} ${Boost_LIBRARIES})

# catch_discover_tests(${_TEST_NAME}_tests TEST_PREFIX "${_TEST_NAME}: ")
add_test(${_TEST_NAME}_tests ${_TEST_NAME}_tests "--durations yes")
