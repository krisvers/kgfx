package com.krisvers.kgfx;

public class KGFXpipeline {
	private long handle;
	private long context;

	public KGFXpipeline(KGFXcontext ctx, KGFXshader[] shaders, KGFXpipelinelayout layout, int cullMode, int frontFace, int fillMode, int topology) {
		if (ctx == null) {
			throw new IllegalArgumentException("ctx must not be null");
		}
		if (shaders == null) {
			throw new IllegalArgumentException("shader must not be null");
		}
		if (layout == null) {
			throw new IllegalArgumentException("layout must not be null");
		}

		context = ctx.getHandle();
		long[] shaderHandles = new long[shaders.length];
		for (int i = 0; i < shaders.length; i++) {
			shaderHandles[i] = shaders[i].getHandle();
		}

		handle = KGFXjni.createPipeline(context, shaderHandles, layout, cullMode, frontFace, fillMode, topology);
		if (handle == 0) {
			throw new RuntimeException("Failed to create KGFXpipeline");
		}
	}

	public void destroy() {
		KGFXjni.destroyPipeline(context, handle);
	}

	public long getHandle() {
		return handle;
	}
}
