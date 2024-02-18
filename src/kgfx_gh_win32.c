#include "kgfx_gh.h"

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

KGFXwindow kgfxWindowFromGLFW(GLFWwindow* window) {
	KGFXwindow win;
	win.hwnd = glfwGetWin32Window(window);
	return win;
}