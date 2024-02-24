FLAGS = -Ilib/include -Iinclude -Wno-switch -Wno-deprecated-declarations -Wno-unused-function

MAC_LIBS = -Llib/mac -lglfw3 -framework IOKit -framework Cocoa -framework QuartzCore -lMoltenVK -lglfw3
LINUX_LIBS = -lglfw -lvulkan

linux-vulkan:
	clang++ src/current.c src/kgfx/kgfx_vulkan.cpp src/kgfx_gh/kgfx_gh_xlib.c $(FLAGS) $(LINUX_LIBS) -o kgfx

mac-vulkan:
	clang -std=c99 -c src/main.c -o obj/main.o $(FLAGS)
	clang -c src/kgfx_gh_cocoa.m -o obj/kgfx_gh_cocoa.o $(FLAGS)
	clang++ -std=c++14 -c src/kgfx/kgfx_vulkan.cpp -o obj/kgfx_vulkan.o $(FLAGS) -Wno-switch
	clang++ obj/main.o obj/kgfx_gh_cocoa.o obj/kgfx_vulkan.o -o kgfx -rpath lib/mac $(MAC_LIBS)

mac-metal:
	clang -std=c99 -c src/main.c -o obj/main.o $(FLAGS)
	clang -c src/kgfx_gh_cocoa.m -o obj/kgfx_gh_cocoa.o $(FLAGS)
	clang++ -std=c++14 -c src/kgfx/kgfx_metal.mm -o obj/kgfx_metal.o $(FLAGS)
	clang++ obj/main.o obj/kgfx_gh_cocoa.o obj/kgfx_metal.o -o kgfx -Llib/mac -framework IOKit -framework Cocoa -framework QuartzCore -framework Metal -lglfw3

pylaunch:
	pylauncher ./kgfx $(PWD)
