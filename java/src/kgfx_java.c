#include <kgfx/kgfx.h>
#include <jni.h>
#include <stdio.h>

JNIEXPORT void JNICALL Java_com_krisvers_kgfx_KGFX_createContext(JNIEnv* env, jclass class) {
	printf("env %p class %p\n", env, class);
}
