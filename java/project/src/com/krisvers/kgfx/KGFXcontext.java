package com.krisvers.kgfx;

public class KGFXcontext {
	private long handle;
	public KGFXcontext(int major, int minor, int patch, KGFXwindow window) {
		handle = KGFXjni.createContext(major, minor, patch, window);
		if (handle == 0) {
			throw new RuntimeException("Failed to create KGFXcontext");
		}
	}

	public long getHandle() {
		return handle;
	}

	public void destroy() {
		KGFXjni.destroyContext(handle);
	}
}
