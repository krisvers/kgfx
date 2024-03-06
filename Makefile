FLAGS = -Ilib/include -Iinclude -Wno-switch -Wno-deprecated-declarations -Wno-unused-function

MAC_LIBS = -Llib/mac -lglfw3 -framework IOKit -framework Cocoa -framework QuartzCore -lMoltenVK
LINUX_LIBS = -lglfw -lvulkan -lassimp

linux-vulkan-examples: linux-vulkan-static
	clang -c src/main.c $(shell find src -type f -name "example*.c") src/kgfx_gh/kgfx_gh_xlib.c $(FLAGS) $(EXTRA)
	mv *.o obj/
	clang++ obj/*.o $(LINUX_LIBS) $(EXTRA) -Lbuild -l:libkgfx.a -lm -o kgfx-examples

linux-vulkan-static:
	clang++ -static -c src/kgfx/kgfx_vulkan.cpp $(FLAGS) $(LINUX_LIBS) $(EXTRA) -o obj/kgfx_vulkan.o
	ar rcs build/libkgfx.a obj/kgfx_vulkan.o

linux-vulkan-dynamic:
	clang++ -shared -fPIC src/kgfx/kgfx_vulkan.cpp $(FLAGS) $(LINUX_LIBS) $(EXTRA) -o build/libkgfx.so

mac-vulkan:
	clang -std=c99 -c src/main.c $(shell find src -type f -name "example*.c") $(FLAGS)
	mv *.o obj/
	clang -c src/kgfx_gh/kgfx_gh_cocoa.m -o obj/kgfx_gh_cocoa.o $(FLAGS)
	clang++ -std=c++14 -c src/kgfx/kgfx_vulkan.cpp -o obj/kgfx_vulkan.o $(FLAGS) -Wno-switch
	# clang++ obj/*.o -o kgfx -rpath lib/mac $(MAC_LIBS)

mac-vulkan-dynamic:
	clang++ -dynamic -shared -fPIC -std=c++14 src/kgfx/kgfx_vulkan.cpp -o build/libkgfx.dylib $(FLAGS) $(MAC_LIBS) -Wno-switch

mac-dummy-dynamic:
	clang -dynamic -shared -fPIC src/kgfx/kgfx_dummy.c -o build/libkgfx.dylib $(FLAGS) -Wno-switch

mac-kgfx_gh-dynamic:
	clang -undefined dynamic_lookup -shared -fPIC src/kgfx_gh/kgfx_gh_cocoa.m -o build/libkgfx_gh.dylib $(FLAGS) -Llib/mac -framework IOKit -framework Cocoa -framework QuartzCore

mac-metal:
	clang -std=c99 -c src/main.c -o obj/main.o $(FLAGS)
	clang -c src/kgfx_gh_cocoa.m -o obj/kgfx_gh_cocoa.o $(FLAGS)
	clang++ -std=c++14 -c src/kgfx/kgfx_metal.mm -o obj/kgfx_metal.o $(FLAGS)
	clang++ obj/main.o obj/kgfx_gh_cocoa.o obj/kgfx_metal.o -o kgfx -Llib/mac -framework IOKit -framework Cocoa -framework QuartzCore -framework Metal -lglfw3

pylaunch:
	pylauncher ./kgfx triangle $(PWD)

clean:
	rm -rf build obj
	mkdir build obj
