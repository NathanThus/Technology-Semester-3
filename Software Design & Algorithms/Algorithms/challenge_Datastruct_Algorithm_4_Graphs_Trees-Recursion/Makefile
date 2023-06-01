# EXE file name
PRODUCT=Build

# Compiler choice
CC=g++

# Files to compile (all files in folder are used that match the extension)
# $(wildcard path/*.<extension>)
SOURCES=$(wildcard Src/*.cpp)
HEADERS=$(wildcard Src/*.hpp)

# Compile flags
CFLAGS=-Wall -Werror -Wextra -pedantic -ggdb -O0 -std=c++17

$(PRODUCT): $(SOURCES) $(HEADERS)
	@$(CC) $(CFLAGS) $(SOURCES) -o $(PRODUCT)

# Cleans EXE (windows only)
# clean:
#     -del $(PRODUCT).exe

# Cleans EXE (linux only)
clean:
	@rm -rf $(PRODUCT)