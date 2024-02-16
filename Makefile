mac-metal:
	clang src/main.c src/kgfx/kgfx_metal.m -Ilib/include -Llib/mac -framework IOKit -framework Cocoa -framework Metal -framework QuartzCore -lglfw3 -o kgfx

pylaunch:
	pylauncher ./kgfx $(PWD)
