#
#   Copyright 2016 Lorenzo Musto
#
#   Licensed under the Apache License, Version 2.0 (the "License");
#   you may not use this file except in compliance with the License.
#   You may obtain a copy of the License at
#
#       http://www.apache.org/licenses/LICENSE-2.0
#
#   Unless required by applicable law or agreed to in writing, software
#   distributed under the License is distributed on an "AS IS" BASIS,
#   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#   See the License for the specific language governing permissions and
#   limitations under the License.
#
cmake_minimum_required( VERSION 3.0 )

project( cmn )

# We need thread support
find_package( Threads REQUIRED )

# Enable ExternalProject CMake module
include( ExternalProject )
# Download and install GoogleTest
ExternalProject_Add(
  gtest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG master # Git branch name, commit id or tag
  PREFIX ${CMAKE_CURRENT_BINARY_DIR}/gtest
  #URL https://github.com/google/googletest/archive/master.zip
  CMAKE_ARGS
    -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}   ## same as master project
    -DBUILD_SHARED_LIBS=${BUILD_SHARED_LIBS} ## same as master project
    -DCMAKE_CXX_FLAGS_DEBUG=${CMAKE_CXX_FLAGS_DEBUG}
  # Disable install step
  INSTALL_COMMAND ""
  # If you don't want the update step to pull changes from the repository
  UPDATE_COMMAND ""
  )

# Set gtest properties
# source_dir=/home/lm/src/cmake/tw/build/gtest/src/gtest
# binary_dir=/home/lm/src/cmake/tw/build/gtest/src/gtest-build
ExternalProject_Get_Property( gtest source_dir binary_dir )

# Create a libgtest target to be used as a dependency by test programs
add_library( libgtest IMPORTED STATIC GLOBAL )
add_dependencies( libgtest gtest )
set_target_properties( libgtest PROPERTIES
  "IMPORTED_LOCATION" "${binary_dir}/googlemock/gtest/libgtest.a"
  "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
  #"INTERFACE_INCLUDE_DIRECTORIES" "${source_dir}/include"
)

add_library( libgmock IMPORTED STATIC GLOBAL )
add_dependencies( libgmock gtest )
set_target_properties( libgmock PROPERTIES
  "IMPORTED_LOCATION" "${binary_dir}/googlemock/libgmock.a"
  "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
)

# I couldn't make it work with INTERFACE_INCLUDE_DIRECTORIES
include_directories( "${source_dir}/googletest/include" )
include_directories( "${source_dir}/googlemock/include" )

add_subdirectory( src )

# Enable testing
include(CTest)

add_test( test_lib_Runs src/lib/test/test_lib )
