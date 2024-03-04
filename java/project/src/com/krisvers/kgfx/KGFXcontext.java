package com.krisvers.kgfx;

public class KGFXcontext {
	public KGFXcontext(int major, int minor, int patch, KGFXwindow window) {
		return KGFX.createContext(major, minor, patch, window);
	}

	public native void createShader();
}
