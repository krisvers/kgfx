package com.krisvers.kgfx;
import org.lwjgl.glfw.*;

public class KGFXwindow {
	public long window;
	public long display;
	public long contentLayer;
	public long view;

	public KGFXwindow(long glfwWindow) {
		String platform = System.getProperty("os.name");
		String windows = "Windows";
		String linux = "Linux";
		String mac = "Mac OS X";

		boolean isWindows = windows.equals(platform.substring(0, windows.length()));
		boolean isLinux = linux.equals(platform.substring(0, linux.length()));
		boolean isMac = mac.equals(platform.substring(0, mac.length()));

		if (isMac) {
			throw new RuntimeException("Mac OS X window support is not implemented yet.");
		} else if (isLinux) {
			window = GLFWNativeX11.glfwGetX11Window(glfwWindow);
			display = GLFWNativeX11.glfwGetX11Display();
		} else if (isWindows) {
			window = GLFWNativeWin32.glfwGetWin32Window(glfwWindow);
		} else {
			throw new RuntimeException("Unknown platform: " + platform);
		}
	}
}
