#include <kgfx/kgfx.h>
#include <jni.h>
#include <stdio.h>

JNIEXPORT jlong JNICALL Java_com_krisvers_kgfx_KGFXjni_createContext(JNIEnv* env, jclass clazz, jint major, jint minor, jint patch, jobject window) {
	jclass windowClass = (*env)->GetObjectClass(env, window);
	jfieldID field = (*env)->GetFieldID(env, windowClass, "window", "J");
	jlong windowPtr = (*env)->GetLongField(env, window, field);

	KGFXwindow w;
	#ifdef KGFX_WINDOWS
	w.hwnd = (void*) windowPtr;
	#else
	w.window = (void*) windowPtr;
		#ifdef KGFX_LINUX
		{
			jfieldID field = (*env)->GetFieldID(env, windowClass, "display", "J");
			jlong display = (*env)->GetLongField(env, window, field);
			w.display = (void*) display;
		}
		#elif KGFX_MACOS
		{
			jfieldID field = (*env)->GetFieldID(env, windowClass, "contentView", "J");
			jlong view = (*env)->GetLongField(env, window, field);
			w.contentView = (void*) view;
		}
		#endif
	#endif

	KGFXcontext ctx;
	kgfxCreateContext(KGFX_MAKE_VERSION((u8) major, (u16) minor, (u16) patch), w, &ctx);
	return (jlong) ctx;
}

JNIEXPORT void JNICALL Java_com_krisvers_kgfx_KGFXjni_destroyContext(JNIEnv* env, jclass clazz, jlong context) {
	kgfxDestroyContext((KGFXcontext) context);
}

JNIEXPORT long JNICALL Java_com_krisvers_kgfx_KGFXjni_createShader(JNIEnv* env, jclass clazz, jlong context, jstring entryName, jbyteArray data, jint type, jint medium) {
	const char* entry = (*env)->GetStringUTFChars(env, entryName, 0);
	jsize len = (*env)->GetArrayLength(env, data);
	jbyte* bytes = (*env)->GetByteArrayElements(env, data, 0);

	KGFXshaderdesc desc = {
		.entryName = entry,
		.pData = (void*) bytes,
		.size = len,
		.type = (KGFXshadertype) type,
		.medium = (KGFXshadermedium) medium
	};
	KGFXshader shader = kgfxCreateShader((KGFXcontext) context, desc);

	(*env)->ReleaseStringUTFChars(env, entryName, entry);
	(*env)->ReleaseByteArrayElements(env, data, bytes, 0);
	return (long) shader;
}

JNIEXPORT void JNICALL Java_com_krisvers_kgfx_KGFXjni_destroyShader(JNIEnv* env, jclass clazz, jlong context, jlong shader) {
	kgfxDestroyShader((KGFXcontext) context, (KGFXshader) shader);
}

JNIEXPORT long JNICALL Java_com_krisvers_kgfx_KGFXjni_createPipeline(JNIEnv* env, jclass clazz, jlong context, jlongArray shaders, KGFXpipelinelayout layout, jint cullMode, jint frontFace, jint fillMode, jint topology) {
	KGFXshader shaders[6] = {
		(KGFXshader) vertexShader,
		(KGFXshader) fragmentShader,
		(KGFXshader) geometryShader,
		(KGFXshader) tessControlShader,
		(KGFXshader) tessEvalShader,
		(KGFXshader) computeShader
	};
	KGFXpipeline pipeline = kgfxCreatePipeline((KGFXcontext) context, shaders, (KGFXvertexlayout) vertexLayout);
	return (long) pipeline;
})