# Copyright 2026, Rocklyn Clarke & Elias Lopes
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met:
#
#     * Redistributions of source code must retain the above copyright
# notice, this list of conditions and the following disclaimer.
#     * Redistributions in binary form must reproduce the above
# copyright notice, this list of conditions and the following disclaimer
# in the documentation and/or other materials provided with the
# distribution.
#     * Neither the name of Rocklyn Clarke & Elias Lopes nor the names of its
# contributors may be used to endorse or promote products derived from
# this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
# FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
# COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
# BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
# ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.


CXX 		   = 
CFLAGS 		   = -Wall --std=c++17
DEBUGGER_FLAGS =

# PROJECT PATHS
BUILD_DIR = ./build
SRC_DIR   = ./src

MAIN_EXE = $(BUILD_DIR)/bin/main

# CHECK PLATFORM TO CHANGE COMPILER
UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Darwin)
    CXX = clang++
else ifeq ($(UNAME_S),Linux)
    CXX = gcc
else ifeq ($(OS),Windows_NT)
    CXX = gcc
else
    $(error Unsupported operating system: $(UNAME_S))
endif


# INCLUDE SEARCH PATH
INCLUDE_SEARCH_PATH = -Iinclude

# SOURCE & OBJECT FILES
SRC_FILES = $(shell find ./src -type f -name '*.cpp' )
OBJ_FILES = $(patsubst %.cpp , $(BUILD_DIR)/obj/%.o, $(SRC_FILES))

# BUILD TARGETS
.PHONY: swap-debug-compiler 
swap-debug-compiler : 
	CXX = cl

.PHONY: main
main: build-dir $(MAIN_EXE)

.PHONY: debug
debug: build-dir swap-debug-compiler $(MAIN_EXE)

.PHONY: run
run: build-dir $(MAIN_EXE)
	$(MAIN_EXE)

.PHONY: rund
rund: debug 
	$(MAIN_EXE)

.PHONY: all 
all : main

# LINK OBJECTS TO EXE
$(MAIN_EXE): $(OBJ_FILES)
	$(CXX) $(CFLAGS) $(DEBUGGER_FLAGS) $(INCLUDE_SEARCH_PATH) $^ -o $@

# CONVERT SRC TO OBJ
$(BUILD_DIR)/obj/%.o : $(SRC_DIR)/%.cpp
	$(CXX) $(CFLAGS) $(DEBUGGER_FLAGS) $(INCLUDE_SEARCH_PATH) \
	-c $< -o $@

build-dir:
	@mkdir -p $(BUILD_DIR)/bin
	@mkdir -p $(BUILD_DIR)/obj

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)
	$(MAKE) build-dir

