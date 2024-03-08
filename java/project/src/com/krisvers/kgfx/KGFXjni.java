package com.krisvers.kgfx;

public class KGFXjni {
	public static native long createContext(int major, int minor, int patch, KGFXwindow window);
	public static native void destroyContext(long context);

	public static native long createShader(long context, String entryName, byte[] data, int type, int medium);
	public static native void destroyShader(long context, long shader);

	public static native long createPipeline(long context, long[] shaders, KGFXpipelinelayout layout, int cullMode, int frontFace, int fillMode, int topology);
	public static native void destroyPipeline(long context, long pipeline);

	public static native long createBuffer(long context, int location, int usage, byte[] data, int size);
	public static native long createBufferFloats(long context, int location, int usage, float[] data, int size);
	public static native void writeBufferBytes(long context, long buffer, byte[] data);
	public static native byte[] readBufferBytes(long context, long buffer);
	public static native void writeBufferFloats(long context, long buffer, float[] data);
	public static native float[] readBufferFloats(long context, long buffer);
	public static native void destroyBuffer(long context, long buffer);

	public static native long createCommandList(long context);
	public static native void commandReset(long context, long commandList);
	public static native void commandBindPipeline(long context, long commandList, long pipeline);
	public static native void commandBindVertexBuffer(long context, long commandList, long buffer, int binding);
	public static native void commandBindIndexBuffer(long context, long commandList, long buffer, int binding);
	public static native void commandBindDescriptorSetBuffer(long context, long commandList, long buffer, int binding);
	public static native void commandDraw(long context, long commandList, int vertexCount, int instanceCount, int firstVertex, int firstInstance);
	public static native void commandDrawIndexed(long context, long commandList, int indexCount, int instanceCount, int firstIndex, int vertexOffset, int firstInstance);
	public static native void commandPresent(long context, long commandList);
	public static native void commandListSubmit(long context, long commandList);
	public static native void destroyCommandList(long context, long commandList);

	public static native int getBackend();

	public static native int jniVersionMajor();
	public static native int jniVersionMinor();
	public static native int jniVersionPatch();

	public static native int implementationVersionMajor();
	public static native int implementationVersionMinor();
	public static native int implementationVersionPatch();

	/* enum */
	public static final int KGFX_JAVA_VERSION_MAJOR = 0;
	public static final int KGFX_JAVA_VERSION_MINOR = 1;
	public static final int KGFX_JAVA_VERSION_PATCH = 2;

	public static final int KGFX_BACKEND_UNKNOWN = 0;
	public static final int KGFX_BACKEND_GENERIC = KGFX_BACKEND_UNKNOWN;
	public static final int KGFX_BACKEND_VULKAN = 1;
	public static final int KGFX_BACKEND_D3D12 = 2;
	public static final int KGFX_BACKEND_METAL = 3;

	public static final int KGFX_SHADERTYPE_VERTEX = 0;
	public static final int KGFX_SHADERTYPE_FRAGMENT = 1;
	public static final int KGFX_SHADERTYPE_GEOMETRY = 2;
	public static final int KGFX_SHADERTYPE_COMPUTE = 3;

	public static final int KGFX_MEDIUM_SPIRV = 0;
	public static final int KGFX_MEDIUM_GLSL = 1;
	public static final int KGFX_MEDIUM_HLSL = 2;
	public static final int KGFX_MEDIUM_MSL = 3;


	public static final int KGFX_DATATYPE_FLOAT = 1;
	public static final int KGFX_DATATYPE_FLOAT2 = 2;
	public static final int KGFX_DATATYPE_FLOAT3 = 3;
	public static final int KGFX_DATATYPE_FLOAT4 = 4;
	public static final int KGFX_DATATYPE_INT = 5;
	public static final int KGFX_DATATYPE_INT2 = 6;
	public static final int KGFX_DATATYPE_INT3 = 7;
	public static final int KGFX_DATATYPE_INT4 = 8;
	public static final int KGFX_DATATYPE_UINT = 9;
	public static final int KGFX_DATATYPE_UINT2 = 10;
	public static final int KGFX_DATATYPE_UINT3 = 11;
	public static final int KGFX_DATATYPE_UINT4 = 12;
	public static final int KGFX_DATATYPE_MAT2 = 13;
	public static final int KGFX_DATATYPE_MAT3 = 14;
	public static final int KGFX_DATATYPE_MAT4 = 15;
	public static final int KGFX_DATATYPE_TEXTURE_SAMPLER = 16;

	public static final int KGFX_VERTEX_INPUT_RATE_VERTEX = 0;
	public static final int KGFX_VERTEX_INPUT_RATE_INSTANCE = 1;


	public static final int KGFX_BINDPOINT_VERTEX = 1;
	public static final int KGFX_BINDPOINT_FRAGMENT = 2;
	public static final int KGFX_BINDPOINT_GEOMETRY = 4;
	public static final int KGFX_BINDPOINT_COMPUTE = 8;

