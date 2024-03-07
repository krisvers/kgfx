#include <jni.h>
#include <stdio.h>
#include <Cocoa/Cocoa.h>
#include <QuartzCore/QuartzCore.h>

JNIEXPORT void JNICALL Java_com_krisvers_kgfxgh_KGFXGHjni_kgfxWindowSetupCocoa(JNIEnv* env, jclass jclazz, jobject window) {
	jfieldID field = (*env)->GetFieldID(env, jclazz, "window", "J");
	jlong w = (*env)->GetLongField(env, window, field);

	NSWindow* win = (NSWindow*) w;
	field = (*env)->GetFieldID(env, jclazz, "contentView", "J");
	(*env)->SetLongField(env, window, field, [win contentView]);

	field = (*env)->GetFieldID(env, jclazz, "layer", "J");
	(*env)->SetLongField(env, window, field, [[win contentView] layer]);
}
