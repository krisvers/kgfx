package com.krisvers.kgfx;

public class KGFXbuffer {
	private long handle;
	private long context;

	public KGFXbuffer(KGFXcontext ctx, int location, int usage, byte[] data, int size) {
		if (ctx == null) {
			throw new IllegalArgumentException("ctx must not be null");
		}
		if (data == null) {
			throw new IllegalArgumentException("data must not be null");
		}

		context = ctx.getHandle();
		handle = KGFXjni.createBuffer(context, location, usage, data, size);
		if (handle == 0) {
			throw new RuntimeException("Failed to create KGFXbuffer");
		}
	}

	public KGFXbuffer(KGFXcontext ctx, int location, int usage, float[] data, int size) {
		if (ctx == null) {
			throw new IllegalArgumentException("ctx must not be null");
		}
		if (data == null) {
			throw new IllegalArgumentException("data must not be null");
		}

		context = ctx.getHandle();
		handle = KGFXjni.createBufferFloats(context, location, usage, data, size);
		if (handle == 0) {
			throw new RuntimeException("Failed to create KGFXbuffer");
		}
	}

	public void destroy() {
		KGFXjni.destroyBuffer(context, handle);
	}

	public long getHandle() {
		return handle;
	}
}