	public static final int KGFX_DESCRIPTOR_USAGE_INVALID = 0;
	public static final int KGFX_DESCRIPTOR_USAGE_UNIFORM_BUFFER = 1;
	public static final int KGFX_DESCRIPTOR_USAGE_STORAGE_BUFFER = 2;
	public static final int KGFX_DESCRIPTOR_USAGE_TEXTURE = 3;
	public static final int KGFX_DESCRIPTOR_USAGE_STORAGE_TEXTURE = 4;

	public static final int KGFX_CULLMODE_NONE = 0;
	public static final int KGFX_CULLMODE_FRONT = 1;
	public static final int KGFX_CULLMODE_BACK = 2;

	public static final int KGFX_FRONTFACE_CCW = 0;
	public static final int KGFX_FRONTFACE_CW = 1;

	public static final int KGFX_FILLMODE_SOLID = 0;
	public static final int KGFX_FILLMODE_LINES = 1;

	public static final int KGFX_TOPOLOGY_POINTS = 0;
	public static final int KGFX_TOPOLOGY_LINES = 1;
	public static final int KGFX_TOPOLOGY_TRIANGLES = 2;

	public static final int KGFX_BUFFER_LOCATION_CPU = 0;
	public static final int KGFX_BUFFER_LOCATION_GPU = 1;

	public static final int KGFX_BUFFER_USAGE_VERTEX_BUFFER = 1;
	public static final int KGFX_BUFFER_USAGE_INDEX_BUFFER = 2;
	public static final int KGFX_BUFFER_USAGE_UNIFORM_BUFFER = 4;
	public static final int KGFX_BUFFER_USAGE_STORAGE_BUFFER = 8;
	public static final int KGFX_BUFFER_USAGE_TEXTURE_SRC = 16;

	public static final int KGFX_TEXTURE_FORMAT_R8G8B8A8_UNORM = 0;
	public static final int KGFX_TEXTURE_FORMAT_R8G8B8A8_SRGB = 1;
	public static final int KGFX_TEXTURE_FORMAT_R32G32B32A32_SFLOAT = 2;
	public static final int KGFX_TEXTURE_FORMAT_R32G32B32_SFLOAT = 3;
	public static final int KGFX_TEXTURE_FORMAT_R32G32_SFLOAT = 4;
	public static final int KGFX_TEXTURE_FORMAT_R32_SFLOAT = 5;
	public static final int KGFX_TEXTURE_FORMAT_DEPTH = 6;

	public static final int KGFX_SAMPLER_FILTER_NEAREST = 0;
	public static final int KGFX_SAMPLER_FILTER_LINEAR = 1;

	public static final int KGFX_SAMPLER_ADDRESS_MODE_REPEAT = 0;
	public static final int KGFX_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT = 1;
	public static final int KGFX_SAMPLER_ADDRESS_MODE_CLAMP = 2;
	public static final int KGFX_SAMPLER_ADDRESS_MODE_MIRRORED_CLAMP = 3;

	static {
		try {
			System.loadLibrary("kgfx");
			System.loadLibrary("kgfx_java");

			if (jniVersionMajor() != KGFX_JAVA_VERSION_MAJOR || jniVersionMinor() != KGFX_JAVA_VERSION_MINOR) {
				throw new RuntimeException("JNI version mismatch (Java library version differs from JNI binding) expected (" + KGFX_JAVA_VERSION_MAJOR + "." + KGFX_JAVA_VERSION_MINOR + "." + KGFX_JAVA_VERSION_PATCH + ") got (" + jniVersionMajor() + "." + jniVersionMinor() + "." + jniVersionPatch() + ")");
			}
			if (jniVersionPatch() != KGFX_JAVA_VERSION_PATCH) {
				System.out.println("Warning: JNI patch version mismatch (Java library version differs from JNI binding) expected (" + KGFX_JAVA_VERSION_MAJOR + "." + KGFX_JAVA_VERSION_MINOR + "." + KGFX_JAVA_VERSION_PATCH + ") got (" + jniVersionMajor() + "." + jniVersionMinor() + "." + jniVersionPatch() + ")");
			}
		} catch (UnsatisfiedLinkError e) {
			String kgfx;
			String kgfx_java;

			String os = System.getProperty("os.name").toLowerCase();
			if (os.contains("win")) {
				kgfx = "kgfx.dll";
				kgfx_java = "kgfx_java.dll";
			} else if (os.contains("nix") || os.contains("nux")) {
				kgfx = "libkgfx.so";
				kgfx_java = "libkgfx_java.so";
			} else if (os.contains("mac")) {
				kgfx = "libkgfx.dylib";
				kgfx_java = "libkgfx_java.dylib";
			} else {
				e.printStackTrace();
				throw new RuntimeException("Unsupported platform");
			}

			e.printStackTrace();
			throw new RuntimeException("Failed to load kgfx library " + kgfx + " or " + kgfx_java);
		}
	}
}