mac:
	clang -fPIC -shared -I../include -I$(HOME)/Library/Python/3.9/bin/jdk-21.0.2.jdk/Contents/Home/include/ -I$(HOME)/Library/Python/3.9/bin/jdk-21.0.2.jdk/Contents/Home/include/darwin $(shell find ./src -type f -name "*.c") -L. -lkgfx -o libkgfx_java.dylib
