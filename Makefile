CXX = clang++
CC = clang

INCLUDE = -Ikgfx/include -Ikgfx/vendor/include
DEFINES = -DKGFX_DEBUG

CXXFLAGS = -std=c++14 -Wall -Wextra -Werror -Wno-unused-parameter -Wno-unused-variable -Wno-unused-private-field -Wno-unused-function $(INCLUDE) $(DEFINES)
CCFLAGS = -std=c99 -Wall -Wextra -Werror -Wno-unused-parameter -Wno-unused-variable -Wno-unused-private-field -Wno-unused-function $(INCLUDE) $(DEFINES)
MMFLAGS = -fobjc-arc

KGFX_SRC_DIR = kgfx/src/kgfx
KGFX_VENDOR = kgfx/vendor

KGFX_SRC_METAL = $(KGFX_SRC_DIR)/kgfx_metal.mm
KGFX_SRC_D3D12 = $(KGFX_SRC_DIR)/kgfx_d3d12.cpp
KGFX_SRC_VULKAN = $(KGFX_SRC_DIR)/kgfx_vulkan.cpp
KGFX_SRC_DUMMY = $(KGFX_SRC_DIR)/kgfx_dummy.c

OBJ_DIR = obj
BUILD_DIR = build

KGFX_OBJ_METAL = $(OBJ_DIR)/$(subst .mm,.o,$(KGFX_SRC_METAL))
KGFX_OBJ_D3D12 = $(OBJ_DIR)/$(subst .cpp,.o,$(KGFX_SRC_D3D12))
KGFX_OBJ_VULKAN = $(OBJ_DIR)/$(subst .cpp,.o,$(KGFX_SRC_VULKAN))
KGFX_OBJ_DUMMY = $(OBJ_DIR)/$(subst .c,.o,$(KGFX_SRC_DUMMY))

OS = $(shell uname)

STATIC_LIB_EXT = a
DYNAMIC_LIB_EXT = dylib

DYNAMIC_LINK_FLAG = -dynamiclib

STATIC_LINK_FLAGS_METAL = -framework Metal -framework Cocoa -framework QuartzCore
DYNAMIC_LINK_FLAGS_METAL = -lobjc -framework Metal -framework Cocoa -framework QuartzCore

STATIC_LINK_FLAGS_VULKAN = -lvulkan
DYNAMIC_LINK_FLAGS_VULKAN = -lvulkan

KGFXGH_LINK_FLAGS = -lglfw3

ifeq ($(OS),Windows_NT)
	STATIC_LIB_EXT = lib
	DYNAMIC_LIB_EXT = dll

	KGFXGH_LINK_FLAGS = -lglfw3
else ifeq ($(OS),Darwin)
	STATIC_LIB_EXT = a
	DYNAMIC_LIB_EXT = dylib

	STATIC_LINK_FLAGS_VULKAN = -L$(KGFX_VENDOR)/mac -lMoltenVK
	DYNAMIC_LINK_FLAGS_VULKAN = -L$(KGFX_VENDOR)/mac -lMoltenVK

	DYNAMIC_LINK_FLAG = -dynamic

	KGFXGH_LINK_FLAGS = -lglfw3
else
	STATIC_LIB_EXT = a
	DYNAMIC_LIB_EXT = so

	STATIC_LINK_FLAGS_VULKAN = -lvulkan
	DYNAMIC_LINK_FLAGS_VULKAN = -libkgfx_vulkan
	
	DYNAMIC_LINK_FLAG = -shared -fPIC

	KGFXGH_LINK_FLAGS = -lglfw
endif


.PHONY: obj-metal build-metal-static link-metal-static build-metal-dylib link-metal-dylib obj-dummy build-dummy-static link-dummy-static build-dummy-dynamic link-dummy-dynamic obj-vulkan build-vulkan-static link-vulkan-static build-vulkan-dynamic link-vulkan-dynamic clean make-vars

