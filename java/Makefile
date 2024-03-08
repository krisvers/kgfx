mac-kgfx-java:
	clang -fPIC -shared -I../include -I$(HOME)/Library/Python/3.9/bin/jdk-21.0.2.jdk/Contents/Home/include/ -I$(HOME)/Library/Python/3.9/bin/jdk-21.0.2.jdk/Contents/Home/include/darwin ./src/kgfx_java.c -L. -lkgfx -o libkgfx_java.dylib

mac-kgfx_gh-java:
	clang -fobjc-arc -fPIC -shared -I../lib/include -I../include -I$(HOME)/Library/Python/3.9/bin/jdk-21.0.2.jdk/Contents/Home/include/ -I$(HOME)/Library/Python/3.9/bin/jdk-21.0.2.jdk/Contents/Home/include/darwin ./src/kgfx_gh_java.m -o libkgfx_gh_java.dylib

linux-kgfx-java:
	clang -fPIC -shared -I../include -I$(JAVA_HOME)/include -I$(JAVA_HOME)/include/linux ./src/kgfx_java.c -L. -lkgfx -o libkgfx_java.so
