#include "kgfx.h"

#ifdef _WIN32
#define VK_USE_PLATFORM_WIN32_KHR
#define SURFACE_EXTENSION_NAME VK_KHR_WIN32_SURFACE_EXTENSION_NAME

#define GLFW_EXPOSE_NATIVE_WIN32
#elif defined(__linux__)
#define VK_USE_PLATFORM_XCB_KHR
#define SURFACE_EXTENSION_NAME VK_KHR_XCB_SURFACE_EXTENSION_NAME

#define GLFW_EXPOSE_NATIVE_X11
#elif defined(__APPLE__)
#define VK_USE_PLATFORM_MACOS_MVK
#define SURFACE_EXTENSION_NAME VK_MVK_MACOS_SURFACE_EXTENSION_NAME

#define GLFW_EXPOSE_NATIVE_COCOA
#else
#error "Unsupported platform"
#endif

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <vector>

#define KGFX_IMPL_VER KGFX_MAKE_VERSION(1, 0, 0)

struct Vulkan {
	VkInstance instance;
	VkSurfaceKHR surface;
	struct KGFXcontext_t* ctx;

	VkResult init(void);
	void destroy(void);

	VkResult createSurface(void);
};

struct KGFXcontext_t {
	u32 version;

	GLFWwindow* window;
	Vulkan vulkan;
};

KGFXresult kgfxCreateContext(u32 version, KGFXcontext* context) {
	if (context == nullptr) {
		return KGFX_NULL_OUTPUT_ARGUMENT;
	}

	if ((KGFX_MAJOR(version) > KGFX_MAJOR(KGFX_IMPL_VER) && KGFX_MAJOR(version) != KGFX_ANY_MAJOR) || (KGFX_MINOR(version) > KGFX_MINOR(KGFX_IMPL_VER) && KGFX_MINOR(version) != KGFX_ANY_MINOR) || (KGFX_PATCH(version) > KGFX_PATCH(KGFX_IMPL_VER) && KGFX_PATCH(version) != KGFX_ANY_PATCH)) {
		return KGFX_VERSION_NOT_SUPPORTED;
	}

	KGFXcontext ctx = new KGFXcontext_t;
	ctx->version = (version == KGFX_ANY_VERSION) ? KGFX_IMPL_VER : version;
	ctx->vulkan.ctx = ctx;

	if (!glfwInit()) {
		delete ctx;
		return KGFX_GENERIC_ERROR;
	}

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	ctx->window = glfwCreateWindow(800, 600, "kgfx", nullptr, nullptr);
	if (ctx->window == nullptr) {
		glfwTerminate();
		delete ctx;
		return KGFX_GENERIC_ERROR;
	}

	VkResult result = ctx->vulkan.init();
	if (result != VK_SUCCESS) {
		delete ctx;
		return KGFX_GENERIC_ERROR;
	}

	*context = ctx;
	return KGFX_SUCCESS;
}

void kgfxDestroyContext(KGFXcontext ctx) {
	if (ctx == KGFX_HANDLE_NULL) {
		return;
	}

	ctx->vulkan.destroy();
	glfwDestroyWindow(ctx->window);
	delete ctx;
}

u8 kgfxGetImplementationVersion() {
	return KGFX_IMPL_VER;
}

VkResult Vulkan::init() {
	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "kgfx";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "kgfx";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_1;

	std::vector<const char*> extensions = {
		VK_KHR_SURFACE_EXTENSION_NAME,
		SURFACE_EXTENSION_NAME,
	};

	std::vector<const char*> layers = {
		"VK_LAYER_KHRONOS_validation",
	};

	VkInstanceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;
	createInfo.enabledExtensionCount = static_cast<u32>(extensions.size());
	createInfo.ppEnabledExtensionNames = extensions.data();
	createInfo.enabledLayerCount = static_cast<u32>(layers.size());
	createInfo.ppEnabledLayerNames = layers.data();

	VkResult res = vkCreateInstance(&createInfo, nullptr, &instance);
	if (res != VK_SUCCESS) {
		return res;
	}

	res = createSurface();
	if (res != VK_SUCCESS) {
		return res;
	}

	return VK_SUCCESS;
}

void Vulkan::destroy() {
	vkDestroySurfaceKHR(instance, surface, nullptr);
	vkDestroyInstance(instance, nullptr);
}

VkResult Vulkan::createSurface() {
#ifdef _WIN32
	VkWin32SurfaceCreateInfoKHR createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	createInfo.hinstance = GetModuleHandle(nullptr);
	createInfo.hwnd = glfwGetWin32Window(ctx->window);

	return vkCreateWin32SurfaceKHR(instance, &createInfo, nullptr, &surface);
#elif defined(__linux__)
	VkXcbSurfaceCreateInfoKHR createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR;
	createInfo.connection = xcb_connect(nullptr, nullptr);
	createInfo.window = glfwGetX11Window(ctx->window);

	return vkCreateXcbSurfaceKHR(instance, &createInfo, nullptr, &surface);
#elif defined(__APPLE__)
	VkMacOSSurfaceCreateInfoMVK createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_MACOS_SURFACE_CREATE_INFO_MVK;
	createInfo.pView = glfwGetCocoaWindow(ctx->window);

	return vkCreateMacOSSurfaceMVK(instance, &createInfo, nullptr, &surface);
#endif
}