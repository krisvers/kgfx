#include <kgfx/kgfx.h>
#include <kgfx_gh/kgfx_gh.h>
#include <GLFW/glfw3.h>

int main() {
	if (!glfwInit()) {
		return 1;
	}

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	GLFWwindow* window = glfwCreateWindow(800, 600, "triangle", NULL, NULL);
	if (window == NULL) {
		return 1;
	}

	KGFXwindow kgfxWindow = kgfxWindowFromGLFW(window);

	KGFXcontext context;
	if (kgfxCreateContext(KGFX_ANY_VERSION, kgfxWindow, &context) != KGFX_SUCCESS) {
		return 1;
	}

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
	}

	kgfxDestroyContext(context);

	return 0;
}
