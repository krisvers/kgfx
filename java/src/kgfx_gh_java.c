#include <kgfx/kgfx.h>
#include <kgfx_gh/kgfx_gh.h>
#include <jni.h>
#include <GLFW/glfw3.h>

JNIEXPORT jobject JNICALL Java_com_krisvers_kgfxgh_KGFXGHjni_kgfxWindowFromGLFW(JNIEnv* env, jclass jclazz, jlong glfwWindow) {
	KGFXwindow window = kgfxWindowFromGLFW((GLFWwindow*) glfwWindow);

	jclass windowClass = (*env)->FindClass(env, "com/krisvers/kgfx/KGFXwindow");
	jmethodID windowConstructor = (*env)->GetMethodID(env, windowClass, "<init>", "()V");
	jobject windowObject = (*env)->NewObject(env, windowClass, windowConstructor);
	if (windowObject == NULL) {
		printf("Error\n");
		return NULL;
	}

	jfieldID field = (*env)->GetFieldID(env, windowClass, "window", "J");
	#ifdef KGFX_WINDOWS
	(*env)->SetLongField(env, windowObject, field, (jlong) window.hwnd);
	#else
		(*env)->SetLongField(env, windowObject, field, (jlong) window.window);
		#ifdef KGFX_LINUX
		field = (*env)->GetFieldID(env, windowClass, "display", "J");
		(*env)->SetLongField(env, windowObject, field, (jlong) window.display);
		#elif KGFX_MACOS
		field = (*env)->GetFieldID(env, windowClass, "contentView", "J");
		(*env)->SetLongField(env, windowObject, field, (jlong) window.contentView);
		field = (*env)->GetFieldID(env, windowClass, "layer", "J");
		(*env)->SetLongField(env, windowObject, field, (jlong) window.layer);
		#endif
	#endif

	return windowObject;
}
