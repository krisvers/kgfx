package com.krisvers.kgfx;

import java.nio.charset.StandardCharsets;

public class KGFXshader {
	private long handle;
	private long context;

	public KGFXshader(KGFXcontext ctx, String entryName, String source, int type, int medium) {
		if (ctx == null) {
			throw new IllegalArgumentException("context must not be null");
		}
		if (entryName == null) {
			throw new IllegalArgumentException("entryName must not be null");
		}
		if (source == null) {
			throw new IllegalArgumentException("source must not be null");
		}

		context = ctx.getHandle();
		handle = KGFXjni.createShader(context, entryName, source.getBytes(StandardCharsets.UTF_8), type, medium);
		if (handle == 0) {
			throw new RuntimeException("Failed to create shader");
		}
	}

	public void destroy() {
		KGFXjni.destroyShader(context, handle);
	}

	public long getHandle() {
		return handle;
	}
}
