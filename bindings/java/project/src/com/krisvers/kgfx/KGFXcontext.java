package com.krisvers.kgfx;

public class KGFXcontext {
	private long handle;
	public KGFXcontext(int major, int minor, int patch, KGFXwindow window) {
		handle = KGFXjni.createContext(major, minor, patch, window);
		if (handle == 0) {
			throw new RuntimeException("Failed to create KGFXcontext");
		}
	}

	public KGFXshader createShader(String entryName, String source, int type, int medium) {
		return new KGFXshader(this, entryName, source, type, medium);
	}

	public KGFXpipeline createPipeline(KGFXshader[] shaders, KGFXpipelinelayout layout, int cullMode, int frontFace, int fillMode, int topology) {
		return new KGFXpipeline(this, shaders, layout, cullMode, frontFace, fillMode, topology);
	}

	public KGFXbuffer createBuffer(int location, int usage, byte[] data, int size) {
		return new KGFXbuffer(this, location, usage, data, size);
	}
	public KGFXbuffer createBuffer(int location, int usage, float[] data, int size) {
		return new KGFXbuffer(this, location, usage, data, size);
	}

	public KGFXcommandlist createCommandList() {
		return new KGFXcommandlist(this);
	}

	public long getHandle() {
		return handle;
	}
	public void destroy() {
		KGFXjni.destroyContext(handle);
	}
}
