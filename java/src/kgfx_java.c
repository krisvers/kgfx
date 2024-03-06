#include <kgfx/kgfx.h>
#include <jni.h>
#include <stdio.h>
#include <stdlib.h>

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

JNIEXPORT jlong JNICALL Java_com_krisvers_kgfx_KGFXjni_createShader(JNIEnv* env, jclass clazz, jlong context, jstring entryName, jbyteArray data, jint type, jint medium) {
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
	(*env)->ReleaseByteArrayElements(env, data, bytes, JNI_ABORT);
	return (jlong) shader;
}

JNIEXPORT void JNICALL Java_com_krisvers_kgfx_KGFXjni_destroyShader(JNIEnv* env, jclass clazz, jlong context, jlong shader) {
	kgfxDestroyShader((KGFXcontext) context, (KGFXshader) shader);
}

JNIEXPORT jlong JNICALL Java_com_krisvers_kgfx_KGFXjni_createPipeline(JNIEnv* env, jclass clazz, jlong context, jlongArray shaders, jobject jlayout, jint cullMode, jint frontFace, jint fillMode, jint topology) {
	KGFXpipelinedesc desc = {
		.cullMode = (KGFXcullmode) cullMode,
		.frontFace = (KGFXfrontface) frontFace,
		.fillMode = (KGFXfillmode) fillMode,
		.topology = (KGFXtopology) topology
	};
	
	jclass layoutClass = (*env)->GetObjectClass(env, jlayout);
	jfieldID field = (*env)->GetFieldID(env, layoutClass, "bindings", "[Lcom/krisvers/kgfx/KGFXpipelinebinding;");
	jobjectArray jbindings = (jobjectArray) (*env)->GetObjectField(env, jlayout, field);
	if (jbindings == NULL) {
		printf("Failed to get bindings\n");
		return 0;
	}
	
	jsize len = (*env)->GetArrayLength(env, jbindings);
	KGFXpipelinebinding* bindings = malloc(sizeof(KGFXpipelinebinding) * len);
	if (bindings == NULL) {
		printf("Failed to malloc binding array\n");
		return 0;
	}
	
	for (jsize i = 0; i < len; i++) {
		jobject binding = (*env)->GetObjectArrayElement(env, jbindings, i);
		jclass bindingClass = (*env)->GetObjectClass(env, binding);

		field = (*env)->GetFieldID(env, bindingClass, "inputRate", "I");
		bindings[i].inputRate = (KGFXinputrate) (*env)->GetIntField(env, binding, field);

		field = (*env)->GetFieldID(env, bindingClass, "bindpoint", "I");
		bindings[i].bindpoint = (*env)->GetIntField(env, binding, field);

		field = (*env)->GetFieldID(env, bindingClass, "binding", "I");
		bindings[i].binding = (*env)->GetIntField(env, binding, field);

		field = (*env)->GetFieldID(env, bindingClass, "attributes", "[Lcom/krisvers/kgfx/KGFXpipelineattribute;");
		jobjectArray attributes = (jobjectArray) (*env)->GetObjectField(env, binding, field);

		jsize attrLen = (*env)->GetArrayLength(env, attributes);
		bindings[i].pAttributes = malloc(sizeof(KGFXpipelineattribute) * attrLen);
		if (bindings[i].pAttributes == NULL) {
			printf("Failed to malloc attribute array\n");
			for (jsize j = 0; j < i; j++) {
				free(bindings[j].pAttributes);
			}
			free(bindings);
			return 0;
		}
		bindings[i].attributeCount = attrLen;
		
		for (jsize j = 0; j < attrLen; j++) {
			jobject attribute = (*env)->GetObjectArrayElement(env, attributes, j);
			jclass attributeClass = (*env)->GetObjectClass(env, attribute);

			field = (*env)->GetFieldID(env, attributeClass, "semanticName", "Ljava/lang/String;");
			jstring string = (jstring) (*env)->GetObjectField(env, attribute, field);
			if (string == NULL) {
				printf("Invalid semanticName\n");
				for (jsize j = 0; j < i; j++) {
					free(bindings[j].pAttributes);
				}
				free(bindings);
				return 0;
			}
			bindings[i].pAttributes[j].semanticName = (*env)->GetStringUTFChars(env, string, NULL);
			
			field = (*env)->GetFieldID(env, attributeClass, "type", "I");
			bindings[i].pAttributes[j].type = (KGFXdatatype) (*env)->GetIntField(env, attribute, field);
			
			field = (*env)->GetFieldID(env, attributeClass, "location", "I");
			bindings[i].pAttributes[j].location = (*env)->GetIntField(env, attribute, field);
		}
	}

	desc.layout.pBindings = bindings;
	desc.layout.bindingCount = len;
	desc.layout.descriptorSetCount = 0;
	desc.layout.pDescriptorSets = NULL;

	desc.shaderCount = (*env)->GetArrayLength(env, shaders);
	desc.pShaders = (KGFXshader*) (*env)->GetLongArrayElements(env, shaders, 0);
	if (desc.pShaders == NULL) {
		printf("Failed to malloc shader array\n");
		for (jsize i = 0; i < len; i++) {
			jobject binding = (*env)->GetObjectArrayElement(env, jbindings, i);
			jclass bindingClass = (*env)->GetObjectClass(env, binding);
			field = (*env)->GetFieldID(env, bindingClass, "attributes", "[Lcom/krisvers/kgfx/KGFXpipelineattribute;");
			jobjectArray attributes = (jobjectArray) (*env)->GetObjectField(env, binding, field);
			for (u32 j = 0; j < bindings[i].attributeCount; ++j) {
				jobject attribute = (*env)->GetObjectArrayElement(env, attributes, j);
				jclass attributeClass = (*env)->GetObjectClass(env, attribute);
				(*env)->ReleaseStringUTFChars(env, (jstring)(*env)->GetObjectField(env, attribute, (*env)->GetFieldID(env, attributeClass, "semanticName", "Ljava/lang/String;")), bindings[i].pAttributes[j].semanticName);
			}
			(*env)->ReleaseLongArrayElements(env, shaders, (jlong*) desc.pShaders, JNI_ABORT);
		}
		free(bindings);
		return 0;
	}
	
	jlong pipeline = (jlong) kgfxCreatePipeline((KGFXcontext) context, desc);
	for (jsize i = 0; i < len; i++) {
		jobject binding = (*env)->GetObjectArrayElement(env, jbindings, i);
		jclass bindingClass = (*env)->GetObjectClass(env, binding);
		field = (*env)->GetFieldID(env, bindingClass, "attributes", "[Lcom/krisvers/kgfx/KGFXpipelineattribute;");

		for (u32 j = 0; j < bindings[i].attributeCount; j++) {
			jobject attribute = (*env)->GetObjectArrayElement(env, (jobjectArray) (*env)->GetObjectField(env, binding, field), j);
			jclass attributeClass = (*env)->GetObjectClass(env, attribute);
			(*env)->ReleaseStringUTFChars(env, (jstring)(*env)->GetObjectField(env, attribute, (*env)->GetFieldID(env, attributeClass, "semanticName", "Ljava/lang/String;")), bindings[i].pAttributes[j].semanticName);
		}
		free(bindings[i].pAttributes);
	}
	(*env)->ReleaseLongArrayElements(env, shaders, (jlong*) desc.pShaders, JNI_ABORT);
	free(bindings);

	return pipeline;
}

