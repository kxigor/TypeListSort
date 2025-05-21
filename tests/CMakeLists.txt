find_package(PkgConfig REQUIRED)
find_package(GTest REQUIRED)

function(add_google_test TEST_NAME TEST_SRC)
    add_executable(${TEST_NAME} ${TEST_SRC})
    target_include_directories(${TEST_NAME} SYSTEM PUBLIC
        Threads::Threads
        ${GTEST_INCLUDE_DIRS}
        ${GMOCK_INCLUDE_DIRS}
    )
    set_target_properties(${TEST_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
    )
    target_link_libraries(${TEST_NAME} ${GTEST_LIBRARIES})
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endfunction()

add_google_test(
    TypeListTests 
    TypeListTests.cpp
)

add_google_test(
    TypeListQuckSortTests 
    TypeListQuckSortTests.cpp
)

add_google_test(
    TypesPredicatesTests
    TypesPredicatesTests.cpp
)