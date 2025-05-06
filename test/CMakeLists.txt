if (WIN32)
    # Patch CMake's 'add_test' to extend the DLL search path on Windows. Use the
    # target name associated with the test, if it is also a CMake target.

    macro(add_test)
        _add_test(${ARGV})

        cmake_parse_arguments("" "" "NAME" "" ${ARGV})
        set(DLL_DIRS $<TARGET_RUNTIME_DLL_DIRS:${_NAME}>)
        set_tests_properties(${_NAME}
            PROPERTIES ENVIRONMENT
            "PATH=$<JOIN:$<SHELL_PATH:${DLL_DIRS}>,\\;>")
    endmacro()
endif()

add_executable(testGfHardToReach testGfHardToReach.cpp)
target_link_libraries(testGfHardToReach PUBLIC gf)
add_test(NAME testGfHardToReach COMMAND testGfHardToReach)

add_executable(testGfColorCpp testGfColorCpp.cpp)
target_link_libraries(testGfColorCpp PUBLIC gf)
add_test(NAME testGfColorCpp COMMAND testGfColorCpp)

if(BUILD_PYTHON_BINDINGS)
    pytest_discover_tests(
        testPyGf
        LIBRARY_PATH_PREPEND
            $<TARGET_FILE_DIR:gf>
            $<TARGET_FILE_DIR:pyGf>
            $<TARGET_FILE_DIR:pxr::tf>
            $<TARGET_FILE_DIR:pxr::pyTf>
        PYTHON_PATH_PREPEND
            "$<TARGET_FILE_DIR:pyGf>/../.."
            "$<TARGET_FILE_DIR:pxr::pyTf>/../.."
        TRIM_FROM_NAME "^(TestGf|test_)"
        EXTRA_ARGS "-v"
        DEPENDS gf pyGf
    )
endif()
