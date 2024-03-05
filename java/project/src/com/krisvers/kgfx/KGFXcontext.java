package com.krisvers.kgfx;

public class KGFXcontext {
	private long handle;
	public KGFXcontext(int major, int minor, int patch, KGFXwindow window) {
		handle = KGFXjni.createContext(major, minor, patch, window);
	}

	public long getHandle() {
		return handle;
	}

	public void destroy() {
		KGFXjni.destroyContext(handle);
	}
}
