CC = clang
INCLUDE = -I../kgfx/include -I../kgfx/vendor/include
OS = $(shell uname)
GLFW_LIB =

ifeq ($(OS),Windows_NT)
GLFW_LIB = -L../kgfx/vendor/vs/x64 -lglfw3
else ifeq ($(OS),Darwin)
GLFW_LIB = -L../kgfx/vendor/mac -lglfw3 -framework IOKit -framework Cocoa
else
GLFW_LIB = -lglfw
endif

LIB = -L../build -lkgfx -lkgfx_gh $(GLFW_LIB)
CCFLAGS = -std=c99 -Wall -Wextra -Werror -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-unused-private-field $(INCLUDE)

BUILD_DIR = build
OBJ_DIR = obj

copy-dependencies:
	cp -r ../build/ ../samples/build/

kgfx_triangle: $(OBJ_DIR)/triangle/triangle.o
	@echo "|-> Linking $@"
	@mkdir -p $(dir $@) $(BUILD_DIR)
	$(CC) $(CCFLAGS) $^ -o $(BUILD_DIR)/$@ $(LIB)

clean:
	@rm -rf $(BUILD_DIR) $(OBJ_DIR)

$(OBJ_DIR)/%.o: %.c
	@echo "|-> Compiling $< to $@"
	@mkdir -p $(dir $@)
	$(CC) $(CCFLAGS) -c $< -o $@
