#
# Copyright 2025 René Widera, Simeon Ehrig
# SPDX-License-Identifier: ISC
#

function(add_test_target target_prefix folder)
    message(DEBUG "test target prefix: ${target_prefix}")

    file(GLOB test_folders ${folder})
    foreach (test_folder ${test_folders})
        if (IS_DIRECTORY ${test_folder})
            message(DEBUG "test folder: ${test_folder}")

            file(GLOB test_sources "${test_folder}/**")

            # iterate over all sub folders
            foreach (source_file ${test_sources})
                # the include folder is a special case, therefore ignore it
                if (source_file STREQUAL "${test_folder}/include")
                    continue()
                endif ()

                get_filename_component(source_file_extension ${source_file} EXT)
                get_filename_component(source_file_name ${source_file} NAME_WE)

                if ("${source_file_extension}" STREQUAL ".cpp")
                    set(_test_target_name "${target_prefix}${source_file_name}")
                    message(DEBUG "  add test target ${_test_target_name}: ${source_file}")

                    add_executable(${_test_target_name} ${source_file})
                    get_filename_component(src_dir ${source_file} DIRECTORY)
                    target_include_directories(${_test_target_name} PRIVATE ${src_dir})
                    if (EXISTS "${test_folder}/include")
                        message(STATUS "  add include folder: ${test_folder}/include")
                        target_include_directories(${_test_target_name} PRIVATE ${test_folder}/include)
                    endif ()

                    target_link_libraries(${_test_target_name} PRIVATE Catch2::Catch2WithMain headers)
                    set_target_properties(${_test_target_name} PROPERTIES
                            CXX_STANDARD 20
                    )
                    catch_discover_tests(${_test_target_name})
                endif ()
            endforeach ()
        endif ()
    endforeach ()
endfunction()