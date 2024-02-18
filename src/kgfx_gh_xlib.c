#include "kgfx_gh.h"

#define GLFW_EXPOSE_NATIVE_X11
#include <GLFW/glfw3native.h>

KGFXwindow kgfxWindowFromGLFW(GLFWwindow* window) {
	KGFXwindow win;
	win.window = glfwGetX11Window(window);
	win.display = glfwGetX11Display();
	return win;
}