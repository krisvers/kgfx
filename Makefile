linux-vulkan:
	clang++ src/main.c src/kgfx/kgfx_vulkan.cpp src/kgfx_gh_xlib.c -Ilib/include -lglfw -lvulkan -o kgfx

mac-metal:
	clang src/main.c src/kgfx/kgfx_metal.m src/kgfx_gh_cocoa.m -Ilib/include -Llib/mac -framework IOKit -framework Cocoa -framework Metal -framework QuartzCore -lglfw3 -o kgfx

win-metal:
	"C:\Program Files\Metal Developer Tools\metal\macos\bin\metal.exe src/main.c src/kgfx/kgfx_metal.m src/kgfx_gh_cocoa.m -Ilib/include -Llib/mac -framework IOKit -framework Cocoa -framework Metal -framework QuartzCore -lglfw3 -o kgfx"

pylaunch:
	pylauncher ./kgfx $(PWD)