make-vars:
	@echo "Makefile variables debug:"
	@echo "CXX: " $(CXX)
	@echo "CC: " $(CC)

	@echo "CXXFLAGS: " $(CXXFLAGS)
	@echo "CCFLAGS: " $(CCFLAGS)
	@echo "MMFLAGS: " $(MMFLAGS)

	@echo "KGFX_SRC_METAL: " $(KGFX_SRC_METAL)
	@echo "KGFX_SRC_D3D12: " $(KGFX_SRC_D3D12)
	@echo "KGFX_SRC_VULKAN: " $(KGFX_SRC_VULKAN)
	@echo "KGFX_SRC_DUMMY: " $(KGFX_SRC_DUMMY)

	@echo "OBJ_DIR: " $(OBJ_DIR)
	@echo "BUILD_DIR: " $(BUILD_DIR)

	@echo "KGFX_OBJ_METAL: " $(KGFX_OBJ_METAL)
	@echo "KGFX_OBJ_D3D12: " $(KGFX_OBJ_D3D12)
	@echo "KGFX_OBJ_VULKAN: " $(KGFX_OBJ_VULKAN)
	@echo "KGFX_OBJ_DUMMY: " $(KGFX_OBJ_DUMMY)

clean:
	rm -rf $(OBJ_DIR) $(BUILD_DIR)

build-metal-static: obj-metal link-metal-static

link-metal-static:
	@echo "| Linking libkgfx_metal.a"
	@mkdir -p $(BUILD_DIR)
	ar rcs $(BUILD_DIR)/libkgfx_metal.a $(KGFX_OBJ_METAL)

build-metal-dylib: obj-metal link-metal-dylib

obj-metal: $(KGFX_OBJ_METAL)

link-metal-dylib:
	@echo "| Linking libkgfx_metal.dylib"
	@mkdir -p $(BUILD_DIR)
	$(CXX) -dynamiclib -o $(BUILD_DIR)/libkgfx_metal.dylib $(KGFX_OBJ_METAL) -lobjc -framework Metal -framework Cocoa -framework QuartzCore

obj-dummy: $(KGFX_OBJ_DUMMY)

build-dummy-static: obj-dummy link-dummy-static

link-dummy-static:
	@echo "| Linking libkgfx_dummy.a"
	@mkdir -p $(BUILD_DIR)
	ar rcs $(BUILD_DIR)/libkgfx_dummy.a $(KGFX_OBJ_DUMMY)

build-dummy-dynamic: obj-dummy link-dummy-dynamic

link-dummy-dynamic:
	@echo "| Linking libkgfx_dummy.dylib"
	@mkdir -p $(BUILD_DIR)
	$(CC) $(DYNAMIC_LINK_FLAG) -o $(BUILD_DIR)/libkgfx_dummy.dylib $(KGFX_OBJ_DUMMY)

obj-vulkan: $(KGFX_OBJ_VULKAN)

build-vulkan-static: obj-vulkan link-vulkan-static

link-vulkan-static:
	@echo "| Linking libkgfx_vulkan.$(STATIC_LIB_EXT)"
	@mkdir -p $(BUILD_DIR)
	ar rcs $(BUILD_DIR)/libkgfx_vulkan.$(STATIC_LIB_EXT) $(KGFX_OBJ_VULKAN)
	
build-vulkan-dynamic: obj-vulkan link-vulkan-dynamic	

link-vulkan-dynamic:
	@echo "| Linking libkgfx_vulkan.$(DYNAMIC_LIB_EXT)"
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(DYNAMIC_LINK_FLAG) -o $(BUILD_DIR)/libkgfx_vulkan.$(DYNAMIC_LIB_EXT) $(KGFX_OBJ_VULKAN) $(DYNAMIC_LINK_FLAGS_VULKAN)

$(OBJ_DIR)/%.o: %.cpp
	@echo "|-> Compiling $< to $@"
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: %.mm
	@echo "|-> Compiling $< to $@"
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(MMFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: %.c
	@echo "|-> Compiling $< to $@"
	@mkdir -p $(dir $@)
	$(CC) $(CCFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: %.m
	@echo "|-> Compiling $< to $@"
	@mkdir -p $(dir $@)
	$(CC) $(CCFLAGS) $(MMFLAGS) -c $< -o $@
