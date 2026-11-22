# Copyright 2026, Elias Lopes & Rocklyn Clarke All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# * Redistributions of source code must retain the above copyright notice, this
#   list of conditions and the following disclaimer.
# * Redistributions in binary form must reproduce the above copyright notice,
#   this list of conditions and the following disclaimer in the documentation
#   and/or other materials provided with the distribution.
# * Neither the name of Elias Lopes & Rocklyn Clarke nor the names of its
#   contributors may be used to endorse or promote products derived from this
#   software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

cmake_minimum_required(VERSION 4.0)

project(DODProject)

# fetch all unittest source files
file(GLOB_RECURSE UNITEST_SRC CONFIGURE_DEPENDS
     "${CMAKE_CURRENT_SOURCE_DIR}/unittests/*.cpp")

if(NOT UNITEST_SRC)
  return()
endif()

add_executable(UnitTests ${UNITEST_SRC})

# add include directory path list
target_include_directories(UnitTests
                           PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

# search for GoogleTest library NOTE: export GTEST_ROOT="path/to/google/test" so
# we can use $GTEST_ROOT/include & $GTEST_ROOT/lib
if(NOT DEFINED ENV{GTEST_ROOT})
  message(FATAL_ERROR "GTEST_ROOT environment variable not set")
endif()

set(GTEST_ROOT "$ENV{GTEST_ROOT}")

# find GTEST headers
find_path(
  GTEST_INCLUDE_DIR gtest/gtest.h
  PATHS "${GTEST_ROOT}/include"
  NO_DEFAULT_PATH)

# find GTEST library
find_library(
  GTEST_LIBRARY
  NAMES gtest
  PATHS "${GTEST_ROOT}/lib"
  NO_DEFAULT_PATH)

find_library(
  GTEST_MAIN_LIBRARY
  NAMES gtest_main
  PATHS "${GTEST_ROOT}/lib"
  NO_DEFAULT_PATH)

# add GTEST include path
target_include_directories(UnitTests PRIVATE "${GTEST_INCLUDE_DIR}")

# link GTEST library
target_link_libraries(UnitTests PRIVATE "${GTEST_LIBRARY}"
                                        "${GTEST_MAIN_LIBRARY}")
