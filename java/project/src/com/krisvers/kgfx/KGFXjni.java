package com.krisvers.kgfx;

import java.nio.ByteBuffer;

public class KGFXjni {
	static {
		System.loadLibrary("kgfx");
		System.loadLibrary("kgfx_java");
	}

	public static native long createContext(int major, int minor, int patch, KGFXwindow window);
	public static native void destroyContext(long context);

	public static native long createShader(long context, String entryName, byte[] data, int type, int medium);
	public static native void destroyShader(long context, long shader);

	public static native long createPipeline(long context, long[] shaders, KGFXpipelinelayout layout, int cullMode, int frontFace, int fillMode, int topology);
}