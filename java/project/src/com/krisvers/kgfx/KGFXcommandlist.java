package com.krisvers.kgfx;

public class KGFXcommandlist {
	private long handle;
	private long context;

	public KGFXcommandlist(KGFXcontext ctx) {
		if (ctx == null) {
			throw new IllegalArgumentException("ctx must not be null");
		}

		context = ctx.getHandle();
		handle = KGFXjni.createCommandList(context);
		if (handle == 0) {
			throw new RuntimeException("Failed to create KGFXcommandlist");
		}
	}

	public void destroy() {
		KGFXjni.destroyCommandList(context, handle);
	}

	public void reset() {
		KGFXjni.commandReset(context, handle);
	}

	public void bindVertexBuffer(KGFXbuffer buffer, int binding) {
		KGFXjni.commandBindVertexBuffer(context, handle, buffer.getHandle(), binding);
	}

	public void bindIndexBuffer(KGFXbuffer buffer) {
		throw new RuntimeException("Not implemented");
	}

	public void bindPipeline(KGFXpipeline pipeline) {
		KGFXjni.commandBindPipeline(context, handle, pipeline.getHandle());
	}

	public void draw(int vertexCount, int instanceCount, int firstVertex, int firstInstance) {
		KGFXjni.commandDraw(context, handle, vertexCount, instanceCount, firstVertex, firstInstance);
	}

	public void present() {
		KGFXjni.commandPresent(context, handle);
	}

	public void submit() {
		KGFXjni.commandListSubmit(context, handle);
	}

	public long getHandle() {
		return handle;
	}
}
