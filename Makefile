linux-vulkan:
	clang++ src/main.c src/kgfx/kgfx_vulkan.cpp src/kgfx_gh_xlib.c -Ilib/include -lglfw -lvulkan -o kgfx

mac-vulkan:
	clang -std=c99 -c src/main.c -o obj/main.o -Ilib/include
	clang -c src/kgfx_gh_cocoa.m -o obj/kgfx_gh_cocoa.o -Ilib/include
	clang++ -std=c++14 -c src/kgfx/kgfx_vulkan.cpp -o obj/kgfx_vulkan.o -Ilib/include -Wno-switch
	clang++ obj/main.o obj/kgfx_gh_cocoa.o obj/kgfx_vulkan.o -o kgfx -rpath lib/mac -Llib/mac -framework IOKit -framework Cocoa -framework QuartzCore -lMoltenVK -lglfw3

mac-metal:
	clang -std=c99 -c src/main.c -o obj/main.o -Ilib/include
	clang -c src/kgfx_gh_cocoa.m -o obj/kgfx_gh_cocoa.o -Ilib/include
	clang++ -std=c++14 -c src/kgfx/kgfx_metal.mm -o obj/kgfx_metal.o -Ilib/include
	clang++ obj/main.o obj/kgfx_gh_cocoa.o obj/kgfx_metal.o -o kgfx -Llib/mac -framework IOKit -framework Cocoa -framework QuartzCore -framework Metal -lglfw3

win-metal:
	"C:\Program Files\Metal Developer Tools\metal\macos\bin\metal.exe src/main.c src/kgfx/kgfx_metal.m src/kgfx_gh_cocoa.m -Ilib/include -Llib/mac -framework IOKit -framework Cocoa -framework Metal -framework QuartzCore -lglfw3 -o kgfx"

pylaunch:
	pylauncher ./kgfx $(PWD)
