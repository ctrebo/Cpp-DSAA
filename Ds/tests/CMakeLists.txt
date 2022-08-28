SET(test_files
    ${CMAKE_CURRENT_SOURCE_DIR}/list_test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/hashtable_test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/hashtablewllist_test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/bst_test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/vector_test.cpp
)

add_executable(tests ${test_files})

find_package(Catch2 3 REQUIRED)
if(Catch2_FOUND) 
    target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)
    include(CTest)
    include(Catch)
    catch_discover_tests(tests)
endif()
target_link_libraries(tests PUBLIC Ds)

find_package(fmt REQUIRED)
if(fmt_FOUND)
    include_directories( ${fmt_INCLUDE_DIRS} )
    target_link_libraries( tests PRIVATE fmt::fmt )
endif()
