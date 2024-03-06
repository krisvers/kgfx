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
	public static native void destroyPipeline(long context, long pipeline);

	public static native long createBuffer(long context, int location, int usage, byte[] data, int size);
	public static native void destroyBuffer(long context, long buffer);

	public static native long createCommandList(long context);
	public static native void commandReset(long context, long commandList);
	public static native void commandBindPipeline(long context, long commandList, long pipeline);
	public static native void commandBindVertexBuffer(long context, long commandList, long buffer, int binding);
	public static native void commandDraw(long context, long commandList, int vertexCount, int instanceCount, int firstVertex, int firstInstance);
	public static native void commandPresent(long context, long commandList);
	public static native void commandListSubmit(long context, long commandList);
	public static native void destroyCommandList(long context, long commandList);
}