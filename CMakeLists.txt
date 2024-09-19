cmake_minimum_required(VERSION 3.10)

#############################
############ FST ############
#############################

project(fst)

    #############################
    ########## SOURCES ##########
    #############################

    file(GLOB_RECURSE   sources           src/fst.cpp src/classes/*.cpp src/classes/*.h src/helpers/*.cpp src/helpers/*.h)
    file(GLOB_RECURSE   sources_tests     src/classes/*.cpp src/classes/*.h src/helpers/*.cpp src/helpers/*.h src/tests/main_test.cpp src/tests/classes/*.cpp src/tests/helpers/*.cpp)

    #############################
    ########## TARGETS ########## 
    #############################

    # Main program
    add_executable(fst ${sources})
    target_compile_options(fst PUBLIC -std=c++17 -Wall)
    target_include_directories(fst PUBLIC src)

    # Test program
    add_executable(fst_tests ${sources_tests})
    target_compile_options(fst_tests PUBLIC -std=c++17 -Wall)
    target_include_directories(fst_tests PUBLIC src)

    # Enable testing
    enable_testing()
    add_test(NAME fst_tests COMMAND fst_tests)


#############################
####### FSST WITH FST #######
#############################

project(fsst_with_fst)

    #################################################
    ## MODIFIED VERSION OF THE FSST CMAKELISTS.TXT ##
    #################################################

    find_package(Threads REQUIRED)

    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_EXTENSIONS OFF)

    set(CMAKE_VERBOSE_MAKEFILE ON)

    include(CheckCXXCompilerFlag)
    check_cxx_compiler_flag("-march=native" COMPILER_SUPPORTS_MARCH_NATIVE)
    if(COMPILER_SUPPORTS_MARCH_NATIVE)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
    endif()

    if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
    endif()

    if(NOT MSVC)
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -DNDEBUG")
    else()
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O3 /DNDEBUG")
    endif()

    if(CMAKE_BUILD_TYPE STREQUAL Release)
    set_source_files_properties( 
        fsst_avx512.cpp 
        PROPERTIES
        COMPILE_FLAGS -O1
    )
    endif()

    file(GLOB_RECURSE   sources           src/classes/*.cpp src/classes/*.h src/helpers/*.cpp src/helpers/*.h)
    add_library(fsst ${sources} src/fsst/libfsst.cpp lib/fsst/libfsst.cpp)
    add_executable(fsst_with_fst src/fsst/fsst.cpp)
    target_link_libraries (fsst_with_fst LINK_PUBLIC fsst)
    target_link_libraries (fsst_with_fst LINK_PUBLIC Threads::Threads)
    set_target_properties(fsst_with_fst PROPERTIES OUTPUT_NAME fsst_with_fst)

#############################
##### FSST WITHOUT FST ######
#############################

project(fsst_without_fst)

    #################################################
    ## MODIFIED VERSION OF THE FSST CMAKELISTS.TXT ##
    #################################################

    cmake_minimum_required(VERSION 3.0)

    find_package(Threads REQUIRED)

    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_EXTENSIONS OFF)

    set(CMAKE_VERBOSE_MAKEFILE ON)

    include(CheckCXXCompilerFlag)
    check_cxx_compiler_flag("-march=native" COMPILER_SUPPORTS_MARCH_NATIVE)
    if(COMPILER_SUPPORTS_MARCH_NATIVE)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
    endif()

    if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
    endif()

    if(NOT MSVC)
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -DNDEBUG")
    else()
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O3 /DNDEBUG")
    endif()

    if(CMAKE_BUILD_TYPE STREQUAL Release)
    set_source_files_properties( 
        fsst_avx512.cpp 
        PROPERTIES
        COMPILE_FLAGS -O1
    )
    endif()

    add_library(fsst_wo_fst lib/fsst/libfsst.cpp)
    add_executable(fsst_without_fst lib/fsst/fsst.cpp)
    target_link_libraries (fsst_without_fst LINK_PUBLIC fsst_wo_fst)
    target_link_libraries (fsst_without_fst LINK_PUBLIC Threads::Threads)
    set_target_properties(fsst_without_fst PROPERTIES OUTPUT_NAME fsst_without_fst)

