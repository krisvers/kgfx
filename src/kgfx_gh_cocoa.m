#include "kgfx_gh.h"

#define GLFW_EXPOSE_NATIVE_COCOA
#include <GLFW/glfw3native.h>

KGFXwindow kgfxWindowFromGLFW(GLFWwindow* window) {
	KGFXwindow win;
	win.window = glfwGetCocoaWindow(window);
	return win;
}