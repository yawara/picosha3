add_executable(test_sha3 test_sha3.cpp)
target_link_libraries(test_sha3 libgtest libgtest_main pthread)
#add_test(NAME Test224 COMMAND test_sha3)
add_test(NAME TestSHA3 COMMAND test_sha3)
