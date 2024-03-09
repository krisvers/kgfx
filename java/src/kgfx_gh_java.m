#include <jni.h>
#include <Cocoa/Cocoa.h>
#include <QuartzCore/QuartzCore.h>

JNIEXPORT void JNICALL Java_com_krisvers_kgfxgh_KGFXGHjni_kgfxWindowSetupCocoa(JNIEnv* env, jclass jclazz, jobject window) {
	jclass windowClass = (*env)->GetObjectClass(env, window);
	jfieldID field = (*env)->GetFieldID(env, windowClass, "window", "J");
	jlong w = (*env)->GetLongField(env, window, field);

	NSWindow* win = (__bridge NSWindow*) (void*) w;
	field = (*env)->GetFieldID(env, windowClass, "contentView", "J");
	(*env)->SetLongField(env, window, field, (jlong) [win contentView]);

	field = (*env)->GetFieldID(env, windowClass, "layer", "J");
	(*env)->SetLongField(env, window, field, (jlong) [[win contentView] layer]);
}
