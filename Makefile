CXX = clang++
CC = clang

INCLUDE = -Ikgfx/include -Ikgfx/vendor/include

CXXFLAGS = -std=c++14 -Wall -Wextra -Werror -Wno-unused-parameter -Wno-unused-variable -Wno-unused-private-field -Wno-unused-function $(INCLUDE)
CCFLAGS = -std=c99 -Wall -Wextra -Werror -Wno-unused-parameter -Wno-unused-variable -Wno-unused-private-field -Wno-unused-function $(INCLUDE)
MMFLAGS = -fobjc-arc

KGFX_SRC_DIR = kgfx/src/kgfx
KGFX_SRC_METAL = $(KGFX_SRC_DIR)/kgfx_metal.mm
KGFX_SRC_D3D12 = $(KGFX_SRC_DIR)/kgfx_d3d12.cpp
KGFX_SRC_VULKAN = $(KGFX_SRC_DIR)/kgfx_vulkan.cpp
KGFX_SRC_DUMMY = $(KGFX_SRC_DIR)/kgfx_dummy.c

OBJ_DIR = obj
BUILD_DIR = build

KGFX_OBJ_METAL = $(OBJ_DIR)/$(subst .mm,.o,$(KGFX_SRC_METAL))
KGFX_OBJ_D3D12 = $(OBJ_DIR)/$(subst .cpp,.o,$(KGFX_SRC_D3D12))
KGFX_OBJ_VULKAN = $(OBJ_DIR)/$(subst .cpp,.o,$(KGFX_SRC_VULKAN))
KGFX_OBJ_DUMMY = $(OBJ_DIR)/$(subst .cpp,.o,$(KGFX_SRC_DUMMY))

.PHONY: build-metal-dylib obj-metal-dylib link-metal-dylib clean make-vars

clean:
	rm -rf $(OBJ_DIR) $(BUILD_DIR)

make-vars:
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

build-metal-dylib: obj-metal-dylib link-metal-dylib

obj-metal-dylib: $(KGFX_OBJ_METAL)

link-metal-dylib:
	mkdir -p $(BUILD_DIR)
	$(CXX) -dynamiclib -o $(BUILD_DIR)/kgfx_metal.dylib $(KGFX_OBJ_METAL) -lobjc -framework Metal -framework Cocoa -framework QuartzCore

$(OBJ_DIR)/%.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: %.mm
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(MMFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: %.m
	mkdir -p $(dir $@)
	$(CC) $(CCFLAGS) $(MMFLAGS) -c $< -o $@