JNIEXPORT void JNICALL Java_com_krisvers_kgfx_KGFXjni_destroyPipeline(JNIEnv* env, jclass clazz, jlong context, jlong pipeline) {
	kgfxDestroyPipeline((KGFXcontext) context, (KGFXpipeline) pipeline);
}

JNIEXPORT jlong JNICALL Java_com_krisvers_kgfx_KGFXjni_createBuffer(JNIEnv* env, jclass clazz, jlong context, jint location, jint usage, jbyteArray data, jint size) {
	if (data == NULL && size == 0) {
		printf("If data is null, then size must not be 0\n");
		return 0;
	}
	
	if (size < 0) {
		printf("Size must not be negative\n");
		return 0;
	}

	jbyte* bytes = NULL;
	jsize len = 0;
	if (data != NULL) {
		bytes = (*env)->GetByteArrayElements(env, data, NULL);
		len = (*env)->GetArrayLength(env, data);

		if (size > len) {
			printf("If data is not null, the provided size must not be larger than data.length\n");
			(*env)->ReleaseByteArrayElements(env, data, bytes, JNI_ABORT);
			return 0;
		}
	}

	KGFXbufferdesc desc = {
		.location = location,
		.usage = usage,
		.pData = bytes,
		.size = size,
	};
	return (jlong) kgfxCreateBuffer((KGFXcontext) context, desc);
}

JNIEXPORT void JNICALL Java_com_krisvers_kgfx_KGFXjni_destroyBuffer(JNIEnv* env, jclass clazz, jlong context, jlong buffer) {
	kgfxDestroyBuffer((KGFXcontext) context, (KGFXbuffer) buffer);
}


JNIEXPORT jlong JNICALL Java_com_krisvers_kgfx_KGFXjni_createCommandList(JNIEnv* env, jclass clazz, jlong context) {

}

JNIEXPORT void JNICALL Java_com_krisvers_kgfx_KGFXjni_commandReset(JNIEnv* env, jclass clazz, jlong context, jlong commandList) {

}

JNIEXPORT void JNICALL Java_com_krisvers_kgfx_KGFXjni_commandBindPipeline(JNIEnv* env, jclass clazz, jlong context, jlong commandList,jlong pipeline) {

}

JNIEXPORT void JNICALL Java_com_krisvers_kgfx_KGFXjni_commandBindVertexBuffer(JNIEnv* env, jclass clazz, jlong context, jlong commandList,jlong buffer, jint binding) {

}

JNIEXPORT void JNICALL Java_com_krisvers_kgfx_KGFXjni_commandDraw(JNIEnv* env, jclass clazz, jlong context, jlong commandList, jint vertexCount, jint instanceCount, jint firstVertex, jint firstInstance) {

}

JNIEXPORT void JNICALL Java_com_krisvers_kgfx_KGFXjni_commandPresent(JNIEnv* env, jclass clazz, jlong context, jlong commandList) {

}

JNIEXPORT void JNICALL Java_com_krisvers_kgfx_KGFXjni_commandListSubmit(JNIEnv* env, jclass clazz, jlong context, jlong commandList) {

}

JNIEXPORT void JNICALL Java_com_krisvers_kgfx_KGFXjni_destroyCommandList(JNIEnv* env, jclass clazz, jlong context, jlong commandList) {

}
