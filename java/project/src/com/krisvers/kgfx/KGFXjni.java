package com.krisvers.kgfx;

import java.nio.ByteBuffer;

public class KGFXjni {
	static {
		try {
			System.loadLibrary("kgfx");
			System.loadLibrary("kgfx_java");
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
				throw new RuntimeException("Unsupported platform");
			}

			throw new RuntimeException("Failed to load kgfx library " + kgfx + " or " + kgfx_java);
		}
	}

	public static native long createContext(int major, int minor, int patch, KGFXwindow window);
	public static native void destroyContext(long context);

	public static native long createShader(long context, String entryName, byte[] data, int type, int medium);
	public static native void destroyShader(long context, long shader);

	public static native long createPipeline(long context, long[] shaders, KGFXpipelinelayout layout, int cullMode, int frontFace, int fillMode, int topology);
	public static native void destroyPipeline(long context, long pipeline);

	public static native long createBuffer(long context, int location, int usage, byte[] data, int size);
	public static native long createBufferFloats(long context, int location, int usage, float[] data, int size);
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